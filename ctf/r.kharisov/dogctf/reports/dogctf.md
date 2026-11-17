# Finding Dory — Bash CTF solutions

Author: Robert Kharisov
Session: 6–7 November 2026

Commands run inside the assigned SSH container unless marked as local. Paths, ports and Git object IDs below are from my session and can change after a reset.

## 01BackgroundDog

I resumed the stopped shell job to reveal the image path, then suspended it after recording the output.

```bash
jobs
bg %1
jobs
```

```bash
kill -STOP %1
ls -l /regular_paw_storage/e32f1de339bd5180/Background_dog.png
```

```bash
kill -STOP %1
ls -l /regular_paw_storage/e32f1de339bd5180/Background_dog.png
```

**Result:** `/regular_paw_storage/e32f1de339bd5180/Background_dog.png`. Image: `Background_dog.png`.

## 01CatDog

I concatenated the two image fragments in top-to-bottom order.

```bash
cd /home/ctf/go_to_opt/Pictures
ls -la
cat Cat_dog_top Cat_dog_bottom > Cat_dog.png
file Cat_dog.png
pwd
```

**Result:** `/home/ctf/go_to_opt/Pictures/Cat_dog.png`. Image: `Cat_dog.png`.

## 01CompileDog

I compiled the provided C source and ran the executable to obtain the image path.

```bash
cd /home/ctf/go_to_opt/Programs
ls -la
gcc compile_dog.c -o compile_dog
./compile_dog
```

**Result:** `/regular_paw_storage/15da32c373850161/Compile_dog.png`. Image: `Compile_dog.png`.

## 01EditDog

I extended the ASCII dog by four characters, preserving its shape and the text outside the safety lines. The exact final drawing was not recorded.

```bash
cd /home/ctf/go_to_opt/Doguments
ls -la
cat EDITME
nano EDITME
```

**Result:** `/regular_paw_storage/646ba1d455904936/Edit_dog.png`. Image: `Edit_dog.png`.

## 01HelloDog

I found the image in the challenge home directory and checked its format.

```bash
file /home/ctf/go_to_opt/hoi_:3.png
```

**Result:** `/home/ctf/go_to_opt/hoi_:3.png`. Image: `hoi_:3.png`.

## 01HistoryDog

I inspected shell history. An earlier copy command disclosed the image destination.

```bash
history
```

```bash
cp /super_secret_doggo_vault/grandpaw.png /regular_paw_storage/72e09810ecd24c63/grandpaw.png
```

**Result:** `/regular_paw_storage/72e09810ecd24c63/grandpaw.png`. Image: `grandpaw.png`.

## 01PoweredDog

I read the README in `/opt`; its output disclosed the image path.

```bash
cat /opt/README.md
cat /opt/README.md
```

**Result:** `/regular_paw_storage/59fee80d8a12c2f/powered_dog.png`. Image: `powered_dog.png`.

## 01TailheadedDog

I joined the path fragment at the beginning of the log with the fragment near its end.

```bash
cd /home/ctf/go_to_opt/Downloads
head dog.log
tail dog.log
tail -n 50 dog.log
```

**Result:** `/regular_paw_storage/16239327166ba6dc/tailheaded.png`. Image: `tailheaded.png`.

## 02MuteDog

I captured both output streams. The output contained two path fragments, which I reordered to form the image path. The block below gives the intended redirection; the original captured command text was incomplete.

```bash
./mute_dog > /tmp/mute_dog.out 2>&1
cat /tmp/mute_dog.out
```

**Result:** `/regular_paw_storage/33c1ccdd6829ebe7/Mute_dog.png`. Image: `Mute_dog.png`.

## 02PetDog

I completed the interrupted rebase, kept the repaired image pieces, dropped the three commits that corrupted them, and resolved both conflicts. In `config/petting.cfg` I retained the surrounding settings and used the values below. In `src/petting_core.sh` I retained the other functions and replaced `pet_loop` with the shown version.

