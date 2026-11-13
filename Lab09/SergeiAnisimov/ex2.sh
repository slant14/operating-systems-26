#!/usr/bin/env bash
# Lab 09 Sergei Anisimov CSE-04
set -euo pipefail
src=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
trace=(R0 R1 R0 W0 R1 R2 R0 R1 W2 R2 R3 R0 R1 R2 R3)
for dimensions in '5 2' '10 2' '10 3'; do
    read -r pages frames <<< "$dimensions"
    printf '\n=== %s pages, %s frames ===\n' "$pages" "$frames"
    bash "$src/run_case.sh" aging "$pages" "$frames" tlb "${trace[@]}"
done
