.comm stack, 0x8000 @ 32 KB
_start:
    .globl _start
    ldr sp, =stack+0x8000
    bl main
    b .
