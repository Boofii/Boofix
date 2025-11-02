global long_mode_start
extern kmain
extern multiboot_info

section .text
bits 64
long_mode_start:
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov rdi, [multiboot_info]
    call kmain