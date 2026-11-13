#!/usr/bin/env bash
# Lab 06 Sergei Anisimov CSE-04
set -euo pipefail
src=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
data=$(realpath -- "${1:-$src/data.txt}")
build=$(mktemp -d)
trap 'rm -f -- "$build/worker" "$build/scheduler"; rmdir -- "$build"' EXIT
gcc -std=c11 -O2 -Wall -Wextra -Wpedantic -Werror -DTRI_BASE="${TRI_BASE:-1000000}" "$src/worker.c" -o "$build/worker"
gcc -std=c11 -O2 -Wall -Wextra -Wpedantic -Werror "$src/scheduler_rr.c" -o "$build/scheduler"
cd -- "$build"
./scheduler "$data"
