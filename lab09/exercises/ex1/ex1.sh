#!/bin/bash

gcc -std=c11 -D_POSIX_C_SOURCE=200809L pager.c -o pager || exit 1
gcc -std=c11 -D_POSIX_C_SOURCE=200809L mmu.c -o mmu || exit 1

for algo in random nfu aging
do
    echo "algorithm: $algo"

    ./pager "$algo" 4 2 &
    pid=$!

    sleep 1

    ./mmu 4 R0 R1 W1 R0 R2 W2 R0 R3 W2 "$pid"

    wait "$pid"
done
