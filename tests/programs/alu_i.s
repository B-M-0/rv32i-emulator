# I-type ALU test: every I-type arithmetic instruction, with edge cases.
# Expected results are in the comments and checked by test.cpp.
# Assemble:
#   riscv64-unknown-elf-as -march=rv32i -mabi=ilp32 alu_i.s -o alu_i.o
#   riscv64-unknown-elf-objcopy -O binary alu_i.o alu_i.bin
# Key idea: the 12-bit immediate is sign-extended to 32 bits first,
# so -1 becomes 0xFFFFFFFF even for the "unsigned" and logical instructions.

.globl _start
_start:
    # inputs
    li    t0, -8              # 0xFFFFFFF8
    li    t1, 0x7FFFFFFF      # largest positive int
    li    t2, 5

    addi  a0, t2, -1          # 4           negative immediate
    addi  a1, t2, 2047        # 0x804       largest immediate
    addi  a2, zero, -2048     # 0xFFFFF800  smallest immediate, sign-extended
    addi  a3, t1, 1           # 0x80000000  overflow wraps around
    slti  a4, t0, -7          # 1           signed: -8 < -7
    slti  a5, t2, -1          # 0           signed: 5 > -1
    sltiu a6, t2, -1          # 1           -1 -> 0xFFFFFFFF, then unsigned 5 < 0xFFFFFFFF
    sltiu a7, zero, 1         # 1           x < 1 unsigned only when x == 0 (seqz)
    sltiu s2, t2, 5           # 0           equal is not less
    xori  s3, t2, -1          # 0xFFFFFFFA  xor with -1 flips every bit (not)
    ori   s4, t2, -2048       # 0xFFFFF805
    andi  s5, t0, 0x7F0       # 0x000007F0
    andi  s6, t1, -2048       # 0x7FFFF800
    slli  s7, t2, 31          # 0x80000000
    srli  s8, t0, 28          # 0x0000000F  zeros shifted in
    srai  s9, t0, 1           # 0xFFFFFFFC  sign bit shifted in: -8 >> 1 = -4
    srai  s10, t1, 30         # 1           positive value: same as srli

    ecall
