#!/usr/bin/env bash
# Lab 10 Sergei Anisimov CSE-04
set -euo pipefail
scratch=$(mktemp -d)
trap 'rm -f -- "$scratch/ex5.txt"; rmdir -- "$scratch"' EXIT
file="$scratch/ex5.txt"
touch -- "$file"
chmod a-w -- "$file"
stat -c '%a %A' "$file"
chmod u=rwx,g=,o=rwx -- "$file"
stat -c '%a %A' "$file"
chmod g=u -- "$file"
stat -c '%a %A' "$file"
for mode in 660 775 777; do
    chmod "$mode" -- "$file"
    stat -c '%a %A' "$file"
done
# 660: owner and group can read/write; others have no access.
# 775: owner and group have rwx; others have read/execute.
# 777: every class has read, write and execute permissions.
