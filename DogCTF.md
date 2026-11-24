i.birkina@innopolis.university CSE-02

# CTF - Dog Finding Report

## 1. CompileDog

First, I found a C program called `compile_dog.c` in the Programs directory.

**Commands:**

```bash
cd ~/go_to_opt/Programs
ls -la
gcc compile_dog.c -o compile_dog
./compile_dog
```

I used gcc to compile the C program and then executed it.

The program printed the path to a dog picture:

```
/regular_paw_storage/4442ded3407b629e/Compile_dog.png
```

I downloaded the picture to my Mac using scp. In another terminal

## 2. MuteDog

I found an executable file called `mute_dog`. It did not have read permission, but I could execute it.

**Commands:**

```bash
file mute_dog
./mute_dog
echo $?
./mute_dog > /tmp/mute_out.txt 2> /tmp/mute_err.txt
wc -c /tmp/mute_out.txt /tmp/mute_err.txt
cat /tmp/mute_out.txt
cat /tmp/mute_err.txt
```

The program displayed an ASCII picture. I discovered that it also printed information to two different output streams.

I redirected standard output and standard error into separate files using `>` and `2>`.

The first file contained 29 bytes and the second contained 21 bytes. Together, they gave the path:

```
/regular_paw_storage/a84b6b93756975a1/Mute_dog.png
```

Then I downloaded the picture.

## 3. SpitoonDog

I found another executable program named `spitoon`.

**Commands:**

```bash
file spitoon
ls -l spitoon
./spitoon
chmod u+r spitoon
strings spitoon | grep -Ei 'tough|nails|milk|dog|png|password|salty|kid'
```

The program asked, "How tough are ya?"

I tried answering the question, but my answers were rejected.

I noticed that the file had execute permission but no read permission. I added read permission using `chmod u+r`.

Then I used `strings` and `grep` to find readable text inside the executable.

This revealed the picture:

```
/regular_paw_storage/4aef356c039cb89a/Spitoon_dog.png
```

## 4. CatDog

I found two files called `Cat_dog_top` and `Cat_dog_bottom`. They appeared to be parts of one image.

**Commands:**

```bash
cd ~/go_to_opt
file Pictures/Cat_dog_top Pictures/Cat_dog_bottom
ls -lh Pictures/
cat Pictures/Cat_dog_top Pictures/Cat_dog_bottom > Cat_dog.png
file Cat_dog.png
```

The first file was recognized as PNG data, while the second was recognized as ordinary data.

I used `cat` to join both parts into one file.

After combining them, `file` recognized `Cat_dog.png` as a PNG image with resolution 223 × 300.

I downloaded the completed image.

## 5. ShyDog

In the Videos/Shy dog directory, there were hundreds of image files.

I noticed that some filenames contained `Maybe_Dog`, so I searched only for those files.

**Commands:**

```bash
cd ~/go_to_opt/'Videos/Shy dog'
find . -type f -name '*Maybe_Dog*' | sort
ls -lh *Maybe_Dog*
file *Maybe_Dog*
```

The first command found 31 possible dog pictures.

I checked their sizes, but they were similar. Then I used `file` to identify their real formats.

Only one candidate was recognized as a valid PNG image:

```
Photo_Maybe_Dog_e9178921.png
```

Its resolution was 150 × 200.

I downloaded this file.

## 6. ScatteredDog

I found a directory containing 100 files named from -5
0 to 49.

I suspected that these files were pieces of one image.

**Commands:**

```bash
cd ~/go_to_opt/'Videos/Scattered dog'
file ./-50 ./-1 ./0 ./1 ./49
for i in $(seq -50 49); do cat -- "./$i"; done > ../Scattered_dog.png
file ../Scattered_dog.png
ls -lh ../Scattered_dog.png
```

The file command showed that -50 contained a PNG header, while the other checked files were just data.

I used a for loop and cat to combine all 100 files in numerical order.

The result was `Scattered_dog.png`, a PNG image with resolution 300 × 195.

