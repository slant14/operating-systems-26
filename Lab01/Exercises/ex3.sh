mkdir root
sleep 3
mkdir home
 
ls -art / > ./root/root.txt
echo -e '\n root.txt was created \n' 

cat ./root/root.txt
sleep 3

ls -art ~ > ./home/home.txt
echo -e '\n home.txt was created \n' 

cat ./home/home.txt
sleep 3

echo -e '\n DONE'