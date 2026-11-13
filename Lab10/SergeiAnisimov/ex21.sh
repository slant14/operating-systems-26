#!/usr/bin/env bash
# Lab 10 Sergei Anisimov CSE-04
set -euo pipefail
scratch=$(mktemp -d)
trap 'rm -f -- "$scratch"/{ex1.c,ex1}; rmdir -- "$scratch"' EXIT
for directory in /dev /etc; do
    printf '\nFile types in %s\n' "$directory"
    find "$directory" -mindepth 1 -maxdepth 1 -printf '%y\n' | sort | uniq -c
done
printf 'Directories in /etc: '
find /etc -mindepth 1 -maxdepth 1 -type d -printf '.\n' | wc -l
printf '#include <stdio.h>\nint main(void) { puts("Hello world!"); }\n' > "$scratch/ex1.c"
file "$scratch/ex1.c"
gcc "$scratch/ex1.c" -o "$scratch/ex1"
file "$scratch/ex1"
printf '#include <stdio.h>\nint main(void) { puts("Привет, мир!"); }\n' > "$scratch/ex1.c"
file "$scratch/ex1.c"
gcc "$scratch/ex1.c" -o "$scratch/ex1"
file "$scratch/ex1"
# The source text now contains UTF-8. Both compiled files are ELF executables.
