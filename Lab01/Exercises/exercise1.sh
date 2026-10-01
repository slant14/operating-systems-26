#!/bash/bin

#The command ls gives us the content of the directory /usr/bin
#The command grep receives the input from the command ls and returns content from /usr/bin with "gcc"
#The command tail receives the input from the command grep and returns the last 5 items
#The command sort receives the input from the command tail, sorts the result in reversed order and writes it to the ex1.txt file
ls /usr/bin | grep "gcc" | tail -n 5 | sort -r > ~/week01/ex1.txt
