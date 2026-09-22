#!/bin/bash

# Compile and execute $1.c with name $1.out

src="$1.c"
out="$1.out"

gcc "$src" -o "$out" ${@:2} 
./"$out"

