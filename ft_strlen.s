; ft_strlen - 文字列の長さを計算する
; 引数: rdi = 文字列のポインタ
; 戻り値: rax = 文字列の長さ

section .text
	global ft_strlen

ft_strlen:
	xor rax, rax				; カウンタを0に初期化
	test rdi, rdi				; NULLチェック
	jz .done					; NULLなら0を返す
.loop:
	cmp byte [rdi + rax], 0		; 文字がヌル文字か確認
	je .done					; ヌル文字なら終了
	inc rax						; カウンタをインクリメント
	jmp .loop
.done:
	ret

section .note.GNU-stack noalloc noexec nowrite progbits
