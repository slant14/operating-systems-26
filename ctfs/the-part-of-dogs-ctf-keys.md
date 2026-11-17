# DogCTF

## 01BackgroundDog

The `.bash_profile` file starts this program in the background when I log in.

```bash
echo 'suppa_seeccreet_kiy' > /tmp/bgdog
/usr/local/bin/background_dog /tmp/bgdog &
```

The program stops because it tries to use the terminal while it is in the background. I bring it back to the foreground so it can use the terminal.

```bash
jobs -l
fg %1
```

The program shows this path.

```text
/regular_paw_storage/6d85bcd6f79e8125/Background_dog.png
```

I can press `Ctrl+C` to stop the program after I get the path.

## 01CatDog

The `Pictures` folder has two parts of one PNG file. I join them to make the full picture.

```bash
cd /home/ctf/go_to_opt/Pictures
file Cat_dog_top Cat_dog_bottom
cat Cat_dog_top Cat_dog_bottom > /tmp/Cat_dog.png
file /tmp/Cat_dog.png
```

These are the paths to the two parts.

```text
/home/ctf/go_to_opt/Pictures/Cat_dog_top
/home/ctf/go_to_opt/Pictures/Cat_dog_bottom
```

## 01CompileDog

The file `Programs/compile_dog.c` has a small virtual machine. I build the program with `gcc` and run it.

```bash
cd /home/ctf/go_to_opt/Programs
gcc compile_dog.c -o compile_dog
./compile_dog
```

The program shows this path.

```text
/regular_paw_storage/6b7064ddfd846b32/Compile_dog.png
```

## 01EditDog

The file `/home/ctf/go_to_opt/Doguments/EDITME` asks me to make the text drawing of the dog 10 characters longer. I only edit the drawing between the marked lines.

1. I add 10 underscores to each of the two long parts of the dog's body.
2. I move the head and the right paws 10 spaces to the right.
3. I leave the title, the marked lines, and the name `jgs` as they are.

I save the file and run the checker.

```bash
/usr/local/bin/checker_dog check
```

The checker shows this path.

```text
/regular_paw_storage/13dc46c2b0a20333/Edit_dog.png
```

## 01HelloDog

The picture is already in the starting folder. I use `file` to check it.

```bash
cd /home/ctf/go_to_opt
file hoi_:3.png
```

This is the path to the picture.

```text
/home/ctf/go_to_opt/hoi_:3.png
```

## 03SpyDog

The picture is gone from the current Git branch. Git still has a copy. I look for old commits that are outside the current branches.

```bash
cd /home/dog/Doguments/cat_tracker
git fsck --full --no-reflogs --unreachable 2>/dev/null
```

I list the files in commit `5775250`.

```bash
git ls-tree -r --name-only 5775250
```

I find `cat_cam/frame_0044.png` and save a copy in `/tmp`.

```bash
git show 5775250:cat_cam/frame_0044.png > /tmp/HistoryDog.png
file /tmp/HistoryDog.png
```

This is the path in Git.

```text
5775250:cat_cam/frame_0044.png
```

## 03InsecureDog

The folder has permissions `311`. I can open a file if I know its name. To list all the files, I give the folder owner read access for a short time. Then I set the permissions back to `311`.

```bash
d=/home/ctf/go_to_opt/Doguments/secret_stuff/super_secret
chmod u+r "$d"
ls -la "$d"
chmod 311 "$d"
```

This is the path to the picture.

```text
/home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/GODO.png
```

## 01TailheadedDog

The first part of the path is at the start of a long log file. The second part is at the end.

```bash
head -20 /home/ctf/go_to_opt/Downloads/dog.log
tail -20 /home/ctf/go_to_opt/Downloads/dog.log
```

I join `/regular_paw_storage/debb091f` and `a48065a8/tailheaded.png` to get the full path.

```text
/regular_paw_storage/debb091fa48065a8/tailheaded.png
```

## 02MuteDog

The program sends the first part of the path to normal output (`stdout`). It sends the second part to error output (`stderr`). I save the two parts in separate files.

```bash
cd /home/ctf/go_to_opt/Programs
./mute_dog > /tmp/mute.out 2> /tmp/mute.err
cat /tmp/mute.out
cat /tmp/mute.err
```

I join the two parts to get this path.

```text
/regular_paw_storage/3b82ac3436c95664/Mute_dog.png
```

## 02PetDog

The folder `/home/ctf/go_to_opt/Doguments/auto_petter` has a Git repository and a script called `tools/assemble_mascot.sh`. The script shows the order of the parts. It uses `part3` first, then `part1`, then `part2`.

The current `part1.bin` file is broken and has only 88 bytes. I use a good copy from the old commit `d25c430`.

```bash
cd /home/ctf/go_to_opt/Doguments/auto_petter
wc -c frames/*
git log --oneline --all
{
  cat frames/part3.bin
  git show d25c430:frames/part1.bin
  cat frames/part2.bin
} > /tmp/PetDog.png
file /tmp/PetDog.png
```

The PNG is 300 pixels wide and 200 pixels high.

## 02RemDog

The `Doguments/unsorted` folder has two groups of text drawings. I use the file size to tell the groups apart.

```bash
cd /home/ctf/go_to_opt/Doguments/unsorted
find . -maxdepth 1 -type f -printf '%s %f\n' | sort
cat 093e2a55
cat 0b140ef8
```

The files that are 1937 bytes show the angry dog. I delete only these files and run the checker.

```bash
find . -maxdepth 1 -type f -size 1937c -delete
/usr/local/bin/checker_dog check
```

The checker shows this path.

```text
/regular_paw_storage/8f9550b9e66002f7/Rem_dog.png
```

## 02ScatteredDog

The picture is in parts. The file names are numbers from `-50` to `49`. I join the files in that order.

```bash
cd '/home/ctf/go_to_opt/Videos/Scattered dog'
for i in $(seq -50 49); do
  cat -- "$i"
done > /tmp/ScatteredDog.png
file /tmp/ScatteredDog.png
```

The PNG is 300 pixels wide and 195 pixels high.

## 02ShyDog

Many files in this folder have the wrong endings in their names. I use `file` to check the real file types.

```bash
cd '/home/ctf/go_to_opt/Videos/Shy dog'
file -- ./* | grep 'PNG image'
```

This is the only real PNG.

```text
/home/ctf/go_to_opt/Videos/Shy dog/Backup_Maybe_Dog_87ff754c.png
```

## 02SpitoonDog

I read the help page for `spitoon` and run the program.

```bash
MANPAGER=cat man spitoon
cd /home/ctf/go_to_opt/Programs
./spitoon
```

I give this answer to the first question.

```text
How tough am I? How tough am I?! I ran 75 of linux programs this lab
```

When the program says `Yea, so?`, I type this line and leave it on the screen.

```text
Without 75 errors
```

I press `Ctrl+C` right away, before pressing Enter. The program gets the `SIGINT` signal and shows this path.

```text
/regular_paw_storage/4adb966dc73b1e12/Spitoon_dog.png
```

## 03RestoreDog

Vim has a locked recovery file. The key is in `crashed.txt`.

```bash
cat /home/ctf/go_to_opt/Doguments/crashed.txt
```

This is the key.

```text
d1e73a1e47da6d43
```

I use Vim to get the text back.

```bash
cd /home/ctf/go_to_opt
vim -r Doguments/crashed.txt
```

I enter the key. Vim shows the recovered text with this path.

```text
/regular_paw_storage/zv6cm92/Restore_dog.png
```

I type `:q!` to close Vim and keep the original file as it is.
