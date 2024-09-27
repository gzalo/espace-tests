.equ STACK_SIZE, 0x20000 @ 128 KB
.comm stack, STACK_SIZE, 4
_start:
    .globl _start

    @ (PSR_FIQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xD1
    msr cpsr_c,r0
    ldr sp, =stack+STACK_SIZE

    @ (PSR_IRQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xD2
    msr cpsr_c,r0
    ldr sp, =stack+STACK_SIZE

    @ (PSR_ABT_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xD7
    msr cpsr_c,r0
    ldr sp, =stack+STACK_SIZE

    @ (PSR_UND_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xDb
    msr cpsr_c,r0
    ldr sp, =stack+STACK_SIZE

    @ (PSR_SYS_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xDf
    msr cpsr_c,r0
    ldr sp, =stack+STACK_SIZE

    @ (PSR_SVC_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xD3
    msr cpsr_c,r0
    ldr sp, =stack+STACK_SIZE

    bl reset
    b .

.align 5 @ 32 byte
_vector_table:
    .globl _vector_table
    ldr pc,_start
    ldr pc,undefined_target
    ldr pc,swi_target
    ldr pc,prefetch_target
    ldr pc,abort_target
    ldr pc,unused_target
    ldr pc,irq_target
    ldr pc,fiq_target

undefined_target:       .word   undefined_handler
swi_target:             .word   swi_handler
prefetch_target:        .word   prefetch_handler
abort_target:           .word   data_handler
unused_target:          .word   unused_handler
irq_target:             .word   irq_handler
fiq_target:             .word   fiq_handler
