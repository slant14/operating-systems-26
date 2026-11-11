#!/bin/bash

pid=$(cat /tmp/ex1.pid)
line=$(grep "/dev/zero" /proc/$pid/maps)

range=$(echo "$line" | awk '{print $1}') # get the 1st column of the line
start=${range%-*}                        # all before defis
end=${range#*-}                          # all after defis

sudo xxd -s 0x$start -l $((0x$end - 0x$start)) /proc/$pid/mem | grep "pass:"
kill -9 $pid

# ./ex1_hack.sh
# 7f25af6f6000: 7061 7373 3a32 6e49 0000 0000 0000 0000  pass:2nI........   