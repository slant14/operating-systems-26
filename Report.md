# Dog CTF

### 1. 01BackgroundDog
We can understand that it is connected with background processes. So I checked:
```bash
ps aux
```
I found there the stopped process: *ctf          238  0.0  0.0    980   656 pts/0    T    15:32   0:00 /usr/local/bin/background_dog /tmp/bgdog*

So I checked
```bash
bg %1
```
and found: */regular_paw_storage/b8baa78c118e5a29/Background_dog.png*

### 2. 01CatDog
I checked the files in directory:
```bash
ls
```
I went to Pictures and checked files in there:
```bash
cd Pictures
ls
```
There I found: *Cat_dog_bottom  Cat_dog_top*

So I combined them and got a complete picture.
```bash
cat Cat_dog_top Cat_dog_bottom > Cat_dog.png
```
I took the directory by:
```bash
pwd
```

### 3. 01CompileDog
I went to Programs folder and found *compile_dog.c*

So I compiled and run the program and got: */regular_paw_storage/d3805ef4565a3835/Compile_dog.png*
```bash
gcc compile_dog.c -o compile
./compile
```

### 4. 01EditDog
I went to Doguments folder and fount *EDITME* file. I opened it:
```bash
nano EDITME
```
There was:

Make the dog 4 characters longer. Preserve the integrity of the dog.

