#!/bash/bin

#The command history is used to see the history of commands
history > ~/week01/ex2.txt

#Here are some commands that I got after application of history command
sort -n -r namelist.txt > namelist1.txt
cat namelist.txt
nano namelist.txt
sudo apt update && sudo apt install ubuntu-desktop -y
sudo reboot
whoami
hostname
man whoami
man man
man sort
man --help
clear
less namelist.txt
head namelist.txt
tail namelist.txt
grep "Marge" namelist.txt
ls > namelist.txt
head -n 3 < namelist.txt
man grep
grep -r "task" /proc > output.txt
grep -r "task" /proc > output.txt | sort -n | head -n 3
grep -r "task" /proc | sort -n | head -n 3 > output.txt
cat output.txt
grep -r "task" /proc 2> errors.txt
cat errors.txt
grep -r "task" 2> errors.txt | sort -n | head -n 3 > output.txt
cat output.txt
cat errors.txt
pwd
mkdir labs
ls
cd ..
rm -r labs
rm namelist1.txt
mkdir labs
ls
mv namelist.txt labs
ls
history
