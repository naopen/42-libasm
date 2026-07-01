#!/bin/bash

set -e

echo "=== Building libasm ==="
make re

echo ""
echo "=== Compiling test program ==="
gcc -o test_libasm main.c -L. -lasm

echo ""
echo "=== Running tests ==="
./test_libasm
