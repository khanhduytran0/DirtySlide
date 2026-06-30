#!/usr/bin/env python3
import argparse
import shutil
import struct
import sys
from pathlib import Path

MH_MAGIC_64 = 0xfeedfacf
CPU_TYPE_ARM64 = 0x0100000c
MH_NOUNDEFS = 0x1
MH_DYLDLINK = 0x4
MH_TWOLEVEL = 0x80
MH_PIE = 0x200000

LC_SEGMENT_64 = 0x19
LC_UNIXTHREAD = 0x5
LC_LOAD_DYLINKER = 0xe
LC_MAIN = 0x80000028

ARM_THREAD_STATE64 = 6
ARM_THREAD_STATE64_COUNT = 68
LC_IGNORED_UNIXTHREAD = 0x999


def u32(buf, off):
    return struct.unpack_from("<I", buf, off)[0]


def u64(buf, off):
    return struct.unpack_from("<Q", buf, off)[0]


def put_u32(buf, off, value):
    struct.pack_into("<I", buf, off, value)


def load_commands(buf):
    ncmds = u32(buf, 16)
    off = 32
    for index in range(ncmds):
        cmd = u32(buf, off)
        cmdsize = u32(buf, off + 4)
        if cmdsize < 8:
            raise ValueError(f"bad load command size at index {index}: {cmdsize}")
        yield index, off, cmd, cmdsize
        off += cmdsize


def first_payload_offset(buf):
    first = len(buf)
    for _, off, cmd, cmdsize in load_commands(buf):
        if cmd == LC_SEGMENT_64:
            nsects = u32(buf, off + 64)
            sect_off = off + 72
            for _ in range(nsects):
                file_offset = u32(buf, sect_off + 48)
                size = u64(buf, sect_off + 40)
                if file_offset != 0 and size != 0:
                    first = min(first, file_offset)
                sect_off += 80
            fileoff = u64(buf, off + 40)
            filesize = u64(buf, off + 48)
            if fileoff != 0 and filesize != 0:
                first = min(first, fileoff)
        else:
            # Non-segment commands live in the load-command area.
            _ = cmdsize
    return first


def find_text_vmaddr(buf):
    for _, off, cmd, _ in load_commands(buf):
        if cmd != LC_SEGMENT_64:
            continue
        name = bytes(buf[off + 8:off + 24]).split(b"\0", 1)[0]
        if name == b"__TEXT":
            return u64(buf, off + 24)
    raise ValueError("missing __TEXT segment")


def find_unixthread_pc(buf):
    for _, off, cmd, cmdsize in load_commands(buf):
        if cmd != LC_UNIXTHREAD:
            continue
        cursor = off + 8
        end = off + cmdsize
        while cursor + 8 <= end:
            flavor = u32(buf, cursor)
            count = u32(buf, cursor + 4)
            cursor += 8
            byte_count = count * 4
            if cursor + byte_count > end:
                break
            if flavor == ARM_THREAD_STATE64 and count == ARM_THREAD_STATE64_COUNT:
                return u64(buf, cursor + 8 * 32)
            cursor += byte_count
    raise ValueError("missing arm64 LC_UNIXTHREAD pc")


def has_load_command(buf, command):
    return any(cmd == command for _, _, cmd, _ in load_commands(buf))


def patch(buf):
    if u32(buf, 0) != MH_MAGIC_64:
        raise ValueError("not a 64-bit Mach-O")
    if u32(buf, 4) != CPU_TYPE_ARM64:
        raise ValueError("not an arm64 Mach-O")

    if has_load_command(buf, LC_LOAD_DYLINKER) or has_load_command(buf, LC_MAIN):
        raise ValueError("binary already has dyld/main load commands")

    text_vmaddr = find_text_vmaddr(buf)
    pc = find_unixthread_pc(buf)
    entryoff = pc - text_vmaddr

    old_ncmds = u32(buf, 16)
    old_sizeofcmds = u32(buf, 20)
    insert_off = 32 + old_sizeofcmds

    dylinker = b"/usr/lib/dyld\0"
    dylinker_cmdsize = (12 + len(dylinker) + 7) & ~7
    dylinker_cmd = bytearray(dylinker_cmdsize)
    put_u32(dylinker_cmd, 0, LC_LOAD_DYLINKER)
    put_u32(dylinker_cmd, 4, dylinker_cmdsize)
    put_u32(dylinker_cmd, 8, 12)
    dylinker_cmd[12:12 + len(dylinker)] = dylinker

    main_cmd = bytearray(24)
    put_u32(main_cmd, 0, LC_MAIN)
    put_u32(main_cmd, 4, 24)
    struct.pack_into("<Q", main_cmd, 8, entryoff)
    struct.pack_into("<Q", main_cmd, 16, 0)

    addition = bytes(dylinker_cmd) + bytes(main_cmd)
    payload_off = first_payload_offset(buf)
    if insert_off + len(addition) > payload_off:
        raise ValueError(
            f"not enough load-command padding: need 0x{insert_off + len(addition):x}, "
            f"payload starts at 0x{payload_off:x}"
        )

    buf[insert_off:insert_off + len(addition)] = addition
    put_u32(buf, 16, old_ncmds + 2)
    put_u32(buf, 20, old_sizeofcmds + len(addition))
    put_u32(buf, 24, u32(buf, 24) | MH_NOUNDEFS | MH_DYLDLINK | MH_TWOLEVEL | MH_PIE)

    for _, off, cmd, _ in load_commands(buf):
        if cmd == LC_UNIXTHREAD:
            put_u32(buf, off, LC_IGNORED_UNIXTHREAD)
            break

    return entryoff


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("input", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()

    data = bytearray(args.input.read_bytes())
    try:
        entryoff = patch(data)
    except Exception as exc:
        print(f"patch failed: {exc}", file=sys.stderr)
        return 1

    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(data)
    shutil.copymode(args.input, args.output)
    print(f"[+] patched {args.output} entryoff=0x{entryoff:x}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
