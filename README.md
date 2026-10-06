# rv32i-emulator

A RISC-V emulator following the RV32I unprivileged specification, written in C++20.

It runs pre-assembled RV32I machine code: you assemble a program with the RISC-V GNU toolchain, and the emulator loads the resulting flat binary and executes it.

## Status

- All 40 RV32I base instructions are decoded and executed.
- The emulator is a single-cycle functional model: each `step()` runs one whole instruction.
- Every instruction except `fence` and `ebreak` is checked by test programs. On all of them, the instruction-by-instruction trace matches [Spike](https://github.com/riscv-software-src/riscv-isa-sim), the official RISC-V reference simulator.

Planned next: a 5-stage pipeline model (IF, ID, EX, MEM, WB), then a custom compiler and toy language that target the emulator. See [Roadmap](#roadmap).

## Requirements

- CMake 3.16+ and a C++20 compiler (e.g. g++ 13)
- To assemble programs: the RISC-V GNU toolchain (`riscv64-unknown-elf-as`, `riscv64-unknown-elf-objcopy`, `riscv64-unknown-elf-ld`)
- Optional, for the reference comparison: [Spike](https://github.com/riscv-software-src/riscv-isa-sim)

## Building

```sh
cmake -S . -B build
cmake --build build
```

This builds the emulator (`build/rv32i`) and the test program (`build/test_decode`).

## Running a program

```sh
./build/rv32i programs/prog.bin
```

The emulator runs until the program halts, then prints the pc and all 32 registers:

```
pc       = 0x8000000c
x 0 zero = 0x00000000  (0)
...
x10   a0 = 0x00000005  (5)
x11   a1 = 0x00000008  (8)
```

### Writing your own program

Programs are written in RISC-V assembly and must end with `ecall`:

```asm
.globl _start
_start:
    addi a0, x0, 5
    addi a1, a0, 3
    ecall
```

Assemble it into a flat binary:

```sh
riscv64-unknown-elf-as -march=rv32i -mabi=ilp32 prog.s -o prog.o
riscv64-unknown-elf-objcopy -O binary prog.o prog.bin
```

How the emulator runs it:

| | |
|---|---|
| Memory | 16 MiB starting at `0x80000000` |
| Load address | the binary is copied to `0x80000000`, and execution starts there |
| Stack pointer | `sp` (x2) starts at the top of memory, `0x81000000` |
| Other registers | start at 0 |
| Halting | `ecall` and `ebreak` stop the program. An unrecognised instruction also stops it, with an error message |
| Errors | an access outside memory stops the emulator with an error and exit code 1 |

### Trace mode

```sh
./build/rv32i --trace programs/prog.bin
```

This prints one line per instruction before the register dump: the pc, the raw instruction word, and any register or memory write.

```
core   0: 3 0x80000000 (0x00500513) x10 0x00000005
core   0: 3 0x80000004 (0x00350593) x11 0x00000008
core   0: 3 0x80000008 (0x00000073)
```

Loads add `mem <address>`, and stores add `mem <address> <value>`. The format is the same as Spike's `--log-commits` output, so the two traces can be compared directly. The leading `3` is the privilege level: this emulator always runs in machine mode.

## Testing

```sh
cmake --build build
ctest --test-dir build --output-on-failure
```

`tests/test.cpp` contains two kinds of test:

- **Decoder tests:** check immediate and field extraction, and that every instruction in `tests/decoder/rv32all_dump.txt` (objdump output of `rv32i_all.s`) decodes to the right operation.
- **Program tests:** run each program in `tests/programs/` and check the final register values. The expected values are worked out by hand from the specification and are written next to each instruction in the `.s` files.

| Program | Covers |
|---|---|
| `alu_r.s` | R-type: overflow, shift amounts above 31, signed vs unsigned compare, writes to x0 |
| `alu_i.s` | I-type: sign-extended immediates, including for `sltiu`, `xori`, `andi` |
| `load_store.s` | every load/store width, sign vs zero extension, negative offsets, stores only changing their own bytes |
| `branch.s` | every branch taken and not taken, signed vs unsigned, a backward loop |
| `jump.s` | `jal`, `jalr` (including bit 0 cleared), call and return, `lui`, `auipc` |

The `.bin` files are assembled by hand and committed. After editing a `.s` file, reassemble it with the commands in its header comment, otherwise the tests still run the old binary.

### Comparing against Spike

`tests/compare_spike.sh` runs each program on both the emulator and Spike, and diffs the two traces. The first differing line is the first instruction where the emulator disagrees with the reference.

```sh
tests/compare_spike.sh programs/prog.s tests/programs/*.s
```

```
prog: match (2 instructions)
alu_i: match (21 instructions)
...
```

The script assembles each `.s` into an ELF linked at `0x80000000`, which is what Spike needs. It then makes the emulator's `.bin` from that same ELF, so both simulators run identical bytes. It also handles three differences between the two simulators:

- Spike first runs a few boot ROM instructions at `0x1000`. These lines are ignored.
- Spike doesn't log `ecall`, so the emulator's final `ecall` line is dropped. Before dropping it, the script checks that it really is an `ecall`, so a program that stopped early can't be reported as a match.
- Spike doesn't stop at `ecall`, so it is stopped after a timeout.

The script expects `spike` to be on your `PATH`.

```sh
git clone https://github.com/riscv-software-src/riscv-isa-sim
cd riscv-isa-sim && mkdir build && cd build
../configure --prefix=$HOME/.local
make -j$(nproc) && make install
```

## Project layout

```
include/Core.h                  Core class: registers, memory, step(), trace
include/decode.h                Instruction struct, Op and Format enums
src/Core.cpp                    instruction execution, memory access, trace output
src/decode.cpp                  instruction decoding and immediate extraction
src/main.cpp                    command-line program: load, run, print registers
programs/                       example program (prog.s / prog.bin)
tests/test.cpp                  decoder and program tests
tests/programs/                 test programs (.s and assembled .bin)
tests/compare_spike.sh          trace comparison against Spike
tests/decoder/rv32i_all.s       every RV32I instruction once, for the decoder tests
tests/decoder/rv32all_dump.txt  objdump output of rv32i_all.s
tests/decoder/test.s            instructions used for the immediate-extraction tests
```

## Roadmap

1. **5-stage pipeline model.** Split execution into IF, ID, EX, MEM and WB, with pipeline registers, hazard detection, forwarding and branch flushing. The current single-cycle core is kept as the reference: the pipelined core must produce the same trace on every program.
2. **Compiler and toy language.** A compiler that turns a small language into RV32I assembly, which then runs on the emulator.
