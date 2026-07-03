; ft_strdup - 文字列を複製する
; 引数: rdi = 複製する文字列のポインタ
; 戻り値: rax = 新しく確保された複製文字列のポインタ（失敗時はNULL）
; ft_strlenとft_strcpyを再利用して実装

section .text
	global ft_strdup
	extern malloc
	extern ft_strlen
	extern ft_strcpy

ft_strdup:
	test rdi, rdi				; NULLチェック
	jz .null_return
	push rbx					; rbxはcallee-savedなので保存
	mov rbx, rdi				; 元の文字列ポインタを保存

	call ft_strlen wrt ..plt	; 文字列の長さを取得 → rax
	lea rdi, [rax + 1]			; malloc(strlen + 1)：ヌル文字分を加算
	call malloc wrt ..plt		; メモリを確保 → rax
	test rax, rax				; malloc失敗チェック
	jz .malloc_fail

	mov rdi, rax				; コピー先 = malloc済みメモリ
	mov rsi, rbx				; コピー元 = 元の文字列
	push rax					; mallocの戻り値を保存
	call ft_strcpy wrt ..plt	; 文字列をコピー
	pop rax						; mallocの戻り値を復元

	pop rbx						; rbxを復元
	ret
.malloc_fail:
	pop rbx						; rbxを復元
	ret							; rax = NULLのまま返す
.null_return:
	xor rax, rax				; NULLを返す
	ret

section .note.GNU-stack noalloc noexec nowrite progbits
