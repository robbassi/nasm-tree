;; Given a directory name, display its contents as a tree

extern tree
extern print_strs
extern display_contents

global _start
global puts
global malloc
global open
global dirents

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

	mov rdi, [rsp+8]
	call tree

	jmp exit

; rdi -> dir path
print_contents:
	push rax          ; save registers
	push rdi

	call open

	mov rdi, rax
	mov rsi, contents
	mov rdx, buff_size
	call dirents

	mov rdi, contents  
	call display_contents

	pop rdi           ; restore registers
	pop rax

	ret

; rdi -> char*path
open:
	mov rax, 2        ; open the directory
	mov rsi, 0        ; flags
	mov rdx, 0        ; read only
	syscall
	ret
	
; rdi -> fd
; rsi -> struct linux_dirent*buff
; rdx -> buff length
dirents:
	mov rdi, rax      ; result fd
	mov rax, 78
	syscall
	ret

; rdi -> char*buff
; rsi -> length
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

; rdi -> length
malloc:
	
	push rdi     ; save the length

	xor rbx, rbx ; determine end of the bss segment
	mov rdi, rbx ; push the addr in rdi (arg 0)
	mov rax, 12  ; brk
	syscall

	pop rdi      ; restore the length
	push rdi

	add rax, rdi ; now add the length to the end of the bss segment
	mov rbx, rax ; update rbx
	mov rdi, rax ; push new offset in rdi
	mov rax, 12  ; brk
	syscall

	pop rdi      
	sub rax, rdi ; rewind the index by the allocation amount

	ret

usage:
	mov rdi, argc_error
	mov rsi, argc_error_len
	call puts
	call exit

exit:
	mov rax, 60
	mov rdi, 0
	syscall