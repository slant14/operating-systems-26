#!/bin/bash

grep -r -l --include='*task*' '' /proc > output.txt 2> errors.txt
echo \`grep\` command was applied to the /proc directory. All names containing \'name\' were saved to the \`output.txt\` file. All errors were saved to the \`errors.txt\` file.
echo First ten lines of \`output.txt\`:
head output.txt -n 10
echo First ten lines of \`errors.txt\`:
head errors.txt -n 10
