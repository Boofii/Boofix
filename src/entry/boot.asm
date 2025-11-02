section .multiboot
header_start:
	dd 0xE85250D6
	dd 0x00000000
	dd header_end - header_start
	dd 0x100000000 - (0xE85250D6 + 0x00000000 + (header_end - header_start))

	dw 5          ; type = framebuffer header tag
	dw 0          ; flags
	dd 20         ; size of this tag (bytes)
	dd 1024       ; preferred width (0 = any)
	dd 768        ; preferred height (0 = any)
	dd 32         ; preferred depth (bits per pixel)
header_end:

global start
extern long_mode_start

section .data
global multiboot_info
multiboot_info: dq 0

section .text
bits 32

start:
	mov esp, stack_top

	call setup_page_tables
	call enable_paging

	lgdt [gdt64.pointer]
	mov [multiboot_info], ebx
	jmp gdt64.code_segment:long_mode_start

	hlt

setup_page_tables:
	mov eax, page_table_l3
	or eax, 0b11
	mov [page_table_l4], eax
	
	mov eax, page_table_l2
	or eax, 0b11
	mov [page_table_l3], eax

	mov ecx, 0
.loop:

	mov eax, 0x200000
	mul ecx
	or eax, 0b10000011
	mov [page_table_l2 + ecx * 8], eax

	inc ecx
	cmp ecx, 512
	jne .loop

	ret

enable_paging:
	mov eax, page_table_l4
	mov cr3, eax

	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

	mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr

	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax

	ret

section .bss
align 4096
page_table_l4:
	resb 4096
page_table_l3:
	resb 4096
page_table_l2:
	resb 4096
stack_bottom:
	resb 4096 * 4
stack_top:

section .rodata
gdt64:
	dq 0
.code_segment: equ $ - gdt64
	dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53)
.pointer:
	dw $ - gdt64 - 1
	dq gdt64