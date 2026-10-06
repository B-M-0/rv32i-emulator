# Branch test: every branch, taken and not taken, signed vs unsigned,
# plus a backward branch (loop).
# Expected results are in the comments and checked by test.cpp.
# Assemble:
#   riscv64-unknown-elf-as -march=rv32i -mabi=ilp32 branch.s -o branch.o
#   riscv64-unknown-elf-objcopy -O binary branch.o branch.bin
# Pattern for each case: set the result to 1, then branch over the line
# that sets it to 0. So result 1 = taken, 0 = not taken.

.option norelax              # stop the linker rewriting instructions, so a linked ELF (for Spike) matches the .bin
.globl _start
_start:
    li    t0, -1              # 0xFFFFFFFF: smallest signed, largest unsigned
    li    t1, 1

    li    a0, 1
    beq   t1, t1, 1f          # taken      (1 == 1)
    li    a0, 0
1:  li    a1, 1
    beq   t1, t0, 1f          # not taken  (1 != -1)
    li    a1, 0
1:  li    a2, 1
    bne   t1, t0, 1f          # taken
    li    a2, 0
1:  li    a3, 1
    bne   t1, t1, 1f          # not taken
    li    a3, 0
1:  li    a4, 1
    blt   t0, t1, 1f          # taken      signed: -1 < 1
    li    a4, 0
1:  li    a5, 1
    blt   t1, t0, 1f          # not taken  signed: 1 > -1
    li    a5, 0
1:  li    a6, 1
    bge   t1, t0, 1f          # taken      signed: 1 >= -1
    li    a6, 0
1:  li    a7, 1
    bge   t1, t1, 1f          # taken      equal counts as >=
    li    a7, 0
1:  li    s2, 1
    bge   t0, t1, 1f          # not taken  signed: -1 < 1
    li    s2, 0
1:  li    s3, 1
    bltu  t1, t0, 1f          # taken      unsigned: 1 < 0xFFFFFFFF
    li    s3, 0
1:  li    s4, 1
    bltu  t0, t1, 1f          # not taken  unsigned: 0xFFFFFFFF > 1
    li    s4, 0
1:  li    s5, 1
    bgeu  t0, t1, 1f          # taken      unsigned: 0xFFFFFFFF >= 1
    li    s5, 0
1:  li    s6, 1
    bgeu  t1, t0, 1f          # not taken  unsigned: 1 < 0xFFFFFFFF
    li    s6, 0
1:  li    s7, 1
    bgeu  t1, t1, 1f          # taken      equal counts as >=
    li    s7, 0

    # backward branch: loop 5 times
1:  li    t2, 5
    li    s8, 0
2:  addi  s8, s8, 1
    addi  t2, t2, -1
    bne   t2, zero, 2b        # s8 = 5 when the loop ends

    ecall
