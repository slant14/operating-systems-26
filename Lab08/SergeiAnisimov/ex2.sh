#!/usr/bin/env bash
# Lab 08 Sergei Anisimov CSE-04
set -euo pipefail
src=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
if (( $# == 0 )); then set -- 4 2 R0 R1 W1 R0 R2 W2 R0 R3 W2; fi
exec bash "$src/run_case.sh" "$@"
