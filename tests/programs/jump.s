# Jump and upper-immediate test: JAL, JALR, LUI, AUIPC.
# Expected results are in the comments and checked by test.cpp.
# Assemble:
#   riscv64-unknown-elf-as -march=rv32i -mabi=ilp32 jump.s -o jump.o
#   riscv64-unknown-elf-objcopy -O binary jump.o jump.bin
# Addresses are checked as differences (e.g. "ra is 4 bytes before t0"),
# so the results don't depend on where the program is loaded.

.option norelax              # stop the linker rewriting instructions, so a linked ELF (for Spike) matches the .bin
.globl _start
_start:
    li    a0, 0
    li    a2, 0
    li    a3, 0
    li    a5, 0

    # JAL forward: link register = address of the next instruction
    jal   ra, 1f
    li    a0, 1               # skipped, a0 stays 0
1:  auipc t0, 0               # t0 = address of this instruction
    sub   a1, t0, ra          # 4: ra pointed at the skipped instruction

    # JALR: target = (rs1 + imm) with bit 0 cleared
    auipc t1, 0               # t1 = P, address of this instruction
    jalr  ra, 17(t1)          # target P+17 -> P+16 (bit 0 cleared), ra = P+8
    li    a2, 1               # P+8:  skipped
    li    a3, 1               # P+12: skipped
    sub   a4, ra, t1          # P+16: 8

    # JAL backward
    j     3f
2:  li    a5, 7               # only reached by jumping backwards
    j     4f
3:  j     2b
4:

    # function call and return (jal + jalr)
    li    a6, 0
    jal   ra, func
    addi  a6, a6, 1           # runs after the return: a6 = 11
    j     5f
func:
    li    a6, 10
    ret                       # jalr zero, 0(ra)
5:

    # LUI: immediate goes into the top 20 bits
    lui   a7, 0xFFFFF         # 0xFFFFF000
    # AUIPC: pc + (imm << 12)
    auipc s2, 0
    auipc s3, 1               # 4 bytes later, plus 0x1000
    sub   s4, s3, s2          # 0x1004

    ecall
