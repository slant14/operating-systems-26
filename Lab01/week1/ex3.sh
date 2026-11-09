mkdir -p root
date
sleep 3

mkdir -p home
date
sleep 3

date
sleep 3
touch root/root.txt
ls / -tr > root/root.txt


date
sleep 3
touch home/home.txt
ls ~ -tr > home/home.txt

echo -e "\nContent of root.txt:"
cat root/root.txt
echo -e "\nContent of home.txt:"
cat home/home.txt
echo -e "\nFolders root and home consist of:"
ls home/home.txt
ls root/root.txt