I downloaded the assembled picture.

## 7. TailheadedDog

I found a large log file named `dog.log` in the Downloads directory.

**Commands:**

```bash
cd ~/go_to_opt/Downloads
ls -lh dog.log
file dog.log
wc -l dog.log
head -n 15 dog.log
tail -n 15 dog.log
```

The file contained 17,019 lines.

Instead of reading the whole file, I used head to see the first 15 lines and tail to see the last 15 lines.

The beginning contained:

```
/regular_paw_storage/b1d9d914
```

The end contained:

```
3705df2c/tailheaded.png
```

I combined these two strings to get the full path:

```
/regular_paw_storage/b1d9d9143705df2c/tailheaded.png
```

Then I downloaded the image.

## 8. HistoryDog

For this task, I used the Linux command history.

**Commands:**

```bash
history | tail -n 30
history | head -n 30
```

First, I checked the last 30 commands, but they only contained my recent actions.

Then I checked the beginning of the history.

I found this command:

```bash
cp /super_secret_doggo_vault/grandpaw.png /regular_paw_storage/78413a62e3e71631/grandpaw.png
```

It showed where a dog picture had been copied.

I used the destination path to download `grandpaw.png`.

## 9. BackgroundDog

I discovered a stopped background process.

**Commands:**

```bash
ps aux | grep dog
jobs -l
ls -la /tmp/bgdog
file /tmp/bgdog
bg %1
```

The ps command showed a process called `background_dog` with status T, which means stopped.

I used `jobs -l` to see the background jobs. The stopped process was job number 1.

The file `/tmp/bgdog` did not exist.

I used `bg %1` to resume the stopped job in the background.

After that, the program printed:

```
/regular_paw_storage/e8f92947bf1db906/Background_dog.png
```

I downloaded the picture.

To stop the background process, I used `kill %1`.

## 10. AutoPetter Mascot Recovery

I found a Git project named `auto_petter`.

The README explained that a mascot image could be assembled from three binary files.

**Commands:**

```bash
cd ~/go_to_opt/Doguments/auto_petter
ls -la
cat README.md
cat tools/assemble_mascot.sh
ls -lh frames/
git log --oneline -10
git log --all --oneline -- frames/
git branch -a
git status --short
```

The Git history showed that some image parts had been replaced with incomplete versions.

I used git show to recover older versions of the files without changing the repository.

**Recovery commands:**

```bash
mkdir -p /tmp/dog_recovery
git show 69ce094:frames/part1.bin > /tmp/dog_recovery/part1.bin
git show 69ce094:frames/part2.bin > /tmp/dog_recovery/part2.bin
git show 15a2640:frames/part3.bin > /tmp/dog_recovery/part3.bin
```

Then I combined the parts in the order specified by the assembly script:

```bash
cat /tmp/dog_recovery/part3.bin /tmp/dog_recovery/part1.bin /tmp/dog_recovery/part2.bin > /tmp/dog_recovery/mascot.png
file /tmp/dog_recovery/mascot.png
ls -lh /tmp/dog_recovery/mascot.png
```

The resulting file was recognized as a 300 × 200 PNG image and had a size of 112 KB.

## 11. RestoreDog

In the Doguments directory, I found a file named `crashed.txt` and a hidden Vim swap file called `.crashed.txt.swp`.

**Commands:**

```bash
cd ~/go_to_opt/Doguments
ls -la
cat crashed.txt
strings .crashed.txt.swp | grep -Ei 'dog|png|storage|paw'
vim -r crashed.txt
```

The text file explained that Vim had crashed while someone was writing down the location of a hidden dog picture.

It also contained an encryption key:

```
5591d531e0c7ee78
```

I used `vim -r crashed.txt` to recover the lost text from the swap file.

Vim asked for an encryption key. I entered the key from crashed.txt.

The recovered file contained:

```
/regular_paw_storage/z3px305/Restore_dog.png
```

I used this path to download the dog picture.
