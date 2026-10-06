# R-type ALU test: every R-type instruction, with edge cases.
# Expected results are in the comments and checked by test.cpp.
# Assemble:
#   riscv64-unknown-elf-as -march=rv32i -mabi=ilp32 alu_r.s -o alu_r.o
#   riscv64-unknown-elf-objcopy -O binary alu_r.o alu_r.bin
# Note: li uses lui/addi, so this also relies on those working.

.globl _start
_start:
    # inputs
    li   t0, -8               # 0xFFFFFFF8
    li   t1, 3
    li   t2, 0x7FFFFFFF       # largest positive int
    li   s0, 1
    li   s1, 33               # shift amount > 31

    add  a0, t2, s0           # 0x80000000  overflow wraps around
    sub  a1, t1, t0           # 11          3 - (-8)
    sub  a2, s0, t1           # 0xFFFFFFFE  1 - 3 = -2
    and  a3, t0, t2           # 0x7FFFFFF8
    or   a4, t0, t1           # 0xFFFFFFFB
    xor  a5, t0, t2           # 0x80000007
    sll  a6, s0, s1           # 2           only the low 5 bits of the shift count: 33 -> 1
    sll  a7, t2, t1           # 0xFFFFFFF8
    srl  s2, t0, t1           # 0x1FFFFFFF  zeros shifted in
    sra  s3, t0, t1           # 0xFFFFFFFF  sign bit shifted in: -8 >> 3 = -1
    sra  s4, t2, t1           # 0x0FFFFFFF  positive value: same as srl
    slt  s5, t0, t1           # 1           signed:   -8 < 3
    slt  s6, t1, t0           # 0
    sltu s7, t0, t1           # 0           unsigned: 0xFFFFFFF8 > 3
    sltu s8, t1, t0           # 1
    add  zero, t1, t1         # x0 must stay 0

    ecall
