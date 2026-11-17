## • 01HelloDog

```bash
pwd #check directory
ls -la #what files here
file 'hoi_:3.png' #found flag
```

Found the dog picture in `/home/ctf/go_to_opt`. - 

Downloading the file to my Mac: 

```bash
scp 'd.shestakov-dog@osctf.nnk.moe:/home/ctf/go_to_opt/hoi_:3.png' ~/Downloads/HelloDog.png //from where downland to where
```

## • 01BackgroundDog

```bash
jobs #check all jobs
fg %1 #start stopped job
```

Resumed the stopped job and found the path to the dog picture.

Stopped the job again using Ctrl + Z. //because normal people stop endless processes 

Downloading the file to my Mac:

```bash
scp 'd.shestakov-dog@osctf.nnk.moe:/regular_paw_storage/438a8382e5019580/Background_dog.png' ~/Downloads/Background_dog.png
```

## • 01HistoryDog

```bash
history #history of all commands
```

Found a command in the history that copied the dog picture to `/regular_paw_storage/929879a4a0cde63e/`.

```bash
file /regular_paw_storage/929879a4a0cde63e/grandpaw.png #checking file
```

Downloading the file to my Mac:

```bash
scp 'd.shestakov-dog@osctf.nnk.moe:/regular_paw_storage/929879a4a0cde63e/grandpaw.png' ~/Downloads/HistoryDog.png
```

## • 01CatDog

```bash
find . -type f #finding regular files
```

Found two parts of the dog picture in the pictures directory

```bash
file Pictures/Cat_dog_top Pictures/Cat_dog_bottom #type of files
ls -l Pictures #information about them
```

Combined both parts into one PNG file:

```bash
cat Pictures/Cat_dog_top Pictures/Cat_dog_bottom > /tmp/CatDog.png #main command of combing pictures
file /tmp/CatDog.png #checking maked file
```

Downloading the file to my Mac:

```bash
scp 'd.shestakov-dog@osctf.nnk.moe:/tmp/CatDog.png' ~/Downloads/CatDog.png
```



## • 01CompileDog

compile_dog.c was found during the previous task

```bash
cat Programs/compile_dog.c #checking obvious file compile_dog.c 
```

Compiled and executed the program:

```bash
gcc Programs/compile_dog.c -o /tmp/compile_dog #ordinary compilation i execute such a command 1000+ times
/tmp/compile_dog  #execute compiled program
```

The program printed the path to the dog picture

Downloading the file to my Mac:

```bash
scp 'd.shestakov-dog@osctf.nnk.moe:/regular_paw_storage/923515babea81ca0/Compile_dog.png' ~/Downloads/CompileDog.png
```

## • 01EditDog

```bash
find . -iname '*edit*' #finding all files with word "edit" inside
cat Doguments/EDITME
```

Found the task to make the ASCII dog 8 characters longer

```bash
nano Doguments/EDITME #it was so funny
```

Made the dog 8 characters longer while preserving its shape by _ and space

Downloading the file to my Mac:

```bash
scp 'd.shestakov-dog@osctf.nnk.moe:/regular_paw_storage/18a38ad4a0398be8/Edit_dog.png' ~/Downloads/EditDog.png
```

## • 01PoweredDog

The initial directory name go_to_opt was a hint to check the /opt directory

```bash
ls -la /opt
cat /opt/README.md #hmm file readme interesting(interesante from spanish)
```

Downloading the file to my Mac:

```bash
scp 'd.shestakov-dog@osctf.nnk.moe:/regular_paw_storage/4a95598ba491e617/powered_dog.png' ~/Downloads/PoweredDog.png
```

## • 01TailheadedDog

```bash
head Downloads/dog.log #see start of file
tail -n 30 Downloads/dog.log #see end of file
```

Found the first part of the path at the beginning of the file and the second part at the end

Combined both parts to get the full path to the dog picture

Downloading the file to my Mac:

```bash
scp 'd.shestakov-dog@osctf.nnk.moe:/regular_paw_storage/1181e9e656241fc9/tailheaded.png' ~/Downloads/TailheadedDog.png
```

## • 02MuteDog

```bash
./Programs/mute_dog #execute a program
```

Redirected stdout and stderr into separate files:

```bash
./Programs/mute_dog > /tmp/mute_stdout 2>/tmp/mute_stderr #change output of output and errors
cat /tmp/mute_stdout 
cat /tmp/mute_stderr
```

