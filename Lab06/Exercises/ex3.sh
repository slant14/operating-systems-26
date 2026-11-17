#!/bin/bash

gcc worker.c -o worker
gcc scheduler_sjf.c -o scheduler_sjf
./scheduler_sjf data.txt
