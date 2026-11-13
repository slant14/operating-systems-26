#!/usr/bin/env bash
# Lab 05 Sergei Anisimov CSE-04
set -euo pipefail
src=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
binary=$(mktemp)
trap 'rm -f -- "$binary" "$binary.time"' EXIT
gcc -std=c11 -O2 -Wall -Wextra -Wpedantic -Werror -pthread "$src/ex4.c" -o "$binary"
TIMEFORMAT='%3R'
printf 'threads primes wall_seconds\n'
for threads in 1 2 4 10 100; do
    { time primes=$("$binary" 10000000 "$threads"); } 2> "$binary.time"
    read -r elapsed < "$binary.time"
    rm -f -- "$binary.time"
    printf '%s %s %s\n' "$threads" "$primes" "$elapsed"
done
