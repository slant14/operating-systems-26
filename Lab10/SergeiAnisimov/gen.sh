#!/usr/bin/env bash
# Lab 10 Sergei Anisimov CSE-04
for ((i = 0; i < $1; ++i)); do printf '%s\n' "$RANDOM" >> "$2"; done
