; ft_write - ファイルにデータを書き込む（writeシステムコールのラッパー）
; 引数: rdi = ファイルディスクリプタ, rsi = バッファ, rdx = バイト数
; 戻り値: rax = 書き込んだバイト数（エラー時は-1、errnoを設定）

section .text
	global ft_write
	extern __errno_location

ft_write:
	mov rax, 1					; writeのsyscall番号（Linux: 1）
	syscall						; カーネルにシステムコールを発行
	cmp rax, 0					; 戻り値が負ならエラー
	jl .error
	ret
.error:
	neg rax						; 負のエラーコードを正の値に変換
	push rax					; エラーコードをスタックに保存（兼16バイトアライン）
	call __errno_location wrt ..plt	; errnoのアドレスを取得
	pop rdx						; スタックからエラーコードを復元
	mov [rax], edx				; errnoにエラーコードを設定（intなので32ビット）
	mov rax, -1					; 戻り値を-1に設定
	ret

section .note.GNU-stack noalloc noexec nowrite progbits
