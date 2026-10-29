#!/bin/bash
gcc publisher.c -o publisher
gcc subscriber.c -o subscriber
gnome-terminal -- ./publisher $1
sleep 1
for i in $(seq 1 $1); do
    gnome-terminal -- ./subscriber $i
done