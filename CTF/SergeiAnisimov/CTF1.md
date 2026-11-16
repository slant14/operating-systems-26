# CTF 1 Sergei Anisimov CSE-04

**Bash CTF (`dogctf`), instance `findingdory` — 11 of 22 flags accepted (50%).**

## Before repeating a task

1. In the bot, select the instance with `/instance findingdory` if needed. Open the task with `/challenge` followed by its exact name, for example `/challenge 01CatDog`.
2. Open PowerShell and set `$ctfLogin` to the login supplied by the bot. Use the bot's password at the SSH prompt.

   ```powershell
   $ctfLogin = 'YOUR_LOGIN'
   ssh "${ctfLogin}@osctf.nnk.moe"
   ```

3. In the Linux terminal, run `cd /home/ctf/go_to_opt` and then `pwd`. Every Linux command below assumes that starting directory. The `ctf@…$` prompt is the Linux session.
4. Download the result from a separate Windows PowerShell window. Set `$ctfLogin` there as well. `scp -O` uses the legacy SCP transfer mode for this server. The left argument identifies the remote file and the right argument saves the copy in Windows Downloads.
5. When submitting a flag, attach the downloaded PNG as a file so Telegram preserves its bytes. Confirm the bot names the expected challenge in its acceptance message.

The paths are the ones observed in `findingdory`. When a command prints a path during a repeat, use the path from that run for the download.

