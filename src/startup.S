# This will set up our new segment registers. We need to do
# something special in order to set CS. We do what is called a
# far jump. A jump that includes a segment as well as an offset.
# This is declared in C as 'extern void gdt_flush();'

#global _gdt_flush     ; Allows the C code to link to this
#extern _gp            ; Says that '_gp' is in another file

#_gdt_flush:
#    lgdt [_gp]        ; Load the GDT with our '_gp' which is a special pointer
#    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
#    mov ds, ax
#    mov es, ax
#    mov fs, ax
#    mov gs, ax
#    mov ss, ax
#    jmp 0x08:flush2   ; 0x08 is the offset to our code segment: Far jump!
#flush2:
#    ret               ; Returns back to the C code!

#.section .text

.global gdt_flush
.global idt_load

.global isr0

.extern gp
.extern idtp
.extern fault_handler

gdt_flush:
	lgdt (gp)
	jmp flush2
flush2:
	ret

idt_load:
    lidt (idtp)
    ret

#############ISR Routines
#  0: Divide By Zero Exception
isr0:
    cli
    pushl 0
    pushl 0
    jmp isr_common_stub

isr_common_stub:
	pusha
	push %ds
    push %es
    push %fs
    push %gs

    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    mov %esp, %eax
    push %eax

    mov fault_handler, %eax
    call *%eax

    pop %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds
	popa
	add 8, %esp
	iret
