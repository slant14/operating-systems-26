#!/bash/bin

#nano allows to create a file and & adds this process to the background
nano notes &
#fg sends the process to the foreground
fg %1
#gedit provides a text editor to create a file and & adds this process to the background
gedit notes.txt &
#shows list of jobs
jobs
#the content of notes goes into notes.txt
cat notes > notes.txt
