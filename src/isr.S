.section .text

.globl   isr_wrapper, idt_load, gdt_flush

isr_wrapper:
    pushal
    cld
    call isr_handler
    popal
    iret

idt_load:
    lidt (idtp)
    ret

gdt_flush:
    lgdt (gp)        # Load the GDT with our '_gp' which is a special pointer
    mov $0x10, %ax      # 0x10 is the offset in the GDT to our data segment
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    #jmp $0x08:flush2   # 0x08 is the offset to our code segment: Far jump!
    ljmp $0x08, $flush2
flush2:
    ret               # Returns back to the C code!
	