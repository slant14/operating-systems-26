# Bash CTF Write-up

## 01HelloDog — Accepted

**Goal:** Find the dog image and submit it to the CTF bot.

### Steps

1. Opened the challenge directory and listed its files:

   ```bash
   cd /home/ctf/go_to_opt
   ls -la
   ```

   Found `hoi_:3.png` (60,446 bytes).

2. Downloaded the image to my local Ubuntu WSL directory using a separate terminal:

   ```bash
   mkdir -p ~/fctf01
   cd ~/fctf01
   scp -O 'o.korchagin-dog@osctf.nnk.moe:/home/ctf/go_to_opt/hoi_:3.png' ./01HelloDog.png
   ```

   `scp` transfers the file over SSH; `-O` enables the legacy SCP protocol. I renamed the local copy to `01HelloDog.png`.

3. Opened the image and submitted it to the CTF bot.

**Result:** The image was accepted. Challenge completed.


## 02. 01BackgroundDog

**Goal:** Find and download the dog image using Linux background processes.

### Commands

Check background jobs:

```
jobs -l
```

Found the process:

```
/usr/local/bin/background_dog /tmp/bgdog
```

Resume the stopped process:

```
bg %1
```

The process revealed the image path:

```
/regular_paw_storage/ac0d291ccad5ef01/Background_dog.png
```

Stop the background process and verify the file:

```
kill %1
file /regular_paw_storage/ac0d291ccad5ef01/Background_dog.png
```

Download the image using SCP from Ubuntu WSL:

```
scp -O 'o.korchagin-dog@osctf.nnk.moe:/regular_paw_storage/ac0d291ccad5ef01/Background_dog.png' ./01BackgroundDog.png
```

### Explanation

I used `jobs -l` to identify the background task and `bg %1` to resume it. The process printed the location of the PNG image. After finding the path, I verified the file and downloaded it using SCP.

**Result:** Successfully downloaded the image and completed the challenge.


## 03. 01CompileDog

**Goal:** Find, compile, and execute a C program to locate the dog image.

### Commands

**1. Find the source code**

```
cd /home/ctf/go_to_opt
ls -la Programs
file Programs/*
```

Found `Programs/compile_dog.c`, identified as a C source file.

**2. Inspect the source code**

```
head -n 45 Programs/compile_dog.c
tail -n 90 Programs/compile_dog.c
```

The code contains a `main()` function that repeatedly calls `prance()`. The program uses `putchar()` to print characters and `exit(0)` to terminate.

**3. Compile and execute the program**

```
gcc Programs/compile_dog.c -o /tmp/compile_dog
/tmp/compile_dog
```

Output:

```
/regular_paw_storage/5f5c7535619bf665/Compile_dog.png
```

**4. Download the image using Ubuntu WSL**

```
cd /home/franxx/fctf01
scp -O 'o.korchagin-dog@osctf.nnk.moe:/regular_paw_storage/5f5c7535619bf665/Compile_dog.png' ./01CompileDog.png
```

**5. Verify the downloaded file**

```
ls -lh 01CompileDog.png
file 01CompileDog.png
```

### Explanation

I located the C source file using `ls` and `file`, then inspected its contents with `head` and `tail`. After identifying the output logic, I compiled the program using `gcc` and executed it. The program printed the hidden PNG path, which I used to download the image through SCP.

	**Result:** Successfully found and downloaded the dog image. Challenge completed.


## 04. 02ScatteredDog

**Goal:** Reassemble a PNG image from 100 numbered fragments and download it.

### 1. Find the fragments

```
cd /home/ctf/go_to_opt
ls -la Videos
ls -la 'Videos/Scattered dog' | head -n 20
```

Found numbered files inside `Videos/Scattered dog`.

### 2. Find the PNG header and count the fragments

```
file -- 'Videos/Scattered dog/'* | grep 'PNG image data'
find 'Videos/Scattered dog' -maxdepth 1 -type f | wc -l
```

Found `-50` as the PNG header and `100` fragments.

### 3. Determine the correct order

```
find 'Videos/Scattered dog' -maxdepth 1 -type f -printf '%f\n' | sort -n | head -n 10
find 'Videos/Scattered dog' -maxdepth 1 -type f -printf '%f\n' | sort -n | tail -n 10
```

The numeric sequence starts at `-50` and ends at `49`.

### 4. Reassemble the image

```
cat -- "Videos/Scattered dog/"{-50..49} > /tmp/Scattered_dog.png
file /tmp/Scattered_dog.png
ls -lh /tmp/Scattered_dog.png
```

The `cat` command combines all 100 fragments in the correct order.

**Output:** PNG image, 300 × 195 pixels, approximately 44 KB.

### 5. Download using Ubuntu WSL