Combined both parts of the path to the dog picture

Downloading the file to my Mac:

```bash
scp 'd.shestakov-dog@osctf.nnk.moe:/regular_paw_storage/daa51d3aa1751cfc/Mute_dog.png' ~/Downloads/MuteDog.png
```

## • 02PetDog

Opened the project directory and read the documentation:

```bash
cd Doguments/auto_petter
cat README.md
git status #status of repository
git log --all --oneline --graph  #history of commits
cat tools/assemble_mascot.sh #just an initial script
```

Found an unfinished rebase and discovered that some versions of the image parts were corrupted

Checked the commit history of each part:

```bash
git log --all --oneline -- frames/part1.bin #history of this file
git log --all --oneline -- frames/part2.bin
git log --all --oneline -- frames/part3.bin
```

Restored the correct versions without changing the repository:

```bash
git show 479835b:frames/part1.bin > /tmp/part1.bin #rewrite initial file 3 commands
git show 8084348:frames/part2.bin > /tmp/part2.bin
git show 1ebbc53:frames/part3.bin > /tmp/part3.bin
```

Combined the parts in the order specified in the assembly script and checked the file type:

```bash
cat /tmp/part2.bin /tmp/part1.bin /tmp/part3.bin > /tmp/PetDog.png #combine into one file main command
file /tmp/PetDog.png
```

Downloading the file to my Mac:

```bash
scp 'd.shestakov-dog@osctf.nnk.moe:/tmp/PetDog.png' ~/Downloads/PetDog.png
```

## • 03InsecureDog

Searched for files related to the challenge:

```bash
find . -iname '*rem*' 2>/dev/null #find files with word "rem" in the name
cat Doguments/secret_stuff/remember.txt
ls -la Doguments/secret_stuff
```
Used brute force to find the filename:

```bash
cd Doguments/secret_stuff/super_secret
for name in {D,O,G}{D,O,G}{D,O,G}{D,O,G}; do
    if [ -f "$name.png" ]; then
        echo "$name.png"
    fi
done
```

Found the file OGDD.png

Downloading the file to my Mac:

```bash
scp 'd.shestakov-dog@osctf.nnk.moe:/home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/OGDD.png' ~/Downloads/InsecureDog.png
```

## • 02ScatteredDog

Searched for files related to the challenge:

```bash
find . -iname '*scatter*' 2>/dev/null #finding files with word "scatter" in name of the file
ls -la "Videos/Scattered dog"
```

Combined all files in numerical order

```bash
for i in {-50..49}; do
    cat "./$i"
done > /tmp/ScatteredDog.png

```

Downloading the file to my Mac:

```bash
scp 'd.shestakov-dog@osctf.nnk.moe:/tmp/ScatteredDog.png' ~/Downloads/ScatteredDog.png
```

## • 02ShyDog

```bash
find . -iname '*shy*' 2>/dev/null
ls -la "Videos/Shy dog"
```

Found a directory containing many files with different extensions
Checked the actual file formats and filtered the output to find PNG images:

```bash
cd "Videos/Shy dog"
file -- * | grep 'PNG image data' #check real format of each file
```

Found only one real PNG image named Backup_Maybe_Dog_cb951b9a.png

Downloading the file to my Mac:

```bash
scp 'd.shestakov-dog@osctf.nnk.moe:/home/ctf/go_to_opt/Videos/Shy dog/Backup_Maybe_Dog_cb951b9a.png' ~/Downloads/ShyDog.png
```

## • 02RemDog

need to remove angry dogs

```bash
cat Doguments/unsorted/README
```

Listed the files and compared two ASCII dog pictures with different file sizes

```bash
ls -la Doguments/unsorted
cat Doguments/unsorted/004a052c
cat Doguments/unsorted/02674f7a
```

Found that angry dogs occupied 1937 bytes and calm dogs occupied 1678 bytes

```bash
find Doguments/unsorted -type f -size 1937c
```

Removed all 13 angry dogs and received the path to the dog picture

```bash
find Doguments/unsorted -type f -size 1937c -delete
```

Downloading the dog picture to my local machine

```bash
scp d.shestakov-dog@osctf.nnk.moe:/regular_paw_storage/7433fe516237cf51/Rem_dog.png ~/Downloads/RemDog.png
```
