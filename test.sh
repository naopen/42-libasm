#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m'

pass=0
fail=0

ok() {
	echo -e "  ${GREEN}[OK]${NC} $1"
	((pass++))
}

ko() {
	echo -e "  ${RED}[KO]${NC} $1"
	((fail++))
}

echo "========================================"
echo " libasm 評価シート準拠テスト"
echo "========================================"

# ----------------------------------------
# 1. ライブラリの構成チェック
# ----------------------------------------
echo ""
echo "=== ライブラリ構成チェック ==="

# .sファイルのみで構成されているか
s_count=$(ls *.s 2>/dev/null | wc -l)
if [ "$s_count" -ge 6 ]; then
	ok "ライブラリは.sファイルで構成されている ($s_count files)"
else
	ko "ライブラリの.sファイルが不足している ($s_count files)"
fi

# ----------------------------------------
# 2. Makefileチェック
# ----------------------------------------
echo ""
echo "=== Makefile チェック ==="

make fclean > /dev/null 2>&1
make > /dev/null 2>&1
if [ -f "libasm.a" ]; then
	ok "make: libasm.aが生成された"
else
	ko "make: libasm.aが生成されなかった"
fi

# 再リンクしないことを確認
make_output=$(make 2>&1)
if echo "$make_output" | grep -q "Nothing to be done\|is up to date"; then
	ok "make再実行: 不要な再リンクなし"
else
	ko "make再実行: 不要な再リンクが発生した"
fi

# clean
make clean > /dev/null 2>&1
if [ ! -f "ft_strlen.o" ]; then
	ok "make clean: .oファイルが削除された"
else
	ko "make clean: .oファイルが残っている"
fi
if [ -f "libasm.a" ]; then
	ok "make clean: libasm.aは残っている"
else
	ko "make clean: libasm.aが削除された"
fi

# fclean
make > /dev/null 2>&1
make fclean > /dev/null 2>&1
if [ ! -f "libasm.a" ]; then
	ok "make fclean: libasm.aが削除された"
else
	ko "make fclean: libasm.aが残っている"
fi

# re
make re > /dev/null 2>&1
if [ -f "libasm.a" ]; then
	ok "make re: libasm.aが再生成された"
else
	ko "make re: libasm.aが生成されなかった"
fi

# ----------------------------------------
# 3. テストプログラムのビルドと実行
# ----------------------------------------
echo ""
echo "=== テストプログラム ビルド ==="

gcc -o test_libasm main.c -L. -lasm 2>&1
if [ $? -eq 0 ]; then
	ok "gcc: main.cのコンパイル成功"
else
	ko "gcc: main.cのコンパイル失敗"
	echo -e "${RED}ビルド失敗のためテスト中断${NC}"
	exit 1
fi

echo ""
echo "=== 機能テスト ==="
./test_libasm
test_result=$?

if [ $test_result -eq 0 ]; then
	ok "全機能テストパス"
else
	ko "機能テストに失敗あり"
fi

# ----------------------------------------
# 4. セグフォルトチェック（評価シート: segfaultしたら即14点）
# ----------------------------------------
echo ""
echo "=== セグフォルトチェック ==="

if [ $test_result -ne 139 ] && [ $test_result -ne 134 ] && [ $test_result -ne 136 ]; then
	ok "テスト中にsegfault/abort/bus errorなし"
else
	ko "テスト中に異常終了が発生した (exit code: $test_result)"
fi

# ----------------------------------------
# 5. メモリリークチェック（valgrind）
# ----------------------------------------
echo ""
echo "=== メモリリークチェック ==="

if command -v valgrind > /dev/null 2>&1; then
	valgrind_output=$(valgrind --leak-check=full --error-exitcode=42 ./test_libasm 2>&1)

	heap_summary=$(echo "$valgrind_output" | grep "in use at exit:")
	if echo "$heap_summary" | grep -q "0 bytes in 0 blocks"; then
		ok "valgrind: メモリリークなし"
	else
		ko "valgrind: メモリリークあり"
		echo "$heap_summary"
	fi

	error_summary=$(echo "$valgrind_output" | grep "ERROR SUMMARY:")
	if echo "$error_summary" | grep -q "0 errors"; then
		ok "valgrind: メモリエラーなし"
	else
		ko "valgrind: メモリエラーあり"
		echo "$error_summary"
	fi
else
	echo -e "  ${YELLOW}[SKIP]${NC} valgrindが未インストール (sudo apt install valgrind)"
fi

# ----------------------------------------
# 結果サマリー
# ----------------------------------------
echo ""
echo "========================================"
total=$((pass + fail))
if [ $fail -eq 0 ]; then
	echo -e "${GREEN}ALL PASSED: $pass/$total${NC}"
else
	echo -e "${RED}FAILED: $fail/$total テストが失敗${NC}"
fi
echo "========================================"
echo ""

rm -f test_libasm
exit $fail
