#!/usr/bin/env bash
# Lab 10 Sergei Anisimov CSE-04
set -euo pipefail
cd -- "${1:?usage: ex1_test.sh directory}"
mkdir test-dir
printf 'created\n' > test-file
cat test-file
chmod u+x test-file
printf 'modified\n' >> test-file
ln -s test-file test-link
rm -- test-link test-file
rmdir -- test-dir