```
cd /home/franxx/fctf01
scp -O 'o.korchagin-dog@osctf.nnk.moe:/tmp/Scattered_dog.png' ./02ScatteredDog.png
```

Verify and open the file:

```
file 02ScatteredDog.png
ls -lh 02ScatteredDog.png
explorer.exe .
```

### Result

Successfully reconstructed the PNG from 100 fragments, downloaded the image, and completed the challenge.




## 05. 02ShyDog

**Goal:** Find the real PNG image among 1,000 files and download it.

### 1. Inspect the directory

```
cd /home/ctf/go_to_opt
ls -la 'Videos/Shy dog' | head -n 20
find 'Videos/Shy dog' -maxdepth 1 -type f | wc -l
```

**Output:** 1,000 files with different names and extensions.

### 2. Identify the file types

```
file -b --mime-type 'Videos/Shy dog/'* | sort | uniq -c
```

**Output:** Only one file has the `image/png` MIME type.

### 3. Find the real PNG

```
file --mime-type 'Videos/Shy dog/'* | grep 'image/png'
```

**Output:**

```
Videos/Shy dog/Backup_Maybe_Dog_fb3c3c14.png: image/png
```

### 4. Download using Ubuntu WSL

```
cd /home/franxx/fctf01
scp -O 'o.korchagin-dog@osctf.nnk.moe:/home/ctf/go_to_opt/Videos/Shy\ dog/Backup_Maybe_Dog_fb3c3c14.png' ./02ShyDog.png
```

The `\` escapes the space in `Shy dog` for legacy SCP. Without it, the download fails.

### 5. Verify the image

```
file 02ShyDog.png
ls -lh 02ShyDog.png
explorer.exe .
```

### Explanation

I searched through 1,000 files and used `file --mime-type` to identify their actual formats. Only one file was a valid PNG. I found its path using `grep` and used SCP to download it.

**Result:** Successfully identified the correct PNG file. The downloaded image can be verified and submitted to the CTF bot.

## 06. 03RestoreDog

**Goal:** Recover an encrypted Vim swap file and find the hidden PNG image.

### 1. Inspect the directory

```
cd /home/ctf/go_to_opt
ls -la Doguments
cat Doguments/crashed.txt
```

Found `.crashed.txt.swp` and the encryption key:

```
040de3f5447a594a
```

### 2. Recover the Vim swap file

```
vim -r Doguments/crashed.txt
```

The first recovery attempt displayed unreadable characters.

Retry with Vim's default configuration:

```
vim -Nu NONE -i NONE -r Doguments/crashed.txt
```

Enter the encryption key when prompted:

```
040de3f5447a594a
```

Recovered image path:

```
/regular_paw_storage/mt7vpqv/Restore_dog.png
```

Exit Vim without modifying the original file:

```
:q!
```

### 3. Download using Ubuntu WSL

```
cd /home/franxx/fctf01
scp -O 'o.korchagin-dog@osctf.nnk.moe:/regular_paw_storage/mt7vpqv/Restore_dog.png' ./03RestoreDog.png
```

### 4. Verify the image

```
file 03RestoreDog.png
ls -lh 03RestoreDog.png
explorer.exe .
```

### Explanation

I found the Vim swap file and encryption key in `Doguments`. I used `vim -r` to recover the crashed session. After troubleshooting the unreadable output, I obtained the hidden PNG path and downloaded the image using SCP.

**Result:** Successfully recovered the image path and completed the challenge.

## 07. 02MuteDog

**Goal:** Find the hidden PNG path by analyzing the output streams of an executable program.

### 1. Find and run the program

```
cd /home/ctf/go_to_opt
ls -l Programs/mute_dog
./Programs/mute_dog
```

The program displayed ASCII art with the message `Terminals are listening`.

### 2. Separate stdout and stderr

```
./Programs/mute_dog > /tmp/02MuteDog.out 2> /tmp/02MuteDog.err
file /tmp/02MuteDog.out /tmp/02MuteDog.err
ls -lh /tmp/02MuteDog.*
```

The output was separated into two files:

- `stdout` — 29 bytes
    
- `stderr` — 21 bytes
    

### 3. Read both output files

```
cat /tmp/02MuteDog.out
echo
cat /tmp/02MuteDog.err
echo
```

**stdout:**

```
/regular_paw_storage/e8654d92
```

**stderr:**

```
6d88d75c/Mute_dog.png
```

Combined path:

```
/regular_paw_storage/e8654d926d88d75c/Mute_dog.png
```

### 4. Download using Ubuntu WSL

```
cd /home/franxx/fctf01
scp -O 'o.korchagin-dog@osctf.nnk.moe:/regular_paw_storage/e8654d926d88d75c/Mute_dog.png' ./02MuteDog.png
```

### 5. Verify the image

```
file 02MuteDog.png
ls -lh 02MuteDog.png
explorer.exe .
```

### Explanation

I found and executed `mute_dog`. The program displayed ASCII art, but its hidden output was divided between `stdout` and `stderr`. I redirected both streams into separate files, read their contents, and combined the two parts to recover the PNG path.

**Result:** Successfully recovered the complete image path for the challenge.


## 08. 02PetDog

**Goal:** Restore the original mascot image from Git history and assemble the PNG from three binary parts.

### 1. Find the project
 

```bash

