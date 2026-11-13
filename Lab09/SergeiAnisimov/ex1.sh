#!/usr/bin/env bash
# Lab 09 Sergei Anisimov CSE-04
set -euo pipefail
src=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
for policy in random nfu aging; do
    printf '\n=== %s ===\n' "$policy"
    bash "$src/run_case.sh" "$policy" 5 3 plain R0 R1 R0 W1 R0 R1 R0 W1 R0 R2 R0 W2 R0 R2 R0 W2 R0 R3 R0 W3 R0 R3 R0 W3 R0 R4 R0 W4 R0 R4 R0 W4
done
