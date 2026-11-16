# CTF dogs (findingdory)

## 1. 01BackgroundDog
I find all processes, using:
```bash
ps aux
```
We get all processes, but the important one is:
ctf          239  0.0  0.0    980   656 pts/0    T    11:34   0:00 /usr/local/bin/background_dog /tmp/bgdog

Then I check the status and get, that it has the number 1, and it has been suspended
```bash
jobs
```
Then I continued the execution of this process in the background
```bash
bg %1
```
**Answer:** */regular_paw_storage/05eb161e2642d038/Background_dog.png*

Then, I kill the process
```bash
kill %1
```

## 2. 01CatDog
I go to the directory 'Pictures'
```bash
ls
cd Pictures
ls
```
I see two parts of an image (Cat_dog_bottom  Cat_dog_top) and combine them into a single file
```bash
cat Cat_dog_top Cat_dog_bottom > Cat_dog.png
```
I get the file, that I need: *Cat_dog.png*  Cat_dog_bottom  Cat_dog_top

I see the path, using
```bash
pwd
```
**Answer:** /home/ctf/go_to_opt/Pictures => */home/ctf/go_to_opt/Pictures/Cat_dog.png*

## 3. 01CompileDog
I go to the directory 'Programs'
```bash
cd Programs
ls
```
We see the file 'compile_dog.c'. I compiled and run.
```bash
gcc compile_dog.c -o main
./main
```
**Answer:** */regular_paw_storage/b1377bc26bc83199/Compile_dog.png*

## 4. 01EditDog
I go to the directory 'Doguments'
```bash
cd Doguments
ls
```
I see the file EDITME and let's open it:
```bash
nano EDITME
```
I see the task:

Make the dog 7 characters longer. Preserve the integrity of the dog.