```bash
cd /home/ctf/go_to_opt/Doguments/auto_petter
cat autopet.sh
cat README.md
git status
git log --oneline --all --graph -n 30
cat tools/assemble_mascot.sh
cat config/petting.cfg
cat .git/rebase-merge/git-rebase-todo
```

```bash
nano config/petting.cfg
```

```ini
default = 2
max_safe = 5
hard_clamp = true

[turbo]
enabled = true
wind_up_sec = 2
```

```bash
git rebase --edit-todo
```

```text
drop 46c7a11 wip: experimental resampler on part3 (produces junk, do not ship)
drop de45c0c wip: experimental resampler on part1 (produces junk, do not ship)
drop c014945 wip: new dithering filter on part2 (mangles the output)
pick cb15ec2 core: adaptive petting cadence
exec ./tools/assemble_mascot.sh
```

```bash
git add config/petting.cfg
git rebase --continue
```

```bash
cat src/petting_core.sh
nano src/petting_core.sh
```

```sh
pet_loop() {
    target="$1"
    intensity="$2"
    strokes=0
    max_strokes=$((intensity * 40))
    while [ "$strokes" -lt "$max_strokes" ]; do
        check_tail "$target" || break
        pet_once "$target" "$intensity"
        strokes=$((strokes + 1))
        sleep 0.2
    done
}
```

```bash
sh -n src/petting_core.sh
git add src/petting_core.sh
git rebase --continue
```

**Result:** `/home/ctf/go_to_opt/Doguments/auto_petter/mascot.png`. Image: `mascot.png`. Assembly reported 114456 bytes.

## 02RemDog

I inspected the ASCII drawings, identified `1cc902a4` as an angry dog, compared file contents, and removed its twelve exact copies.

```bash
cd /home/ctf/go_to_opt/Doguments/unsorted
ls -la
cat README
file -- *
head -n 30 00c08d8a 1cc902a4
```

```bash
for f in *; do
  cmp -s -- "$f" 1cc902a4 && printf '%s\n' "$f"
done
```

```bash
rm -- 1cc902a4 257b3a0c 7319f789 86ba4ddf 8b994dbd a25e70e2 a3e64607 a4104275 ab79f1a8 c7b6fe75 d47066c5 d88fb218
```

**Result:** `/regular_paw_storage/8268a8989cd31693/Rem_dog.png`. Image: `Rem_dog.png`.

## 02ScatteredDog

I concatenated the pieces in numeric order from -50 to 49. `--` makes negative filenames literal arguments.

```bash
cd '/home/ctf/go_to_opt/Videos/Scattered dog'
ls -la
for i in $(seq -50 49); do
  cat -- "$i"
done > /tmp/Scattered_dog.png
file /tmp/Scattered_dog.png
```

**Result:** `/tmp/Scattered_dog.png`. Image: `Scattered_dog.png`.

## 02ShyDog

I checked file signatures instead of trusting filenames or extensions.

```bash
cd '/home/ctf/go_to_opt/Videos/Shy dog'
ls -la
file -- * | grep -i 'PNG image data'
```

**Result:** `/home/ctf/go_to_opt/Videos/Shy dog/Vacation_Maybe_Dog_48a095bb.png`. Image: `Vacation_Maybe_Dog_48a095bb.png`.

## 02SpitoonDog

I read the manual and followed its dialogue, including the final SIGINT. I entered the first phrase below with Enter. After `Yea, so?`, I typed `Without 200 errors` and pressed Ctrl+C without Enter.

```bash
cd /home/ctf/go_to_opt/Programs
./spitoon
man spitoon
```

```text
How tough am I? How tough am I?! I ran 200 of linux programs this lab
```

**Result:** `/regular_paw_storage/c713f35855e81dcd/Spitoon_dog.png`. Image: `Spitoon_dog.png`.

## 03AqueousDog

I inspected the fish shell configuration. It contained a Base64-encoded image path and its decoding command. The standalone decoding command below reproduces the path; that separate invocation was not recorded during the original run.

```bash
command -v fish
find ~/.config -maxdepth 3 -type f
cat /home/ctf/.config/fish/config.fish
```

