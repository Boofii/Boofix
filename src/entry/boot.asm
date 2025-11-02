section .multiboot
align 4
dd 0x1BADB002
dd 0x00000003
dd -(0x1BADB002 + 0x00000003)

global start
extern long_mode_start

section .text
bits 32

start:
	mov esp, stack_top

	call setup_page_tables
	call enable_paging

	lgdt [gdt64.pointer]
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