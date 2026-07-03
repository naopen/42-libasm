; ft_strcpy - 文字列をコピーする
; 引数: rdi = コピー先, rsi = コピー元
; 戻り値: rax = コピー先のポインタ

section .text
	global ft_strcpy

ft_strcpy:
	mov rax, rdi				; 戻り値用にコピー先を保存
	test rdi, rdi				; コピー先のNULLチェック
	jz .done
	test rsi, rsi				; コピー元のNULLチェック
	jz .done
	xor rcx, rcx				; インデックスを0に初期化
.loop:
	mov dl, [rsi + rcx]			; コピー元から1バイト読み込み
	mov [rdi + rcx], dl			; コピー先に1バイト書き込み
	test dl, dl					; ヌル文字か確認
	jz .done
	inc rcx						; インデックスをインクリメント
	jmp .loop
.done:
	ret

section .note.GNU-stack noalloc noexec nowrite progbits
