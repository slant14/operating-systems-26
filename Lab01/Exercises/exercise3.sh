#!/bash/bin

#Creation of the first folder
mkdir folder1
date
sleep 3
#Creation of the second folder
mkdir folder2
date
sleep 3
cd folder1
#Creation of root.txt file inside the folder1
touch root.txt
date
sleep 3
cd ../folder2
#Creation of home.txt file inside the folder2
touch home.txt
date
sleep 3
cd ../folder1
#The command ls / gives the content of the root directory, using -t and -r options I sort it by time (oldest first) and write the result into root.txt file
ls / -t -r > root.txt
#The content of root.txt file
cat root.txt
cd ../folder2
#The command ls ~ gives the content of the home directory, using -t and -r options I sort it by time (oldest first) as in previous case and write the result into home.txt file
ls ~ -t -r > home.txt
#The content of home.txt file
cat home.txt
cd ..
#Content of folder1 and folder2
ls folder1
ls folder2
