n.ionov@innopolis.university CSE-04

# DogCTF

## 01CatDog

In `Pictures` there were two files of the same size. `file` showed that
`Cat_dog_top` starts with the PNG signature, and `Cat_dog_bottom` is plain
data. I joined them in that order and the result opened as a normal image.

```bash
cd /home/ctf/go_to_opt/Pictures
cat Cat_dog_top Cat_dog_bottom > /tmp/Cat_dog.png
file /tmp/Cat_dog.png
```

Flag: `/tmp/Cat_dog.png`

## 01CompileDog

The source in `Programs` does not contain the path as a string. It builds the
path at run time and prints it, so I compiled and ran it.

```bash
cd /home/ctf/go_to_opt/Programs
cc compile_dog.c -o /tmp/compile_dog && /tmp/compile_dog
```

Flag: `/regular_paw_storage/40f6b61becb075e0/Compile_dog.png`

## 01HelloDog

The image was in the start folder. The name contains a colon, so I kept it in
quotes and used `realpath` to get the absolute path for `scp`.

```bash
cd /home/ctf/go_to_opt
realpath 'hoi_:3.png'
```

Flag: `/home/ctf/go_to_opt/hoi_:3.png`

## 01TailheadedDog

`dog.log` is a large log. The first half of the folder name is at the beginning
and the second half is at the end, so I looked at both ends and joined the
parts `b855b0fa` and `58fd644f` without a slash.

```bash
cd /home/ctf/go_to_opt/Downloads
head -c 200 dog.log
tail -c 300 dog.log | strings
```

Flag: `/regular_paw_storage/b855b0fa58fd644f/tailheaded.png`

## 02MuteDog

The program prints a scene and the path, but when I redirected the output
everything went into one line without line breaks.

```bash
cd /home/ctf/go_to_opt/Programs
./mute_dog > /tmp/mute.out 2>&1
strings -a /tmp/mute.out | tail -n 20
```

The text I found was `8dadc132/Mute_dog.png/regular_paw_storage/de74141a`.
This is the real path rotated: the first half (`8dadc132`) stands before the
PNG name, the second half (`de74141a`) after `/regular_paw_storage/`.

Flag: `/regular_paw_storage/de74141a8dadc132/Mute_dog.png`

## 02PetDog

The mascot was split into three files in the Git history of `auto_petter`. The
assembly script reads `part3`, `part1`, `part2` in this order, and `part3`
contains the PNG header. One version of `part3` is broken, so I checked the
first bytes before choosing the commit.

```bash
cd /home/ctf/go_to_opt/Doguments/auto_petter
git log --oneline --all -- frames
git show a924ab4:frames/part3.bin | od -An -tx1 -N16
( git show a924ab4:frames/part3.bin
  git show 0e7b633:frames/part1.bin
  git show e649d2f:frames/part2.bin ) > /tmp/mascot.png
```

`git show <commit>:<path>` reads the file from the commit, not from the working
tree, so the unfinished rebase in the repository does not matter.

Flag: `/tmp/mascot.png`

## 02ScatteredDog

The folder had 100 files named from `-50` to `49`. The file `-50` starts with
the PNG signature, so the files are pieces of one image in numeric order.

```bash
cd '/home/ctf/go_to_opt/Videos/Scattered dog'
file -- ./-50
( for i in $(seq -50 49); do cat -- "./$i" || exit 1; done ) > /tmp/Scattered.png
```

The `--` and `./` are needed because the names begin with a dash and would
otherwise be read as options. Sorting by name would give `-1, -10, -11`, so I
used `seq` to keep the numeric order.

Flag: `/tmp/Scattered.png`

## 02ShyDog

The folder contains hundreds of images, but most are decoys with a fake
extension.

```bash
cd '/home/ctf/go_to_opt/Videos/Shy dog'
file -- *Maybe_Dog* | grep 'PNG image data'
```

`file` checks the magic bytes, so it found the only real PNG among the
candidates.

Flag: `Backup_Maybe_Dog_d09de499.png`

## 03ChainedDog

Every file in `/home/dog/Downloads` points to the next one and carries a piece
of the picture: a line `Next: <name>`, then `Content: ` and raw PNG bytes.

```bash
cd /home/dog/Downloads
od -An -tc -N80 start
```

```bash
( name=start
  while :; do
    read -r line < "$name" || exit 1
    next=${line#Next: }
    off=$((${#line} + 1))
    tail -c "+$((off + 10))" "$name" || exit 1
    [ "$name" = end ] && break
    name=$next
  done ) > /tmp/Chained.png
```

`tail -c "+N"` prints from byte `N`, and `off + 10` skips the newline and the
nine characters of `Content: ` after the first line. The chain ends with a file
that points to itself, so the loop stops at `end`. I read bytes, not lines,
because the PNG data contains `\n`.

Flag: `/tmp/Chained.png`

## 03InsecureDog

The note in `secret_stuff` says the name has four letters from D, O and G, and
one letter repeats. The answer is `OOGD`. The folder is protected by
`chmod 311`: it can not be listed, but a known name inside it can still be
opened.

```bash
cd /home/ctf/go_to_opt/Doguments
cat secret_stuff/remember.txt
file secret_stuff/super_secret/OOGD.png
```

Flag: `/home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/OOGD.png`

## 03RestoreDog

`crashed.txt` says that Vim crashed while writing the path, and the hidden
`.crashed.txt.swp` is the swap file. Vim asks for the key at start, so the key
must be typed into the `Enter encryption key:` prompt before pressing Enter.
After recovery `:set key=` removes the encryption, and `:w!` writes a plain
copy.

```bash
cd /home/ctf/go_to_opt/Doguments
cat crashed.txt
vim -r .crashed.txt.swp
```

```vim
:set key=
:w! /tmp/restore-notes.txt
:q!
```

The notes contained the path to the picture.

Flag: `/regular_paw_storage/36ngswy/Restore_dog.png`

## 03SpyDog

The README mentions a camera, but the frames are not in the working tree. The
commit `133d6b7` is unreachable, and its tree still has the deleted frame.

```bash
cd /home/dog/Doguments/cat_tracker
git fsck --full --no-reflogs --unreachable
git ls-tree -r --name-only 133d6b7
git show 133d6b7:cat_cam/frame_0054.png > /tmp/frame_0054.png
```

Flag: `/tmp/frame_0054.png`

## 03UsedDog

The challenge folder is `/home/ctf/go_to_opt`. One level up there is a second
home folder with the picture.

```bash
ls -l /home/dog
```

Flag: `/home/dog/Used_dog.png`
