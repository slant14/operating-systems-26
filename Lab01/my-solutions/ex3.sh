#!/bin/bash

date
sleep 3
mkdir dir1

date
sleep 3
ls -ltr / > dir1/root.txt

date
sleep 3
mkdir dir2

date
sleep 3
ls -ltr ~ > dir2/home.txt

echo "contents of dir1/root.txt:"
cat dir1/root.txt

echo "content of dir2/home.txt"
cat dir2/home.txt

echo "items in dir1:"
ls -la dir1/

echo "items in dir2:"
ls -la dir2/
