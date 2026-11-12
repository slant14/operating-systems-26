#!/usr/bin/env bash
# Lab 01 Sergei Anisimov CSE-04
set -euo pipefail

week_dir=${1:-"$HOME/week01"}
mkdir -p -- "$week_dir"

mkdir -p -- "$week_dir/dir1"
date
sleep 3
LC_ALL=C ls -ltr -- / > "$week_dir/dir1/root.txt"

date
sleep 3
mkdir -p -- "$week_dir/dir2"
date
sleep 3
LC_ALL=C ls -ltr -- "$HOME" > "$week_dir/dir2/home.txt"

printf '\nContents of dir1/root.txt\n'
cat -- "$week_dir/dir1/root.txt"
printf '\nContents of dir2/home.txt\n'
cat -- "$week_dir/dir2/home.txt"
printf '\nEntries in dir1\n'
ls -- "$week_dir/dir1"
printf '\nEntries in dir2\n'
ls -- "$week_dir/dir2"
