# Every RV32I instruction once, plus edge cases.
# Assemble: riscv64-unknown-elf-as -march=rv32i_zifencei -mabi=ilp32 rv32i_all.s -o rv32i_all.o
# Vectors:  riscv64-unknown-elf-objdump -d -M no-aliases rv32i_all.o

    .option norelax          # resolve branch/jump offsets now, not at link time
    .text
    .globl _start
_start:
back:
    # U-type
    lui     a0, 0x12345
    lui     a0, 0xfffff          # top bit set
    auipc   a1, 0x1

    # J / I jumps
    jal     ra, fwd
    jal     zero, back           # backward (negative J imm)
    jalr    ra, 4(a0)
    jalr    zero, -4(a0)         # negative I imm

    # B-type
    beq     a0, a1, fwd
    bne     a0, a1, back         # backward
    blt     a0, a1, fwd
    bge     a0, a1, back
    bltu    a0, a1, fwd
    bgeu    a0, a1, back

    # LOAD
    lb      a2, 0(sp)
    lh      a2, -2(sp)
    lw      a2, 8(sp)
    lbu     a2, 1(sp)
    lhu     a2, 2(sp)

    # STORE
    sb      a2, 0(sp)
    sh      a2, -2(sp)           # negative S imm
    sw      a2, 2047(sp)         # max positive S imm

    # OP-IMM
    addi    a0, a0, 5
    addi    a0, a0, -1           # all-ones top bits, must NOT hit funct7 check
    addi    a0, a0, -2048        # min I imm
    slti    a0, a1, -7
    sltiu   a0, a1, 7
    xori    a0, a1, -1
    ori     a0, a1, 0x7ff
    andi    a0, a1, 0xff
    slli    a0, a1, 31           # max shamt
    srli    a0, a1, 1
    srai    a0, a1, 31

    # OP
    add     a0, a1, a2
    sub     a0, a1, a2
    sll     a0, a1, a2
    slt     a0, a1, a2
    sltu    a0, a1, a2
    xor     a0, a1, a2
    srl     a0, a1, a2
    sra     a0, a1, a2
    or      a0, a1, a2
    and     a0, a1, a2

    # MISC-MEM / SYSTEM
    fence
    ecall
    ebreak

fwd:
    addi    zero, zero, 0        # prints as nop without -M no-aliases
