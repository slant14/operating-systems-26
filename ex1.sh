#!/bin/bash

mkdir -p /tmp/ex2
gcc pager.c -o pager
gcc mmu.c -o mmu

echo "Test 1 (random algorithm)"
./pager 4 2 random &
sleep 0.2
./mmu 4 R0 R1 W1 R0 R2 W2 R0 R3 W2 $!

echo "Test 2 (aging algorithm)"
./pager 4 2 aging &
sleep 0.2
./mmu 4 R0 R1 W1 R0 R2 W2 R0 R3 W2 $!

echo "Test 3 (not frequently used algorithm)"
./pager 4 2 nfu &
sleep 0.2
./mmu 4 R0 R1 W1 R0 R2 W2 R0 R3 W2 $!

echo "Test 4 (random algorithm)"
./pager 5 3 random &
sleep 0.2
./mmu 5 R0 R1 R0 W1 R0 R1 R0 W1 R0 R2 R0 W2 R0 R2 R0 W2 R0 R3 R0 W3 R0 R3 R0 W3 R0 R4 R0 W4 R0 R4 R0 W4 $!

echo "Test 5 (not frequently used algorithm)"
./pager 5 3 nfu &
sleep 0.2
./mmu 5 R0 R1 R0 W1 R0 R1 R0 W1 R0 R2 R0 W2 R0 R2 R0 W2 R0 R3 R0 W3 R0 R3 R0 W3 R0 R4 R0 W4 R0 R4 R0 W4 $!

echo "Test 6 (aging algorithm)"
./pager 5 3 aging &
sleep 0.2
./mmu 5 R0 R1 R0 W1 R0 R1 R0 W1 R0 R2 R0 W2 R0 R2 R0 W2 R0 R3 R0 W3 R0 R3 R0 W3 R0 R4 R0 W4 R0 R4 R0 W4 $!
