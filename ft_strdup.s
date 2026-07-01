section .text
	global ft_strdup
	extern malloc

ft_strdup:
	push rbx
	mov rbx, rdi
	xor rax, rax
.strlen_loop:
	cmp byte [rdi + rax], 0
	je .strlen_done
	inc rax
	jmp .strlen_loop
.strlen_done:
	lea rdi, [rax + 1]
	call malloc wrt ..plt
	test rax, rax
	jz .done
	xor rcx, rcx
.copy_loop:
	mov dl, [rbx + rcx]
	mov [rax + rcx], dl
	test dl, dl
	jz .done
	inc rcx
	jmp .copy_loop
.done:
	pop rbx
	ret

section .note.GNU-stack noalloc noexec nowrite progbits
