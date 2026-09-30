#!/bin/bash

cd ~/week01

date
sleep 3
mkdir root_folder

date
sleep 3
ls -tr / > root_folder/root.txt

date
sleep 3
mkdir home_folder

date
sleep 3
ls -tr ~ > home_folder/home.txt

cat root_folder/root.txt
cat home_folder/home.txt
ls root_folder
ls home_folder
