#!/usr/bin/env bash
# Lab 02 Sergei Anisimov CSE-04
set -euo pipefail

src=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
binary=$(mktemp)
trap 'rm -f -- "$binary"' EXIT
gcc -std=c11 -Wall -Wextra -Wpedantic "$src/ex5.c" -o "$binary"
"$binary" "$@"
