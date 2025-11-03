section .multiboot
header_start:
	dd 0xE85250D6
	dd 0x00000000
	dd header_end - header_start
	dd 0x100000000 - (0xE85250D6 + 0x00000000 + (header_end - header_start))

	dw 5
	dw 0
	dd 20
	dd 1024
	dd 768
	dd 32
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

	mov [multiboot_info], ebx
	call setup_page_tables
	call enable_paging

	lgdt [gdt64.pointer]
	jmp gdt64.code_segment:long_mode_start

	hlt

setup_page_tables:
    mov eax, page_table_l3
    or eax, 0b11
    mov [page_table_l4], eax

    mov ecx, 0
.link_l3:
    mov eax, page_table_l2_0
    mov ebx, ecx
    shl ebx, 12
    add eax, ebx
    or eax, 0b11
    mov [page_table_l3 + ecx*8], eax
    inc ecx
    cmp ecx, 4
    jne .link_l3

    mov ebx, 0
.fill_tables:
    mov ecx, 0

    mov edx, page_table_l2_0
    mov esi, ebx
    shl esi, 12
    add edx, esi

    mov eax, 0

    mov edi, ebx
    shl edi, 30

    mov esi, edi
.fill_entries:
    mov eax, esi
    or  eax, 0x83
    mov [edx + ecx*8], eax

    add esi, 0x200000
    inc ecx
    cmp ecx, 512
    jne .fill_entries

    inc ebx
    cmp ebx, 4
    jne .fill_tables

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
page_table_l2_0: resb 4096
page_table_l2_1: resb 4096
page_table_l2_2: resb 4096
page_table_l2_3: resb 4096
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