cd /home/ctf/go_to_opt

ls -la Doguments/auto_petter

cat Doguments/auto_petter/README.md

```

  The `auto_petter` directory contains a Git repository. The README says that `mascot.png` is assembled from files in `frames/`.
### 2. Inspect the assembly script and Git history


```bash

cd /home/ctf/go_to_opt/Doguments/auto_petter

cat tools/assemble_mascot.sh

ls -lah frames/

git log --oneline --all -n 15

```

The script assembles the parts in this order: `part1.bin`, `part3.bin`, `part2.bin`. The current `part2.bin` and `part3.bin` are only 88 bytes each.

I first tried assembling the current parts:

```bash

cat frames/part1.bin frames/part3.bin frames/part2.bin > mascot.png

```
### 3. Check changes to each frame
 
```bash

git log --all --oneline -- frames/part2.bin

git log --all --oneline -- frames/part3.bin

git log --all --oneline -- frames/part1.bin

```

The history shows that `part2.bin` and `part3.bin` were replaced with stubs, while `part1.bin` was later repaired.
### 4. Compare frame sizes in Git

```bash

git cat-file -s HEAD:frames/part2.bin

git cat-file -s f3a0f3d:frames/part2.bin

git cat-file -s ff758a1:frames/part2.bin

  

git cat-file -s HEAD:frames/part3.bin

git cat-file -s d728ce3:frames/part3.bin

git cat-file -s ff758a1:frames/part3.bin

```

Results for both `part2.bin` and `part3.bin`: current version **88 bytes**, intermediate version **104 bytes**, original version **38,152 bytes**.
### 5. Recover the frame data

```bash

git show ff758a1:frames/part2.bin > /tmp/part2.bin

git show ff758a1:frames/part3.bin > /tmp/part3.bin

git show HEAD:frames/part1.bin > /tmp/part1.bin

ls -lh /tmp/part*.bin

file /tmp/part*.bin

```

I recovered the original `part2` and `part3` from commit `ff758a1` and kept the repaired `part1` from `HEAD`. Using `/tmp` avoids changing the tracked files.
### 6. Assemble and verify the PNG
  

```bash

cat /tmp/part1.bin /tmp/part3.bin /tmp/part2.bin > /tmp/02PetDog.png

file /tmp/02PetDog.png

ls -lh /tmp/02PetDog.png

```

**Output:** PNG image, **300 × 200** pixels, approximately **112 KB**.

### 7. Download to Ubuntu WSL

Run these commands **on the local Ubuntu WSL terminal**, not on the CTF server:

```bash

cd /home/franxx/fctf01

scp -O 'o.korchagin-dog@osctf.nnk.moe:/tmp/02PetDog.png' ./02PetDog.png

file 02PetDog.png

ls -lh 02PetDog.png

explorer.exe .

```
### Explanation

I inspected the assembly script and used Git history to find why the mascot could not be assembled from the current files. I recovered the original frame parts with `git show`, combined them in the order specified by the script, and checked the resulting PNG with `file`.

**Result:** Reconstructed the mascot as `02PetDog.png` (300 × 200, approximately 112 KB) and prepared it for submission.

## 09. 02RemDog

**Goal:** Find and remove all angry dogs from a directory to complete the challenge.

### 1. Inspect the directory

```bash
cd /home/ctf/go_to_opt
ls -la Doguments/unsorted | head -n 20
cat Doguments/unsorted/README
find Doguments/unsorted -maxdepth 1 -type f | wc -l
```

**Output:** 33 files, including `README`, which says to remove all angry dogs.

### 2. Compare the ASCII images

```bash
cat Doguments/unsorted/01dc562b
cat Doguments/unsorted/17584c76
```

Found two different ASCII images:
- `01dc562b` — normal dog (1678 bytes).
- `17584c76` — angry dog (1937 bytes).

### 3. Identify duplicate files using SHA-256

```bash
sha256sum Doguments/unsorted/[0-9a-f]* | cut -d' ' -f1 | sort | uniq -c
```

**Output:**

```text
14 815468f08ef9cdba00adf526749e6242ddeaa866a4401a7cdae636893a6855af
18 9ab2e1e0c4420d8044612d0639df69fdcb6e474ad31d020b7e18319a4de1a45f
```

Compare hashes of both images:

```bash
sha256sum Doguments/unsorted/01dc562b Doguments/unsorted/17584c76
```

Identified 14 normal dogs and 18 angry dogs.

### 4. Remove angry dogs

Create a backup directory:

```bash
mkdir -p /tmp/02RemDog_backup
```

Move all 1937-byte files:

```bash
find Doguments/unsorted -maxdepth 1 -type f -size 1937c -exec mv -t /tmp/02RemDog_backup -- {} +
```

**Server output:**

```text
Woof! You have solved Rem_dog!

