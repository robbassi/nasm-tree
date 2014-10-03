;; Given a directory name, display its contents as a tree

extern print_strs

global _start
global puts

section .text

_start:
	pop rdi           ; number of args (int argc)
	mov rsi, rsp      ; array of args  (char**argv)
        call print_strs
	jmp exit

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