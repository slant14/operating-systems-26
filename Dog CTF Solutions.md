# 01BackgroundDog

```
jobs
fg "%/usr/local/bin/background_dog" 
Ctrl + C
```

# 01CatDog 

```
cd /home/ctf/go_to_opt/Pictures
cat Cat_dog_top Cat_dog_bottom > CatDog.png 
```

# 01CompileDog 

```
cd /home/ctf/go_to_opt/Programs
gcc compile_dog.c -o compile_dog.out
./compile_dog.out
```

# 01EditDog 
```
cd /home/ctf/go_to_opt/Doguments
vim EDITME
```
Make dog longer by 5 symbols

# 01HelloDog 

```
cd /home/ctf/go_to_opt
ls 
```

# 01HistoryDog
```
history | grep "cp /super_secret_doggo_vault/grandpaw.png"
```

# 01PoweredDog 
```
cd ~
ls
cd /opt
cat README.md
```

# 01TailheadedDog
```
cd /home/ctf/go_to_opt/Downloads
head -n 2 dog.log
tail -n 14 dog.log
```

# 02MuteDog 
```
cd /home/ctf/go_to_opt/Programs
./mute_dog > ans.txt
./mute_dog 2>> ans.txt
cat ans.txt
```

# 02PetDog 

```
cat README.md
cat ./tools/assemble_mascot.sh

git reflog

git checkout 80b4697 -- frames/part1.bin
git checkout e57174c -- frames/part2.bin
git checkout 09b0014 -- frames/part3.bin

./tools/assemble_mascot.sh
```
We collect parts from specific commits. We select them based on the comments.
```
4ef3d79 (HEAD) HEAD@{0}: rebase (pick): config: longer default sessions
cae5d38 HEAD@{1}: rebase (pick): docs: add turbo mode roadmap
df366ae (master) HEAD@{2}: rebase (start): checkout master
0b162bc (dev) HEAD@{3}: checkout: moving from master to dev
df366ae (master) HEAD@{4}: commit: hotfix: tail sensor debounce


d491c74 HEAD@{5}: commit: hotfix: stub out part3 pending investigation of the corruption reports

09b0014 HEAD@{6}: commit: mascot: shrink part2 to fit the kiosk panel's memory budget
%%commit before corruption of part3%%



d0e8ca4 HEAD@{7}: commit: hotfix: replace part1 with a stub while artifacts are investigated

80b4697 HEAD@{8}: commit: hotfix: clamp intensity after the park incident
%%commit before corruption of part1%%

06fe95a HEAD@{9}: commit: docs: describe the mascot frame pipeline
8226744 HEAD@{10}: checkout: moving from dev to master
0b162bc (dev) HEAD@{11}: commit: core: adaptive petting cadence
ad99627 HEAD@{12}: commit: wip: sharpen part3 (halos around the edges, not an improvement)


e57174c HEAD@{13}: commit: mascot: design sent the final part2 encode, dropping the placeholder
%%commit with final version of part2%%

ecc7260 HEAD@{14}: commit: wip: upscale part1 for the retina mockup (ringing everywhere, parking 
this)
e11a40f HEAD@{15}: commit: feature: turbo petting mode
0cc7e9e HEAD@{16}: commit: config: longer default sessions
a15e701 HEAD@{17}: commit: docs: add turbo mode roadmap
8226744 HEAD@{18}: checkout: moving from master to dev
8226744 HEAD@{19}: commit: config: tune default petting intensity
fe807c5 HEAD@{20}: commit: petting core: first working loop
0e3d3bc HEAD@{21}: commit (initial): initial commit: autopetter skeleton, mascot frames from the 
design drop
```

# 02RemDog (141 points)
```
cd /home/ctf/go_to_opt/Doguments/unsorted
cat README   
ls -la
cat 0a5d0441 
cat fe9241fa 

find . -type f -size "1937c" -delete 
```
Files with angry dogs have a size of 1937 bytes. We find all of them and delete them.
# 02ScatteredDog
```
cd /home/ctf/go_to_opt/Videos/Scattered\ dog/
ls
cat -- $(seq -50 49) >> ~/go_to_opt/res.png
```
We combine all the files of Scattered dog in order.

# 02ShyDog 
```
cd /home/ctf/go_to_opt/Videos/Shy\ dog/
ls
file IMG_archive_5b269c64.png
find . -type f -name "*.png" | xargs file | grep "PNG"
mv IMG_Maybe_Dog_9f8f3233.png ~/go_to_opt/
```
File names may not match their actual type. We find the PNG-type files among all of them.

# 02SpitoonDog
```
cd /home/ctf/go_to_opt/Programs
man spitoon 
How tough am I? How tough am I?! I ran 168 of linux programs this lab
Without 168 errors^C
```
We follow the instructions for proper use from the manual.

# 03AqueousDog 
```
cd /home/ctf/.config/fish
bash config.fish
```

# 03InsecureDog 
```
cd /home/ctf/go_to_opt/Doguments/secret_stuff
cat remember.txt 
cd super_secret
ls {D,O,G}{D,O,G}{D,O,G}{D,O,G}.png 2>null
```
We go through all possible names made up of 4 letters: D, O, or G.

# 03NetDog 
```
ps aux 
cd /usr/local/bin
./netdog
```
Using port: 3706. We connect to it and collect received data.
```
cd /home/ctf/go_to_opt
nc localhost 3706 > netdog.png
```


# 03ProDog 
```
ps aux
for i in $(seq 0 2000); do ps aux; done | grep -vE "ps aux|sleep|vim|background_dog|-bash|dropbear|netfog|sort" | sort -u
```
We try to find short-lived processes that we did not see earlier. 
```
while (! pkill -u ctf prodog); do :; done
```
Kill all prodog processes.
# 03RestoreDog 
```
cd /home/ctf/go_to_opt/Doguments
cat crashed.txt 
ls -la
vim -r .crashed.txt.swp
```
We restore data from vim swap file.

# 03UsedDog 
```
cd /home/dog
ls
```
