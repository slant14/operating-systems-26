# Bash CTF (dogctf)

## 01BackgroundDog

I saw this right after connecting to vm:

```
Hiiii :3

[1]+  Stopped                 /usr/local/bin/background_dog /tmp/bgdog
```
Suspicious job 1 was stopped in the background. Run it in foreground:

```
fg %1
```

## 01CatDog

```
cd Pictures
cat Cat_dog_top Cat_dog_bottom > Cat_dog.png
```
To find path to Cat_dog.png I used pwd.

## 01CompileDog

```
cd Programs
gcc compile_dog.c -o compile.dog
./compile.dog
```

## 01EditDog

```
cd Doguments
ls
nano EDITME
```
Make the dog 8 characters longer (in each row add 8 spaces or underscores).

## 01HelloDog

```
ls
pwd
```
Download suspicious hoi_:3.png. Find its path using pwd.

## 01HistoryDog

```
history
```
At 13th line find path to dog.

## 01PoweredDog

```
cd ~
ls
```
It says "go_to_opt". Ok, let's go to /opt.
```
cd /opt
ls
nano README.md
```

## 01TailheadedDog

```
cd Downloads/
head dog.log -n 3
tail dog.log -n 20
```
Combine 2 parts of path to Tailheaded dog. (head - start of path, tail - end of path)

## 02MuteDog

```
cd Programs/
./mute_dog
```
The terminal output was “Terminals are listening,” so let’s redirect the output to a file. 
```
./mute_dog > mute.txt
```
Some part of path still missing, let's check error stream:
```
./mute_dog 2>> mute.txt
```

## 02PetDog

```
cd Doguments/auto_petter
nano README.md
```
We can’t immediately assemble the `mascot.png` file because it’s corrupted (./tools/assemble_mascot.sh gives a warning). We need to find its raw fragments in `frames/` using the git commit history.
```
git reflog 
```
Find the right commits based on the description:
```
2ba837d HEAD@{5}: commit: hotfix: pull part2 from rotation until the artifact source is known
# here part2 was moved, so pick previous commit to restore it
10d3116 HEAD@{6}: commit: mascot: derive a thumbnail-grade part1 for the status screen


1842b5a HEAD@{7}: commit: hotfix: replace part3 with a stub while artifacts are investigated
# here part 3 was replaced with sth, so pick previous commit to restore it
4c3e411 HEAD@{8}: commit: hotfix: clamp intensity after the park incident


088a061 HEAD@{13}: commit: mascot: swap placeholder part1 for the final encode from design
# here part1 was improved for final encode, so pick this commit to restore part1
```

Then restore parts and assemble mascot again.
```
git checkout 088a061 -- frames/part1.bin
git checkout 10d3116 -- frames/part2.bin
git checkout 4c3e411 -- frames/part3.bin
./tools/assemble_mascot.sh
```

## 02RemDog

```
cd Doguments/unsorted
ls
nano README
```
We need to remove all the angry dogs. After opening some files, I noticed that the files with angry dogs contain '8_mGk_8'.
```
grep -l '8_mGk_8' * | xargs rm
```
Find this pattern in files of current directory, make list of file names, convert this list to arguments for remove, and, finally, remove files. 

## 02ScatteredDog

```
cd Videos/'Scattered dog'
ls
```
Let’s concatenate all the data files in ascending order of their names:
```
cat -- {-50..49} > scattered.png
```
Since the path contains a directory with a space in its name, the remote path is wrapped in quotes and the space is escaped with a backslash. (as in Shy dog)

## 02ShyDog

```
cd Videos/'Shy dog'
file *
```
Among all the files in this directory, there is actually 1 real PNG file. 
```
Winter_Maybe_Dog_9fb988de.png:   PNG image data, 150 x 200, 8-bit colormap, non-interlaced
```

Since the path contains a directory with a space in its name, the remote path is wrapped in quotes and the space is escaped with a backslash.
```
scp -O "a.glinskaia-dog@osctf.nnk.moe:/home/ctf/go_to_opt/Videos/Shy\ dog/Winter_Maybe_Dog_9fb988de.png" Downloads
```

## 02SpitoonDog

```
cd Programs/
./spitoon
```
It says: "Listen, kid: come back when you are more of a man". So use man command to check correct usage!
```
man spitoon
```
Follow the example or correct usage to get dog path:
```
Welcome to the salty spitoon, how tough are ya?
How tough am I? How tough am I?! I ran 5 of linux programs this lab
Yea, so?
Without 5 errors^C
Right this way sir: /regular_paw_storage/0487a8a577eadff8/Spitoon_dog.png, sorry to keep you waiting
```

## 03AqueousDog

```
cd /home/ctf/.config/fish
nano config.fish
```
File contains some commands and text: "It's nice to use nice shells, but some tasks are bash-only". So let's use bash to read and run commands from this file:
```
bash config.fish
```

## 03InsecureDog

```
cd Doguments/
cd secret_stuff/
nano remember.txt
```
We need to go to super_secret/ and find a file with a name containing 4 letters D, O, G, one of which is repeated twice. We can simply go through all the possible names consisting of 4 letters (no need to check repetitions).

```
cd super_secret/
ls {D,O,G}{D,O,G}{D,O,G}{D,O,G}.png
```

## 03NetDog

```
htop
```
Check suspicious /usr/local/bin/netdog command.
```
cd /usr/local/bin
./netdog
```
We get using port: 1097. Let's connect to it using netcat:
```
nc localhost 1097 > /home/ctf/go_to_opt/netdog.png
```
## 03ProDog

```
htop
```
Check suspicious /usr/local/sbin/prodog_loop. We can notice that processes with name `prodog` appear from time to time. Let's kill all such processes of ctf user (we have no permission to kill other users' processes):
```
pkill -u ctf prodog
```
Repeat this few times.

## 03RestoreDog

```
cd Doguments/
nano crashed.txt
```
We need to recover the data using a Vim swap file with recovery key from crashed.txt:
```
ls -a
vim -r .crashed.txt.swp
```
Find hidden Vim swap file and recover it. 

## 03UsedDog

```
cd /home/dog
```