#!/usr/bin/env bash
# Lab 10 Sergei Anisimov CSE-04
set -euo pipefail
src=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
scratch=$(mktemp -d)
trap 'rm -f -- "$scratch/tmp1"; if [[ -d $scratch/tmp ]]; then rm -f -- "$scratch/tmp/tmp2" "$scratch/tmp/ex1.txt"; rmdir -- "$scratch/tmp"; fi; rmdir -- "$scratch"' EXIT
cd -- "$scratch"
ln -s -- "$scratch/tmp" tmp1
ls -li -- tmp1
test ! -e tmp1
mkdir tmp
ls -li -- tmp1 tmp
bash "$src/gen.sh" 10 tmp/ex1.txt
cmp -- tmp/ex1.txt tmp1/ex1.txt
ln -s -- "$scratch/tmp" tmp1/tmp2
bash "$src/gen.sh" 3 tmp1/tmp2/ex1.txt
cmp -- tmp/ex1.txt tmp1/tmp2/tmp2/tmp2/ex1.txt
wc -l tmp/ex1.txt
rm -- tmp/ex1.txt tmp/tmp2
rmdir tmp
ls -li -- tmp1
test ! -e tmp1
# The first symlink starts and ends dangling. While tmp exists, tmp2
# resolves back to tmp and the repeated path reaches the same file.
