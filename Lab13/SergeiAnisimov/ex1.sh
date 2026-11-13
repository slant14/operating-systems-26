#!/usr/bin/env bash
# Lab 13 Sergei Anisimov CSE-04
set -euo pipefail
src=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
binary=$(mktemp)
trap 'rm -f -- "$binary"' EXIT
gcc -std=c11 -O2 -Wall -Wextra -Wpedantic -Werror -pthread "$src/ex1.c" -o "$binary"
"$binary" "${1:-3}" "${2:-4}" "${3:-$src/input.txt}"
