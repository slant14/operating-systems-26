#!/usr/bin/env bash
# Lab 01 Sergei Anisimov CSE-04
set -euo pipefail

week_dir=${1:-"$HOME/week01"}
mkdir -p -- "$week_dir"

# Apply the five-entry limit after sorting and filtering.
LC_ALL=C ls -1r -- /usr/bin | grep -F gcc | tail -n 5 > "$week_dir/ex1.txt"
