section .text
	global ft_strcmp

ft_strcmp:
.loop:
	movzx eax, byte [rdi]
	movzx ecx, byte [rsi]
	cmp al, cl
	jne .diff
	test al, al
	jz .equal
	inc rdi
	inc rsi
	jmp .loop
.diff:
	sub eax, ecx
	ret
.equal:
	xor eax, eax
	ret

section .note.GNU-stack noalloc noexec nowrite progbits
