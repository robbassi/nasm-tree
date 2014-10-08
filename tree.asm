;; Given a directory name, display its contents as a tree

extern print_strs
extern display_contents

global _start
global puts

section .data
argc_error: db `Usage: tree <dir>\n`, 18
argc_error_len: equ 18
entry_size: equ 16
max_entries: equ 1000
buff_size: equ entry_size * max_entries

section .bss
contents: resb buff_size

section .text

_start:
	pop r8            ; argc
	cmp r8, 2         ; argc > 2
	jne usage         ; show usage and exit
	
	; dev - print args
	;mov rdi, r8
	;mov rsi, rsp
	;call print_strs

	mov rax, 2
	mov rdi, [rsp+8]
	mov rsi, 0
	mov rdx, 0
	syscall

	mov rdi, rax      ; result fd
	mov rax, 78
	mov rsi, contents
	mov rdx, buff_size
	syscall

	mov rdi, contents
	call display_contents

	jmp exit

usage:
	mov rdi, argc_error
	mov rsi, argc_error_len
	call puts
	call exit

puts:
	push rax          ; save registers
	push rdi          

	mov rax, 1        ; write
	mov rdx, rsi      ; length
	mov rsi, rdi      ; char buffer
	mov rdi, 1        ; stdin
	syscall

	pop rdi           ; restore registers
	pop rax

	ret

exit:
	mov rax, 60
	mov rdi, 0
	syscall