DO NOT EDIT ANYTHING OUTSIDE THESE SAFETY LINES
\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\


                                          .-.
     (___________________________________()6 `-,
     (   _______________________________   /''"`
     //\\                              //\\
jgs  "" ""                             "" ""


\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
DO NOT EDIT ANYTHING OUTSIDE THESE SAFETY LINES

So I did what is asked:

Make the dog 4 characters longer. Preserve the integrity of the dog.

DO NOT EDIT ANYTHING OUTSIDE THESE SAFETY LINES
\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\


                                              .-.
     (_______________________________________()6 `-,
     (   __________________________________   /''"`
     //\\                                  //\\
jgs  "" ""                                 "" ""


\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
DO NOT EDIT ANYTHING OUTSIDE THESE SAFETY LINES

and got:

*Woof! You have solved Edit_dog!*
*Here's your dog pic: /regular_paw_storage/289d28c64da4fabb/Edit_dog.png*

### 5. 01HelloDog
I looked the files in the starting directory:
```bash
ls
```
and saw *hoi_:3.png*.

### 6. 01HistoryDog

I looked the history:
```bash
history
```
and found */regular_paw_storage/c16ceecd3f1b618a/grandpaw.png*

### 7. 01TailheadedDog

I went to Downloads folder and looked what is there:
```bash
cd Downloads
ls
```
I found *dog.log* file. I tried to read it and saw that there was the end of the path, but the file was too big, so I used:
```bash
head -n 20 dog.log
tail -n 20 dog.log
```

I found the beginning and the end of the path:

*/regular_paw_storage/9eb2aaef*

*3baeba30/tailheaded.png*

### 8. 02MuteDog

I went to Programs folder and found *mute_dog*
```bash
cd Programs
ls
```

I tried to run it and there was a picture: *Terminals are listening*

So I tried to write the output to the file:
```bash
./mute_dog > answer.txt
```

I got one part of the path. So I tried to write errors to the output as well.
```bash
./mute_dog > answer.txt 2>&1
cat answer.txt
```

and got: *d933fd0a/Mute_dog.png/regular_paw_storage/e8fdc830* = */regular_paw_storage/e8fdc830d933fd0a/Mute_dog.png*

### 9. 02RemDog

I went to Doguments folder and found there unsorted folder.
```bash
cd Dogumemts
ls
cd unsorted
ls
```

In this folder I found some files and README, where was: *I need to remove all the angry dogs from this folder some day...*
```bash
cat README
```

I looked at files by:
```bash
ls -l
```

and it turned out that angry dogs were files with size 1937, so I deleted them:
```bash
find . -type f -size 1937c -delete
```
and got: *Here's your dog pic: /regular_paw_storage/4fa46cf5a8b89c48/Rem_dog.png*

### 10. 02ScatteredDog

I went to Videos folder and found there 'Scattered dog' folder.
```bash
cd Videos
ls
cd  'Scattered dog'
ls
```

There were files named with numbers from -50 to 49. I tried to combine them into png file and got the flag.
```bash
for i in $(seq -50 49); do cat -- "./$i"; done > ../Scattered_dog.png
```

I got the path by:
```bash
cd ..
pwd
```

### 11. 02ShyDog

I went to Videos folder and found there 'Shy dog' folder.
```bash
cd Videos
ls
cd 'Shy dog'
ls
```

There were a lot of files but I tried to look for png file and there was only one:
```bash
file *
```
*Snapshot_Maybe_Dog_3108badb.png* I moved it to another directory because I couldn't send file from 'Shy dog':
```bash
mv Snapshot_Maybe_Dog_3108badb.png ..
```

### 12. 02SpitoonDog

I went to Programs folder and found spitoon:
```bash
cd Programs
```

I tried to run it:
```bash
./spitoon
```

There was text: 

Welcome to the salty spitoon, how tough are ya?

kc

Listen, kid: come back when you are more of a **man**

I wrote random answer and got a hint that I have to see **man**.
```bash
man spitoon
```

There I saw how I have to answer.
```
Welcome to the salty spitoon, how tough are ya?
How tough am I? How tough am I?! I ran 185 of linux programs this lab
Yea, so?
Without 185 errors^C
Right this way sir: /regular_paw_storage/spitoon/tough_dog.png, sorry to keep you waiting
```

By doing this I got: *regular_paw_storage/b8f5933753560a19/Spitoon_dog.png*

13. ### 03InsecureDog

I went to Doguments folder and found secret_stuff folder:
```bash
cd Doguments
ls
cd secret_stuff
ls
```

There was remember.txt
```bash
cat remember.txt
```
```
I keep forgetting the filename for that sweet secret dog... 
I put it in the super_secret folder inside secret_stuff in my Doguments.

I know the name was a mix of the letters D, O, and G. 
It was four letters long, and one of those letters appeared twice. 
What was it again? DOGG.png? GOGO.png? DOOG.png?
```

I tried to go into super_secret folder but I could not check files in there because I had no permission:
```bash
cd super_secret
ls
```

```
ls: cannot open directory '.': Permission denied
```

So I tried to find the file by:
```bash
for n in {D,O,G}{D,O,G}{D,O,G}{D,O,G}; do     [ -f "$n.png" ] && echo "$n.png"; done
```

and got: *GDGO.png*

14. ### 03NetDog

By the name I could understand that I have to look nets.
```bash
cat /proc/net/tcp
```

```
  sl  local_address rem_address   st tx_queue rx_queue tr tm->when retrnsmt   uid  timeout inode                                                     
   0: 00000000:0F90 00000000:0000 0A 00000000:00000000 00:00000000 00000000     0        0 176250286 1 0000000000000000 100 0 0 10 0                 
   1: 00000000:0016 00000000:0000 0A 00000000:00000000 00:00000000 00000000  1000        0 176320615 1 0000000000000000 100 0 0 10 100               
   2: 170011AC:0016 010011AC:AD00 01 00000000:00000000 00:00000000 00000000  1000        0 176320617 3 0000000000000000 20 7 31 10 320 
```
```bash
nc -w 3 127.0.0.1 3984 > Net_dog.png
```

### 15. 03RestoreDog

I went to Doguments folder and found crashed.txt
```bash
cd Doguments
ls
cat crashed.txt
```

```
My session crashed while I was writing down where I hid my favorite dog...
Good thing vim keeps swap files around.
I encrypted my notes, so here is the recovery key: 6b425e6754ac2faa
```

So I opened the file in recovery mode and entered there the recovery key:
```bash
vim -r crashed.txt
```
I got: */regular_paw_storage/n23k9i7/Restore_dog.png*