Here's your dog pic: /regular_paw_storage/613ded30f597edb3/Rem_dog.png
```

### 5. Verify the result

```bash
ls -1 /tmp/02RemDog_backup | wc -l
find Doguments/unsorted -maxdepth 1 -type f | wc -l
```

**Output:**

```text
18
15
```

Successfully moved 18 angry dogs. The original directory contains 14 normal dogs and `README`.

### 6. Download using Ubuntu WSL

```bash
cd /home/franxx/fctf01
scp -O 'o.korchagin-dog@osctf.nnk.moe:/regular_paw_storage/613ded30f597edb3/Rem_dog.png' ./02RemDog.png
```

Verify the downloaded image:

```bash
file 02RemDog.png
ls -lh 02RemDog.png
explorer.exe .
```

### Explanation

I inspected the directory and compared two ASCII dog images. I used `sha256sum` to identify duplicate files and found 18 angry dogs. Then I used `find` to move these files into a backup directory. The server automatically confirmed the solution and provided the PNG path.

**Result:** Successfully removed all 18 angry dogs and completed the challenge.


## 10. 01CatDog

**Goal:** Combine two image fragments into a valid PNG file.

### 1. Find the image parts

```bash
cd /home/ctf/go_to_opt
ls -lah Pictures/
file Pictures/*
```

**Output:** Found two files:
- `Cat_dog_top` — PNG header, 223 × 300 pixels.
- `Cat_dog_bottom` — remaining image data.

### 2. Reassemble the image

```bash
cat Pictures/Cat_dog_top Pictures/Cat_dog_bottom > /tmp/01CatDog.png
```

The `cat` command combines both fragments. The part containing the PNG header must come first.

### 3. Verify the image

```bash
file /tmp/01CatDog.png
ls -lh /tmp/01CatDog.png
```

**Output:** PNG image, 223 × 300 pixels, approximately 52 KB.

### 4. Download using Ubuntu WSL

```bash
cd /home/franxx/fctf01
scp -O 'o.korchagin-dog@osctf.nnk.moe:/tmp/01CatDog.png' ./01CatDog.png
```

### 5. Verify the downloaded file

```bash
file 01CatDog.png
ls -lh 01CatDog.png
explorer.exe .
```

### Explanation

I found two image fragments using `ls` and checked their formats with `file`. Since `Cat_dog_top` contained the PNG header, I placed it first and used `cat` to combine the fragments. I verified the resulting PNG and downloaded it using SCP.

**Result:** Successfully reconstructed and downloaded the PNG image. Challenge completed.

## 11. 01TailheadedDog

**Goal:** Find a hidden PNG image path at the beginning and end of a large log file.

### 1. Find and inspect the log file

```bash
cd /home/ctf/go_to_opt
ls -lah Downloads/
file Downloads/*
```

**Output:** Found `dog.log`, a UTF-8 text file of approximately 1.4 MB.

### 2. Read the beginning of the file

```bash
head -c 240 Downloads/dog.log
```

Found the first part of the image path:

```text
/regular_paw_storage/2c1d7145
```

### 3. Read the end of the file

```bash
tail -c 240 Downloads/dog.log
tail -n 30 Downloads/dog.log
```

The first command showed only ASCII art. The second command revealed the missing part:

```text
831a4064/tailheaded.png
```

### 4. Combine the image path

Combined both parts into the full path:

```text
/regular_paw_storage/2c1d7145831a4064/tailheaded.png
```

### 5. Download using Ubuntu WSL

```bash
cd /home/franxx/fctf01
scp -O 'o.korchagin-dog@osctf.nnk.moe:/regular_paw_storage/2c1d7145831a4064/tailheaded.png' ./01TailheadedDog.png
```

### 6. Verify the downloaded image

```bash
file 01TailheadedDog.png
ls -lh 01TailheadedDog.png
explorer.exe .
```

### Explanation

I inspected the large log file using `head` and `tail`. The beginning contained the first part of the image path, while the last 30 lines contained the second part. I combined them to get the full path and used SCP to download the image.

**Result:** Successfully recovered the complete PNG path and prepared the image for submission.



