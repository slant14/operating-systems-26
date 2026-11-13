#!/usr/bin/env bash
# Lab 10 Sergei Anisimov CSE-04
set -euo pipefail
src=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
scratch=$(mktemp -d)
watch="$scratch/work"
outside="$scratch/other"
mkdir -- "$watch" "$outside"
trap 'rm -f -- "$watch"/{ex1.txt,ex11.txt,ex12.txt,output.txt} "$outside/ex13.txt"; rmdir -- "$watch" "$outside" "$scratch"' EXIT
bash "$src/gen.sh" 10 "$watch/ex1.txt"
ln -- "$watch/ex1.txt" "$watch/ex11.txt"
ln -- "$watch/ex1.txt" "$watch/ex12.txt"
cmp -- "$watch/ex1.txt" "$watch/ex11.txt"
cmp -- "$watch/ex1.txt" "$watch/ex12.txt"
stat -c '%i %n' "$watch"/ex*.txt | tee "$watch/output.txt"
du -- "$watch/ex1.txt"
ln -- "$watch/ex1.txt" "$watch/ex13.txt"
mv -- "$watch/ex13.txt" "$outside/ex13.txt"
find "$watch" -samefile "$watch/ex1.txt"
find "$scratch" -samefile "$watch/ex1.txt"
stat -c 'Hard links: %h' "$watch/ex1.txt"
# The three names are the same inode and share the same data blocks.
# The wider search includes the fourth name outside the working directory.
# All four names are deleted by the cleanup trap.
