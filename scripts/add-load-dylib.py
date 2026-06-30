#!/usr/bin/env python3
import argparse
import shutil
import struct
from pathlib import Path

MH_MAGIC_64 = 0xfeedfacf
LC_SEGMENT_64 = 0x19
LC_LOAD_DYLIB = 0xc
LC_CODE_SIGNATURE = 0x1d


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
    for _, off, cmd, _ in load_commands(buf):
        if cmd != LC_SEGMENT_64:
            continue
        nsects = u32(buf, off + 64)
        sect_off = off + 72
        for _ in range(nsects):
            size = u64(buf, sect_off + 40)
            file_offset = u32(buf, sect_off + 48)
            if file_offset != 0 and size != 0:
                first = min(first, file_offset)
            sect_off += 80
        fileoff = u64(buf, off + 40)
        filesize = u64(buf, off + 48)
        if fileoff != 0 and filesize != 0:
            first = min(first, fileoff)
    return first


def existing_dylib_name(buf, off, cmdsize):
    name_off = u32(buf, off + 8)
    start = off + name_off
    end = off + cmdsize
    return bytes(buf[start:end]).split(b"\0", 1)[0].decode("utf-8", "replace")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("input", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("name")
    args = parser.parse_args()

    data = bytearray(args.input.read_bytes())
    if u32(data, 0) != MH_MAGIC_64:
        raise SystemExit("not a 64-bit Mach-O")
    for _, off, cmd, cmdsize in load_commands(data):
        if cmd == LC_LOAD_DYLIB and existing_dylib_name(data, off, cmdsize) == args.name:
            raise SystemExit(f"already has LC_LOAD_DYLIB {args.name!r}")

    old_ncmds = u32(data, 16)
    old_sizeofcmds = u32(data, 20)
    load_end = 32 + old_sizeofcmds
    insert_off = load_end
    for _, off, cmd_existing, _ in load_commands(data):
        if cmd_existing == LC_CODE_SIGNATURE:
            insert_off = off
            break
    name = args.name.encode() + b"\0"
    cmdsize = (24 + len(name) + 7) & ~7
    cmd = bytearray(cmdsize)
    put_u32(cmd, 0, LC_LOAD_DYLIB)
    put_u32(cmd, 4, cmdsize)
    put_u32(cmd, 8, 24)
    put_u32(cmd, 12, 2)
    put_u32(cmd, 16, 0)
    put_u32(cmd, 20, 0)
    cmd[24:24 + len(name)] = name

    payload_off = first_payload_offset(data)
    if insert_off + len(cmd) > payload_off:
        raise SystemExit(
            f"not enough load-command padding: need 0x{insert_off + len(cmd):x}, "
            f"payload starts at 0x{payload_off:x}"
        )

    if insert_off != load_end:
        data[insert_off + len(cmd):load_end + len(cmd)] = data[insert_off:load_end]
    data[insert_off:insert_off + len(cmd)] = cmd
    put_u32(data, 16, old_ncmds + 1)
    put_u32(data, 20, old_sizeofcmds + len(cmd))

    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(data)
    shutil.copymode(args.input, args.output)
    print(f"[+] added LC_LOAD_DYLIB {args.name} to {args.output}")


if __name__ == "__main__":
    main()