**Result:** `/regular_paw_storage/f0e994fba47b3b89/Aqueous_dog.png`. Image: `Aqueous_dog.png`.

## 03ChainedDog

I followed each `Next:` link from `start` to `end`, appending only the bytes after `Content: `. Binary content stayed in pipes rather than shell variables.

```bash
ls -la /home/dog/Downloads
cat /home/dog/Downloads/start
cat /home/dog/Downloads/end
```

```bash
cd /home/dog/Downloads
name=start

while :; do
  next=$(head -n 1 -- "$name")
  next=${next#Next: }
  tail -n +2 -- "$name" | tail -c +10
  [ "$name" = end ] && break
  name=$next
done > /tmp/Chained_dog.png

file /tmp/Chained_dog.png
```

**Result:** `/tmp/Chained_dog.png`. Image: `Chained_dog.png`.

## 03InsecureDog

The directory allowed traversal but denied listing. The note restricted the filename to four letters from D, O and G, so I checked the 81 possible names directly.

```bash
cd /home/ctf/go_to_opt/Doguments/secret_stuff
ls -la
cat remember.txt
cd super_secret
ls -la
```

```bash
for a in D O G; do
  for b in D O G; do
    for c in D O G; do
      for d in D O G; do
        name="$a$b$c$d.png"
        [ -f "$name" ] && printf '%s\n' "$name"
      done
    done
  done
done
```

**Result:** `/home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/GDGO.png`. Image: `GDGO.png`.

## 03NetDog

The netdog program reported port 4210. I saved the TCP response as binary image data.

```bash
ls -la /usr/local/bin
/usr/local/bin/netdog
```

```bash
nc 127.0.0.1 4210 > /tmp/Net_dog.png
file /tmp/Net_dog.png
```

**Result:** `/tmp/Net_dog.png`. Image: `Net_dog.png`.

## 03RestoreDog

I recovered the encrypted Vim swap using the key from `crashed.txt`: `d99570c01553852b`. After entering the key and pressing Enter, the recovered buffer displayed the image path. I left the recovered buffer without overwriting the original file.

```bash
cd /home/ctf/go_to_opt/Doguments
ls -la
cat crashed.txt
vim -r crashed.txt
```

**Result:** `/regular_paw_storage/d9oslxm/Restore_dog.png`. Image: `Restore_dog.png`.

## 03SpyDog

The README and Git history mentioned deleted camera frames. I used `fsck` to locate an unreachable blob and extracted its raw contents without modifying the working tree.

```bash
ls -la /home/dog/Doguments/cat_tracker
cat /home/dog/Doguments/cat_tracker/README.md
git -C /home/dog/Doguments/cat_tracker log --all --oneline -n 25
git -C /home/dog/Doguments/cat_tracker show --stat f86a43b
git -C /home/dog/Doguments/cat_tracker show --stat 656c5c5
git -C /home/dog/Doguments/cat_tracker show f86a43b:tools/frame_grab.sh
git -C /home/dog/Doguments/cat_tracker show 656c5c5:suspects.txt
```

```bash
git -C /home/dog/Doguments/cat_tracker fsck --full --no-reflogs --unreachable
```

```bash
git -C /home/dog/Doguments/cat_tracker cat-file blob e951576f6dfee0ef423fc5c12b7a116e3dc174e2 > /tmp/recovered_blob
file /tmp/recovered_blob
```

**Result:** `/tmp/recovered_blob`. Image: `Spy_dog.png`.

## 03UsedDog

I listed the other account’s home directory. The image was readable by my account without switching users.

```bash
ls -la /home/dog
```

**Result:** `/home/dog/Used_dog.png`. Image: `Used_dog.png`.

## Downloading the images

From my Mac, I copied each resulting image to my local machine, substituting its absolute remote path:

```bash
scp "r.kharisov-dog@osctf.nnk.moe:/absolute/remote/image.png" ./
```

This command is run in a local terminal; `./` is the local destination directory. The filenames above identify the recovered images; image files are not included in this submission.
