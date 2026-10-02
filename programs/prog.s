# Minimal smoke test. Expected on halt: a0 (x10) = 5, a1 (x11) = 8.
.globl _start
_start:
    addi a0, x0, 5
    addi a1, a0, 3
    ecall
