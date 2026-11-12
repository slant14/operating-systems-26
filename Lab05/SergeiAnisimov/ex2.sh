#!/usr/bin/env bash
# Lab 05 Sergei Anisimov CSE-04
set -euo pipefail

src=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
binary=$(mktemp)
trap 'rm -f -- "$binary"' EXIT
gcc -std=c11 -Wall -Wextra -Wpedantic -pthread "$src/ex2.c" -o "$binary"
"$binary" "$@"
