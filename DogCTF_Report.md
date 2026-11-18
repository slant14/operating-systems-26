# CTF Dog Challenge Report

**Bash CTF (dogctf), instance findingdory — 11 of 22 flags accepted (1100 points).**

## How to connect

The bot provides the login and password. Connect like this:

```bash
ssh n.denisov-dog@osctf.nnk.moe
```

After logging in, you're usually in `/home/ctf/go_to_opt`. If not, `cd` there.

It's more convenient to download files from another terminal via `scp`.

---

## 01PoweredDog

`/home/dog/Downloads` has a pile of small files, plus `start` and `end`.
`start` says `Next: file_....txt`, followed by a chunk of PNG.
Had to follow the chain and concatenate all the `Content` pieces.

At the end, `end` contained the path:

```
/regular_paw_storage/d3ea8e4a6da0c3c0/powered_dog.png
```

Just downloaded it.

```bash
scp n.denisov-dog@osctf.nnk.moe:/regular_paw_storage/d3ea8e4a6da0c3c0/powered_dog.png ~/powered_dog.png
```

## 01TailheadedDog

`Downloads/dog.log` held two path fragments and the hints "more at the end" / "more at the start".

They were:
- `/regular_paw_storage/ea0fd60e`
- `c46e32c1/tailheaded.png`

Concatenated them and got:

```bash
ls /regular_paw_storage/ea0fd60ec46e32c1/tailheaded.png
```

Worked.

## 01CatDog

`Pictures` had `Cat_dog_top` and `Cat_dog_bottom`.
The top part is a normal PNG, the bottom is just data.

```bash
cat Pictures/Cat_dog_top Pictures/Cat_dog_bottom > /tmp/catdog.png
file /tmp/catdog.png
```

Got a normal 223x300 PNG.

## 01CompileDog

`Programs/compile_dog.c` is some strange C interpreter.
Compiled and ran it:

```bash
gcc Programs/compile_dog.c -o /tmp/compile_dog
/tmp/compile_dog
```

It printed the path:

```
/regular_paw_storage/48aaa6e345c90e91/Compile_dog.png
```

## 02MuteDog

Ran `Programs/mute_dog`. It printed path fragments.
Concatenated `d6e71463` + `11359af1` and got:

```
/regular_paw_storage/d6e7146311359af1/Mute_dog.png
```

## 01BackgroundDog

The `background_dog` process was already running and stopped.
Did `fg` and it started spamming the path:

```
/regular_paw_storage/13549649f1af03ce/Background_dog.png
```

Then stopped it again.

## 01HistoryDog

Checked `.bash_history`. It had this command:

```bash
cp /super_secret_doggo_vault/grandpaw.png /regular_paw_storage/042a1edcb51e3b06/grandpaw.png
```

Just grabbed that file.

## 02ScatteredDog

`Videos/Scattered dog` has files named `-50`, `-49`, ..., `49`.
Plain `cat *` didn't work because of the leading minus signs.

Sorted them numerically:

```bash
cd "Videos/Scattered dog"
ls | sort -n | while read f; do cat "./$f"; done > /tmp/scattered.png
```

Got a 300x195 PNG.

## 03UsedDog

The simplest one. The file was already there:

```bash
cp /home/dog/Used_dog.png /tmp/used_dog.png
file /tmp/used_dog.png
```

A normal PNG.

## 01EditDog

`Doguments` had `crashed.txt` and a vim swap file.
The swap file said the file was in `secret_stuff/super_secret`, with a 4-letter name made of D/O/G, one letter repeated.

The folder couldn't be listed (`ls` didn't work), so brute-forced the names:

```bash
cd Doguments/secret_stuff/super_secret
ls GDOG.png   # worked
```

Found `GDOG.png`.

## 02ShyDog

`Videos/Shy dog` had 1000 files. Almost none of them were PNGs.
Searched by magic bytes:

```bash
cd "Videos/Shy dog"
for f in *; do
  if head -c 8 "$f" | grep -q PNG; then
    echo "FOUND: $f"
    break
  fi
done
```

Found `Gallery_Maybe_Dog_c1f00ac4.png`.

---

## Summary

| Challenge | Approach | File |
|---------|-----------|------|
| PoweredDog | Chain of files in Downloads | powered_dog.png |
| TailheadedDog | Concatenated path fragments from the log | tailheaded.png |
| CatDog | cat top + bottom | catdog.png |
| CompileDog | Compiled and ran it | Compile_dog.png |
| MuteDog | Ran the binary, concatenated the path | Mute_dog.png |
| BackgroundDog | fg on the backgrounded process | Background_dog.png |
| HistoryDog | .bash_history | grandpaw.png |
| ScatteredDog | sort -n + cat | scattered.png |
| UsedDog | Just copied it | Used_dog.png |
| EditDog | Brute-forced the name from the hint | GDOG.png |
| ShyDog | Searched for a PNG among 1000 files | Gallery_Maybe_Dog_...png |

**Total: 1100 points.**

Most challenges came down to finding the path to a PNG or concatenating fragments.
The most tedious were Tailheaded (path fragments) and Shy (1000 files).
The simplest were Used and History.
