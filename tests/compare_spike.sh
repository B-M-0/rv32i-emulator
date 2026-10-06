#!/usr/bin/env bash
# Runs test programs on both the emulator (--trace) and Spike (--log-commits)
# and diffs the two traces line by line. The first differing line is the
# first instruction where the emulator disagrees with the reference.
# courtesy of claude code : 



set -uo pipefail

EMU=./build/rv32i
status=0
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

for src in "$@"; do
    name=$(basename "$src" .s)

    riscv64-unknown-elf-as -march=rv32i -mabi=ilp32 "$src" -o "$tmp/$name.o" || { status=1; continue; }
    # -N: don't page-align, so the program starts exactly at 0x80000000 (Spike has no memory below it)
    riscv64-unknown-elf-ld -m elf32lriscv -N --no-warn-rwx-segments -Ttext=0x80000000 "$tmp/$name.o" -o "$tmp/$name.elf" || { status=1; continue; }
    riscv64-unknown-elf-objcopy -O binary "$tmp/$name.elf" "$tmp/$name.bin"

    # emulator: trace lines only, without the final ecall
    "$EMU" --trace "$tmp/$name.bin" | grep '^core' | sed '$d' > "$tmp/emu.log"
    n=$(wc -l < "$tmp/emu.log")

    # Spike: its log goes to stderr. Keep program lines (pc >= 0x80000000), same count.
    timeout 10 spike --isa=rv32i --log-commits "$tmp/$name.elf" 2>&1 >/dev/null \
        | awk '$1 == "core" && $4 >= "0x80000000"' | head -n "$n" > "$tmp/spike.log"

    if diff -u --label spike --label emulator "$tmp/spike.log" "$tmp/emu.log" > "$tmp/diff"; then
        echo "$name: match ($n instructions)"
    else
        echo "$name: MISMATCH"
        head -n 20 "$tmp/diff"
        status=1
    fi
done
exit $status
