#!/bin/bash
pid=$(cat /tmp/ex1.pid) # pid of ex1
range=$(grep "rw-s" /proc/$pid/maps | cut -d' ' -f1) # address range of the shared memory
start=$(echo $range | cut -d'-' -f1) # the part before "-"
password=$(xxd -s $((0x$start)) -l 13 -p /proc/$pid/mem | xxd -r -p) # read 13 bytes ("pass:" + 8 chars) from the address
echo "Password: $password"
echo "Address: 0x$start"
kill -SIGKILL $pid