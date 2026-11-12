#!/bin/bash
cd ~/week01
mkdir root
date
sleep 3
mkdir home
date
sleep 3
touch root/root.txt
date
sleep 3
touch home/home.txt
date
sleep 3
ls -t -r / > root/root.txt
ls -t -r ~ > home/home.txt
cat root/root.txt
cat home/home.txt
ls home
ls root
