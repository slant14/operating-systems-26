#!/bin/bash

mkdir -p ~/week01
ls /usr/bin | grep gcc | sort -r | tail -n 5 > ~/week01/ex1.txt