Jump to [Hello](#01hellodog) · [Cat](#01catdog) · [Tailheaded](#01tailheadeddog) · [Background](#01backgrounddog) · [Compile](#01compiledog) · [Scattered](#02scattereddog) · [Shy](#02shydog) · [Restore](#03restoredog) · [Mute](#02mutedog) · [Pet](#02petdog) · [Rem](#02remdog)

## 01HelloDog

```bash
pwd
ls -la
```

`pwd` should print `/home/ctf/go_to_opt`. `ls -la` shows detailed entries, including hidden files. The regular file `hoi_:3.png` was visible in that directory and its size was 60,446 bytes. Combining the directory and filename gave the download path.

Keep the colon in the Linux filename. Save the Windows copy as `01HelloDog.png`, since Windows filenames cannot contain a colon.

```powershell
scp -O "${ctfLogin}@osctf.nnk.moe:/home/ctf/go_to_opt/hoi_:3.png" "$env:USERPROFILE\Downloads\01HelloDog.png"
```

## 01CatDog

```bash
file Pictures/Cat_dog_top Pictures/Cat_dog_bottom
cat Pictures/Cat_dog_top Pictures/Cat_dog_bottom > /tmp/01CatDog.png
ls -l /tmp/01CatDog.png
```

`file` identified `Cat_dog_top` as a 223 × 300 PNG because it contained the image header. It identified `Cat_dog_bottom` as `data` because that part started inside the file. `cat` emits the exact bytes of each input in order, and `>` writes these bytes to the output file. `ls -l` showed 53,086 bytes, which is 26,543 bytes from each part.

```powershell
scp -O "${ctfLogin}@osctf.nnk.moe:/tmp/01CatDog.png" "$env:USERPROFILE\Downloads\01CatDog.png"
```

## 01TailheadedDog

```bash
head -c 240 Downloads/dog.log
tail -n 30 Downloads/dog.log
file /regular_paw_storage/58e4094f2a4380ba/tailheaded.png
```

`head -c 240` previews the first 240 bytes. It showed `/regular_paw_storage/58e4094f` and a note to look at the end. A lookup using only that fragment failed. Reading 30 full lines with `tail -n 30` revealed `2a4380ba/tailheaded.png` between two drawings. Join the fragments directly. `58e4094f2a4380ba` is one directory name. `file` reported a 300 × 300 PNG.

```powershell
scp -O "${ctfLogin}@osctf.nnk.moe:/regular_paw_storage/58e4094f2a4380ba/tailheaded.png" "$env:USERPROFILE\Downloads\01TailheadedDog.png"
```

## 01BackgroundDog

```bash
jobs -l
bg %1
kill -STOP %1
file /regular_paw_storage/3e353d62763c2263/Background_dog.png
```

Use the original SSH shell because Bash's job list belongs to that shell. `jobs -l` showed `[1]+ 3366 Stopped (signal) /usr/local/bin/background_dog /tmp/bgdog`. `[1]` is the Bash job number and `3366` was the process ID. On a repeat, replace `%1` with the job number shown for `background_dog`.

`bg` continued the stopped job. It repeatedly printed the actual image path. Once the path was visible, `kill -STOP` paused the job to stop the repeated output. Use the printed path for the file lookup. `/tmp/bgdog` was the argument shown in the job entry.

```powershell
scp -O "${ctfLogin}@osctf.nnk.moe:/regular_paw_storage/3e353d62763c2263/Background_dog.png" "$env:USERPROFILE\Downloads\01BackgroundDog.png"
```

## 01CompileDog

```bash
head -n 45 Programs/compile_dog.c
tail -n 90 Programs/compile_dog.c
gcc Programs/compile_dog.c -o /tmp/01CompileDog && /tmp/01CompileDog
```

The top of the source defines the embedded instructions in `woofscript`. The end shows `main()` calling `prance()`, which interprets the instructions, prints characters with `putchar()` and exits on the stop instruction. `gcc` compiles the source, `-o` names the executable, and `&&` runs it only after compilation succeeds.

The program printed `/regular_paw_storage/6faf9edf706d35d3/Compile_dog.png`.

```powershell
scp -O "${ctfLogin}@osctf.nnk.moe:/regular_paw_storage/6faf9edf706d35d3/Compile_dog.png" "$env:USERPROFILE\Downloads\01CompileDog.png"
```

## 02ScatteredDog

```bash
ls -la 'Videos/Scattered dog' | head -n 20
file -- 'Videos/Scattered dog/'* | grep 'PNG image data'
cat 'Videos/Scattered dog/'{-50..49} > /tmp/02ScatteredDog.png && file /tmp/02ScatteredDog.png
```

`ls` showed filenames such as `-1`, `-10`, and `-2`, so alphabetical order would join the bytes incorrectly. `file` identified `-50` as the part with the PNG header. Bash expands `{-50..49}` into the 100 consecutive names in numeric order, including `0`. The quoted directory prefix keeps the space in `Scattered dog` as part of each path.

The assembled file was recognized as a 300 × 195 PNG. The downloaded copy was 44,259 bytes.

```powershell
scp -O "${ctfLogin}@osctf.nnk.moe:/tmp/02ScatteredDog.png" "$env:USERPROFILE\Downloads\02ScatteredDog.png"
```

## 02ShyDog

```bash
file -b --mime-type 'Videos/Shy dog/'* | sort | uniq -c
file --mime-type -- 'Videos/Shy dog/'* | grep 'image/png$'
cp 'Videos/Shy dog/Gallery_Maybe_Dog_59562971.png' /tmp/02ShyDog.png
```

The directory contained 1,000 files with mixed extensions. `file --mime-type` checks their content. With `-b`, it emits only the MIME type. `sort` groups equal types and `uniq -c` counts them. The output included `1 image/png`. Running `file` without `-b` put the filenames back in the output, and `grep 'image/png$'` kept only the PNG line.

The matching file was `Gallery_Maybe_Dog_59562971.png`. `cp` copied it to a short path with no spaces for the transfer.

```powershell
scp -O "${ctfLogin}@osctf.nnk.moe:/tmp/02ShyDog.png" "$env:USERPROFILE\Downloads\02ShyDog.png"
```

## 03RestoreDog

```bash
cat Doguments/crashed.txt
vim -r Doguments/crashed.txt
```

1. `cat` shows the saved note and its recovery key. Keep the key visible so you can enter it when Vim prompts.
2. `vim -r` uses `Doguments/.crashed.txt.swp` to restore what was in the editor before it crashed. Vim displayed `E308` about the original file possibly having changed and then asked for the encryption key. Enter the key from the saved note. Press **Enter** again if Vim asks you to continue.
3. Read the recovered line, `/regular_paw_storage/19c05la/Restore_dog.png`. The character after `5` is a lowercase `l`. To exit without writing the recovered buffer over the saved note, press **Esc**, type `:q!`, and press **Enter**.

```powershell
scp -O "${ctfLogin}@osctf.nnk.moe:/regular_paw_storage/19c05la/Restore_dog.png" "$env:USERPROFILE\Downloads\03RestoreDog.png"
```

## 02MuteDog

```bash
./Programs/mute_dog
./Programs/mute_dog > /tmp/02MuteDog.out 2> /tmp/02MuteDog.err
file /tmp/02MuteDog.out /tmp/02MuteDog.err
head -c 200 /tmp/02MuteDog.out /tmp/02MuteDog.err
```

The direct run drew an image in the terminal and included the clue `Terminals are listening`. `>` redirects stdout, file descriptor `1`. `2>` redirects stderr, file descriptor `2`. With both redirected, `file` identified two text fragments with no line terminators. `head` displayed each fragment under its filename.

The stdout fragment was `/regular_paw_storage/5265eef2`. The stderr fragment was `5b50fbab/Mute_dog.png`. Put stdout first and join the two fragments directly.

```powershell
scp -O "${ctfLogin}@osctf.nnk.moe:/regular_paw_storage/5265eef25b50fbab/Mute_dog.png" "$env:USERPROFILE\Downloads\02MuteDog.png"
```

## 02PetDog

```bash
head -n 35 Doguments/auto_petter/README.md
head -n 80 Doguments/auto_petter/tools/assemble_mascot.sh
cat Doguments/auto_petter/frames/{part2.bin,part1.bin,part3.bin} > /tmp/02PetDog-current.png
file /tmp/02PetDog-current.png
git -C Doguments/auto_petter log --all --oneline -- frames
```

The README points to `frames/` and the Git history. The script gives the order `part2 → part1 → part3`. Concatenating the working-tree parts in that order gave `data`. The script's four-byte check only looks for the start of the PNG header. The history identifies the following versions.

| Part | Commit | Reason |
| --- | --- | --- |
| `part2.bin` | `1ba0d83` | Final encode from the design team, before the thumbnail version. |
| `part1.bin` | `6173e9d` | Clean re-encode after the faulty import and quantizer attempt. |
| `part3.bin` | `0574400` | Initial import, before the banding experiment and temporary stub. |

```bash
(
  git -C Doguments/auto_petter show 1ba0d83:frames/part2.bin &&
  git -C Doguments/auto_petter show 6173e9d:frames/part1.bin &&
  git -C Doguments/auto_petter show 0574400:frames/part3.bin
) > /tmp/02PetDog.png
file /tmp/02PetDog.png
```

`git -C` runs Git in the named repository. `git show COMMIT:PATH` writes the bytes of that file version. The single `>` after the group directs all three outputs into the same file, giving a 300 × 200 PNG.

```powershell
scp -O "${ctfLogin}@osctf.nnk.moe:/tmp/02PetDog.png" "$env:USERPROFILE\Downloads\02PetDog.png"
```

## 02RemDog

```bash
cat Doguments/unsorted/README
cat Doguments/unsorted/061042a2
cat Doguments/unsorted/29fc94a9
sha256sum Doguments/unsorted/[0-9a-f]* | cut -d' ' -f1 | sort | uniq -c
sha256sum Doguments/unsorted/{061042a2,29fc94a9}
```

The README said to remove the angry dogs. `29fc94a9` showed the dog with bared teeth. The checksums identified 18 copies of that picture, each 1,937 bytes, and 14 copies of the other picture, each 1,678 bytes. Comparing the checksum groups first established which size belonged to the picture selected for removal.

```bash
remdog_backup=$(mktemp -d /tmp/02RemDog.XXXXXX)
find Doguments/unsorted -maxdepth 1 -type f -name '[0-9a-f]*' -size 1937c -exec mv -t "$remdog_backup" -- {} +
printf '%s\n' "$remdog_backup"
ls -1 "$remdog_backup" | wc -l
checker_dog check
```

`mktemp -d` creates a new directory. `-maxdepth 1` limits the search to files directly inside `unsorted`, `-type f` selects regular files, and `1937c` means exactly 1,937 bytes. `mv -t` moves the matches to the directory printed by `printf`. `wc -l` should show `18`. The checker printed `Woof! You have solved Rem_dog!` and gave the image path.

```powershell
scp -O "${ctfLogin}@osctf.nnk.moe:/regular_paw_storage/743314a4ce1f74e3/Rem_dog.png" "$env:USERPROFILE\Downloads\02RemDog.png"
```

In the original run, the 18 moved files were kept in `/tmp/02RemDog.85XxPA`. To repeat from the original state in that same container, first confirm the backup still has all 18 files. Move those entries back into `Doguments/unsorted` before repeating the move and check. The command below skips any destination filename that already exists:

```bash
find /tmp/02RemDog.85XxPA -maxdepth 1 -type f -size 1937c | wc -l
find /tmp/02RemDog.85XxPA -maxdepth 1 -type f -name '[0-9a-f]*' -size 1937c -exec mv -n -t Doguments/unsorted -- {} +
```
