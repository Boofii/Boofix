global enable_idt
global outb
global inb
extern idtr

global asm_handler32
global asm_handler33
extern c_handler

section .text
align 16

outb:
    mov al, dil
    out dx, al
    ret

inb:
    mov dx, di
    in al, dx
    movzx rax, al
    ret

asm_handler32:
    push 32
    jmp asm_handler

asm_handler33:
    push 33
    jmp asm_handler

enable_idt:
    mov al, 0x11
    out 0x20, al
    out 0xA0, al
    mov al, 0x20
    out 0x21, al
    mov al, 0x28
    out 0xA1, al
    mov al, 0x04
    out 0x21, al
    mov al, 0x02
    out 0xA1, al
    mov al, 0x01
    out 0x21, al
    out 0xA1, al

    mov al, 0x00
    out 0x21, al
    out 0xA1, al
    
    lidt [rdi]
    sti
    ret

asm_handler:
    pop rdi

    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    call c_handler

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax

    mov al, 0x20
    out 0x20, al
    
    iretq