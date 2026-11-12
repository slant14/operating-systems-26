#!/usr/bin/env bash
# Lab 01 Sergei Anisimov CSE-04
set -euo pipefail

week_dir=${1:-"$HOME/week01"}
mkdir -p -- "$week_dir"
cd -- "$week_dir"

# Bash starts scripts with command history disabled.
set -o history
pwd
ls -ld .
whoami
uname -s
date
history > ex2.txt