DO NOT EDIT ANYTHING OUTSIDE THESE SAFETY LINES
\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\


                         .-.
     (__________________()6 `-,
     (   ______________   /''"`
     //\\             //\\
jgs  "" ""            "" ""


\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
DO NOT EDIT ANYTHING OUTSIDE THESE SAFETY LINES

I need to make the dog longer (+7 characters):

Make the dog 7 characters longer. Preserve the integrity of the dog.

DO NOT EDIT ANYTHING OUTSIDE THESE SAFETY LINES
\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\


                                .-.
     (_________________________()6 `-,
     (   _____________________   /''"`
     //\\                    //\\
jgs  "" ""                   "" ""


\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
DO NOT EDIT ANYTHING OUTSIDE THESE SAFETY LINES

Save the file: Ctrl + O, Enter, Ctrl + X

**Answer:** */regular_paw_storage/d7f2d764bb17099a/Edit_dog.png*

## 5. 01HelloDog
In the directory 'go_to_opt' I see the file 'hoi_:3.png'.

**Answer:** */home/ctf/go_to_opt/hoi_:3.png*

## 6. 01HistoryDog
I need to see the history, I use the command:
```bash
history
```
**Answer:** */regular_paw_storage/8c9ce192e382cbeb/grandpaw.png*

## 7. 01PoweredDog
I see that we have the directory 'go_to_opt' that says that I need to go to 'opt'. So I tried to find this directory, then I find file 'README.md' and read it
```bash
ls
cd ..
ls
cd ..
ls
cd ..
ls
cd opt
ls
cat README.md
```
**Answer:** */regular_paw_storage/7b1b72fbbe0203b6/powered_dog.png*

## 7. 01TailheadedDog
I go to the directory 'Downloads'
```bash
cd Downloads
ls
```
I see the file 'dog.log'. Let's see the head and tail of this file.

```bash
head dog.log
tail -n 20 dog.log
```
I get two parts of the path of the dog: /regular_paw_storage/c7b092e2 and f9a52281/tailheaded.png. I join two parts and get our dog.

**Answer:** */regular_paw_storage/c7b092e2f9a52281/tailheaded.png*

## 8. 02MuteDog
I go to the directory 'Programs'
```bash
cd Programs
ls
```
I see the file 'mute_dog'. I run the program and save its outputs, icnluding all errors. Then I read the file 'dog.txt'
```bash
./mute_dog > dog.txt 2>&1
cat dog.txt
```
I get: 605cc918/Mute_dog.png/regular_paw_storage/14545041

**Answer:** */regular_paw_storage/14545041605cc918/Mute_dog.png*

## 9. 02RemDog
I go to the directory 'Doguments/unsorted' and see the file README and read it
```bash
cd Doguments
ls
cd unsorted
ls -la
cat README
```
I see: I need to remove all the angry dogs from this folder some day... All angry dogs has size 1937 (I see use the command 'ls -la')

I remove all angry dogs
```bash
find . -maxdepth 1 -type f -size 1937c -delete
```
**Answer:** */regular_paw_storage/81f3f7d43c3704db/Rem_dog.png*

## 10. 02ScatteredDog
I go to the directory 'Videos/'Scattered dog''.
```bash
cd Videos
ls
cd 'Scattered dog'
ls
```
I see files with names from -50 to 49. I combine all files in the single file.
```bash
for i in $(seq -50 49); do cat -- "./$i"; done > ../Scattered_dog.png
```
I get file 'Scattered_dog.png':
```bash
cd ..
ls
pwd
```
**Answer:** */home/ctf/go_to_opt/Videos/Scattered_dog.png*

## 11. 02ShyDog
I go to the directory 'Videos/'Shy dog''.
```bash
cd Videos
ls
cd 'Shy dog'
ls
```
I see all files and find the one with png format, using the command:
```bash
file *
```
I find Winter_Maybe_Dog_f1ad22ae.png:   PNG image data, 150 x 200, 8-bit colormap, non-interlaced

I move this file to another directory
```bash
mv Winter_Maybe_Dog_f1ad22ae.png ..
```
**Answer:** */home/ctf/go_to_opt/Videos/Winter_Maybe_Dog_f1ad22ae.png*

## 12. 02SpitoonDog
I go to the directory 'Programs' and see the file spitoon and compile it
```bash
cd Programs
ls
./spitoon
```
I get a message and answer it, then i see a hint to use the command 'man':

Welcome to the salty spitoon, how tough are ya?

jkn j

Listen, kid: come back when you are more of a man
```bash
man spitoon
```
I see the instruction, how to answer questions:
Welcome to the salty spitoon, how tough are ya?
How tough am I? How tough am I?! I ran 112 of linux programs this lab
Yea, so?
Without 112 errors
Right this way sir: /regular_paw_storage/6f813f960a992354/Spitoon_dog.png, sorry to keep you waiting
**Answer:** */regular_paw_storage/6f813f960a992354/Spitoon_dog.png*

## 13. 03InsecureDog
I go to the directory 'Doguments/secret_stuff'.
```bash
cd Doguments
ls
cd secret_stuff
ls
```
I find the file 'rememeber.txt' and read it. It is a hint that the filename has four letters made from D, O, G.
```bash
cat remember.txt
```
I keep forgetting the filename for that sweet secret dog... 

I put it in the super_secret folder inside secret_stuff in my Doguments.

I know the name was a mix of the letters D, O, and G. 

It was four letters long, and one of those letters appeared twice. 

What was it again? DOGG.png? GOGO.png? DOOG.png?

Then I find the name of the file, because I can't see the files in the 'super_secret'
```bash
for name in {D,O,G}{D,O,G}{D,O,G}{D,O,G}; do
    [ -f "secret_stuff/super_secret/$name.png" ] && echo "$name.png"
done
```
I find the file: GODO.png
**Answer:** */home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/GODO.png*

## 14. 03NetDog
I see nets
```bash
cat /proc/net/tcp
```
I get:

  sl  local_address rem_address   st tx_queue rx_queue tr tm->when retrnsmt   uid  timeout inode 
  
   0: 00000000:0AF9 00000000:0000 0A 00000000:00000000 00:00000000 00000000     0        0 184893000 1 0000000000000000 100 0 0 10 0     
   
   1: 00000000:0016 00000000:0000 0A 00000000:00000000 00:00000000 00000000  1000        0 184915020 1 0000000000000000 100 0 0 10 100        
   
   2: 220011AC:0016 010011AC:875A 01 00000000:00000000 00:00000000 00000000  1000        0 185048397 3 0000000000000000 20 4 31 10 -1
   
I found the port 0AF9 in decimal system 2809

I connected to the port and save the answer in the file
```bash
nc -w 3 127.0.0.1 2809 > ~/Net_dog.png
```
**Answer:** */home/ctf/Net_dog.png*

## 15. 03RestoreDog
I go to the directory 'Doguments' and read the file 'crashed.txt'
```bash
cd Doguments
ls
cat crashed.txt
```
I get:

My session crashed while I was writing down where I hid my favorite dog...

Good thing vim keeps swap files around.

I encrypted my notes, so here is the recovery key: c46d9cbd2035c716

I get the hint to use vim:
```bash
vim -r crashed.txt
```
I use the recovery key and get the answer

**Answer:** */regular_paw_storage/xikhc8a/Restore_dog.png*

## 16. 03UsedDog
I go to the directory 'home/dog' and see the file 'Used_dog.png'
```bash
ls
cd ..
ls
cd ..
cd dog
ls
```
**Answer:** */home/dog/Used_dog.png*


