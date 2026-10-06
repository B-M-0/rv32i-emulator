# Load/store test: every load and store width, sign vs zero extension,
# negative offsets, and stores only changing the bytes they should.
# Expected results are in the comments and checked by test.cpp.
# Assemble:
#   riscv64-unknown-elf-as -march=rv32i -mabi=ilp32 load_store.s -o load_store.o
#   riscv64-unknown-elf-objcopy -O binary load_store.o load_store.bin
# Memory is little-endian: the word 0x8081FF7F is stored as bytes 7F FF 81 80.

.globl _start
_start:
    li    t0, 0x80001000      # data address: empty memory past the program
    li    t1, 0x8081FF7F
    sw    t1, 0(t0)

    lw    a0, 0(t0)           # 0x8081FF7F
    lb    a1, 0(t0)           # 0x0000007F  top bit 0: stays positive
    lb    a2, 1(t0)           # 0xFFFFFFFF  0xFF sign-extended
    lbu   a3, 1(t0)           # 0x000000FF  0xFF zero-extended
    lh    a4, 2(t0)           # 0xFFFF8081  sign-extended
    lhu   a5, 2(t0)           # 0x00008081  zero-extended
    lh    a6, 0(t0)           # 0xFFFFFF7F
    addi  t2, t0, 8
    lw    a7, -8(t2)          # 0x8081FF7F  negative offset

    # stores must only change their own bytes
    li    t3, -1
    sw    t3, 4(t0)           # word at +4 = 0xFFFFFFFF
    li    t4, 0x12345678
    sb    t4, 4(t0)           # low byte only (0x78):        0xFFFFFF78
    sh    t4, 6(t0)           # upper half only (0x5678):    0x5678FF78
    lw    s2, 4(t0)           # 0x5678FF78
    lw    s3, 8(t0)           # 0x00000000  next word untouched
    sb    zero, 3(t0)         # clear the top byte of the first word
    lw    s4, 0(t0)           # 0x0081FF7F

    ecall
