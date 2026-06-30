#!/usr/bin/env python3
import argparse
import shutil
import struct
from pathlib import Path

MH_MAGIC_64 = 0xfeedfacf
LC_SEGMENT_64 = 0x19


def u32(buf, off):
    return struct.unpack_from("<I", buf, off)[0]


def put_u32(buf, off, value):
    struct.pack_into("<I", buf, off, value)


def load_commands(buf):
    ncmds = u32(buf, 16)
    off = 32
    for _ in range(ncmds):
        cmd = u32(buf, off)
        cmdsize = u32(buf, off + 4)
        if cmdsize < 8:
            raise ValueError("bad load command size")
        yield off, cmd, cmdsize
        off += cmdsize


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("input", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("segment")
    parser.add_argument("flag", type=lambda s: int(s, 0))
    args = parser.parse_args()

    data = bytearray(args.input.read_bytes())
    if u32(data, 0) != MH_MAGIC_64:
        raise SystemExit("not a 64-bit Mach-O")

    hits = 0
    want = args.segment.encode()
    for off, cmd, _ in load_commands(data):
        if cmd != LC_SEGMENT_64:
            continue
        name = bytes(data[off + 8:off + 24]).split(b"\0", 1)[0]
        if name == want:
            flags_off = off + 68
            put_u32(data, flags_off, u32(data, flags_off) | args.flag)
            hits += 1

    if hits == 0:
        raise SystemExit(f"segment {args.segment!r} not found")

    args.output.write_bytes(data)
    shutil.copymode(args.input, args.output)
    print(f"[+] set 0x{args.flag:x} on {hits} {args.segment} segment(s) in {args.output}")


if __name__ == "__main__":
    main()
