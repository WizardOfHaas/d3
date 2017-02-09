.globl   isr_wrapper
.align   4

.section .text

isr_wrapper:
    pushal
    cld
    call isr_handler
    popal
    iret
    