#!/usr/bin/env bash
# Lab 10 Sergei Anisimov CSE-04
set -euo pipefail
scratch=$(mktemp -d)
trap 'rm -f -- "$scratch"/{ex1.c,ex1,ex2}; rmdir -- "$scratch"' EXIT
printf '#include <stdio.h>\nint main(void) { puts("Hello world!"); }\n' > "$scratch/ex1.c"
gcc "$scratch/ex1.c" -o "$scratch/ex1"
cp -- "$scratch/ex1" "$scratch/ex2"
stat -c 'inode=%i blocks=%b block_unit=%B bytes=%s permissions=%A links=%h path=%n' \
    "$scratch/ex1" "$scratch/ex2"
stat -c '%h - %n' /etc/* | awk '$1 == 3'
# cp creates a separate inode with one hard link. A directory's reported
# count of three conventionally includes its parent entry, . and one child's ..
