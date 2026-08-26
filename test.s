# I-type
addi x1, x2, 0
addi x1, x2, 1
addi x1, x2, -1
addi x1, x2, 2047
addi x1, x2, -2048
lw   x1, -8(x2)

# S-type
sw x5, 0(x8)
sw x5, 4(x8)
sw x5, -4(x8)
sw x5, 2047(x8)
sw x5, -2048(x8)

# J-type
jal x1, fwd
fwd:
jal x1, back
back:
jal x1, back

# U-type
lui   x1, 0
lui   x1, 1
lui   x1, 0x12345
lui   x1, 0xFFFFF
auipc x1, 0x1000

# B-type
top:
beq  x1, x2, top
bne  x1, x2, near
near:
blt  x1, x2, far
nop
nop
nop
nop
nop
nop
nop
nop
far:
bge  x1, x2, top
bltu x1, x2, far
bgeu x1, x2, top