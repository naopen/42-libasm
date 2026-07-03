; ft_strcmp - 2つの文字列を比較する
; 引数: rdi = 第1文字列, rsi = 第2文字列
; 戻り値: eax = 差分（s1[i] - s2[i]）

section .text
	global ft_strcmp

ft_strcmp:
	test rdi, rdi				; 第1文字列のNULLチェック
	jz .s1_null
	test rsi, rsi				; 第2文字列のNULLチェック
	jz .s2_null
.loop:
	movzx eax, byte [rdi]		; s1から1バイト読み込み（ゼロ拡張）
	movzx ecx, byte [rsi]		; s2から1バイト読み込み（ゼロ拡張）
	cmp al, cl					; 2つのバイトを比較
	jne .diff					; 異なれば差分を計算
	test al, al					; ヌル文字に到達したか確認
	jz .equal					; 到達していれば等しい
	inc rdi						; 次の文字へ
	inc rsi
	jmp .loop
.diff:
	sub eax, ecx				; 差分を計算（s1[i] - s2[i]）
	ret
.equal:
	xor eax, eax				; 等しい場合は0を返す
	ret
.s1_null:
	test rsi, rsi				; s1がNULL: s2もNULLなら等しい
	jz .equal
	mov eax, -1					; s1がNULL, s2が非NULL: s1 < s2
	ret
.s2_null:
	mov eax, 1					; s1が非NULL, s2がNULL: s1 > s2
	ret

section .note.GNU-stack noalloc noexec nowrite progbits
