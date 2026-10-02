mkdir root
echo \`root\` directory was created
date
sleep 3

mkdir home
echo \`home\` directory was created
date
sleep 3

touch root/root.txt
echo \`root/root.txt\` file was created
date
sleep 3

touch home/home.txt
echo \`home/home.txt\` file was created
date
sleep 3

ls ~ --sort=time -r > root/root.txt
echo \`~\` directory was listed and sorted by time in reverse order\; Result was saved in \`root/root.txt\` file.
echo \`root/root.txt\` first 5 lines:
head root/root.txt -n 5

ls / --sort=time -r > home/home.txt
echo \`/\` directory was listed and sorted by time in reverse order\; Result was saved in \`home/home.txt\` file.
echo \`home/home.txt\` first 5 lines:
head home/home.txt -n 5
