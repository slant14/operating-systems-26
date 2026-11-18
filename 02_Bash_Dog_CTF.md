# Dog CTF: 11 accepted challenges

**Instance:** `findingdory` (11/22 accepted).

These flags are **PNG pictures**. Send `/challenge NAME` to the bot, then connect from Ubuntu:

```bash
ssh a.khaliullova-dog@osctf.nnk.moe
```

Enter the password from the bot. Run the challenge commands **inside SSH**. When a PNG path is found, type `exit`, then run **on Ubuntu**:

```bash
scp a.khaliullova-dog@osctf.nnk.moe:/path/to/picture.png ~/
```

Replace the path with the one found for that challenge. Send the downloaded picture **as an image** to the bot. Reconnect to SSH for the next task.

**Note:** The first two solutions are reconstructed from saved file names, not exact terminal transcripts.

## 1. 01HelloDog

A directory listing showed a greeting picture called `hoi_:3.png`:

```bash
ls ~/go_to_opt
```

To copy that file, leave SSH and run:

```bash
scp 'a.khaliullova-dog@osctf.nnk.moe:/home/ctf/go_to_opt/hoi_:3.png' ~/
```

Submit the picture. The saved listing confirms this file exists, but not that it was the exact one used for the accepted flag.

**Why:** `ls` shows the available picture; `scp` copies it to Ubuntu.

## 2. 01CatDog

The `Pictures` folder contains two parts, `Cat_dog_top` and `Cat_dog_bottom`:

```bash
ls ~/go_to_opt/Pictures
cat ~/go_to_opt/Pictures/Cat_dog_top ~/go_to_opt/Pictures/Cat_dog_bottom > ~/Cat_dog.png
```

Copy `~/Cat_dog.png` from the SSH container to Ubuntu, then send the PNG to the bot.

**Why:** `cat` puts the top part before the bottom part. `>` saves the combined data.

## 3. 01CompileDog

Find and compile the C source:

```bash
find ~ -name '*.c'
gcc ~/go_to_opt/Programs/compile_dog.c -o dog
./dog
```

The program prints a PNG path. Copy that picture with `scp` and submit it.

**Why:** `gcc` builds the program, and running it reveals the picture location.

## 4. 01HistoryDog

Find the picture path in command history:

```bash
history
```

The history shows this earlier copy command:

```bash
cp /super_secret_doggo_vault/grandpaw.png /regular_paw_storage/b83409ab8662242e/grandpaw.png
```

The **second** path is the picture location. Leave SSH and run:

```bash
scp a.khaliullova-dog@osctf.nnk.moe:/regular_paw_storage/b83409ab8662242e/grandpaw.png ~/
```

Submit `grandpaw.png`.

**Why:** `history` records an earlier command and reveals where the picture was placed.

## 5. 01TailheadedDog

Read the beginning and end of the log:

```bash
ls Downloads
head Downloads/dog.log
tail -n 15 Downloads/dog.log
```

The two outputs contain the beginning and end of a PNG path. Join the fragments, use the resulting full path with `scp`, and submit the picture.

**Why:** `head` reads the first lines; `tail` reads the last lines.

## 6. 01EditDog

Find the editable drawing:

```bash
find ~ -iname '*edit*'
nano ~/go_to_opt/Doguments/EDITME
```

The file asks for the ASCII dog to be **five characters longer**. Edit the drawing and save with **Ctrl+O**, **Enter**, **Ctrl+X**. It gives this picture path:

```text
/regular_paw_storage/5e863db8c97fc052/Edit_dog.png
```

Copy the PNG using `scp`, then submit it.

**Why:** The challenge checks the change saved in `EDITME`.

## 7. 01BackgroundDog

Resume the stopped job:

```bash
jobs
fg %1
```

It prints:

```text
/regular_paw_storage/a10672f338e5faeb/Background_dog.png
```

Press **Ctrl+C** to stop the repeating output. Copy the PNG with `scp` and submit it.

**Why:** `fg %1` resumes job 1 in the foreground.

## 8. 03InsecureDog

I found this **while looking for `02RemDog`**, not while searching for `03InsecureDog`:

```bash
find ~/go_to_opt -iname '*rem*'
cat ~/go_to_opt/Doguments/secret_stuff/remember.txt
```

The note says that a PNG is inside `super_secret`. Its four-letter name is built from `D`, `O`, `G`, with one letter repeated. The directory cannot be listed, so try the possible names:

```bash
ls ~/go_to_opt/Doguments/secret_stuff/super_secret/{D,O,G}{D,O,G}{D,O,G}{D,O,G}.png
```

This finds `OGDG.png`. Leave SSH and run:

```bash
scp a.khaliullova-dog@osctf.nnk.moe:/home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/OGDG.png ~/
```

The bot accepted it as **`03InsecureDog`**.

**Why:** Brace expansion tests possible filenames even though directory listing is blocked.

## 9. 02MuteDog

Find the program and show its output as text:

```bash
find ~ -iname '*mute*'
~/go_to_opt/Programs/mute_dog 2>&1 | cat
```

It reveals two parts of the path:

```text
/regular_paw_storage/8fc82283
97c75c7d/Mute_dog.png
```

Together they form:

```text
/regular_paw_storage/8fc8228397c75c7d/Mute_dog.png
```

Copy that PNG with `scp` and submit it.

**Why:** `2>&1` joins error output with normal output; `| cat` makes the text visible.

## 10. 02SpitoonDog

Find the program and read its manual:

```bash
find ~ -iname '*spitoon*'
man spitoon
~/go_to_opt/Programs/spitoon
```

Type and press **Enter**:

```text
How tough am I? How tough am I?! I ran 88 of linux programs this lab
```

Then type this line, but **do not press Enter**:

```text
Without 88 errors
```

Press **Ctrl+C**. The program prints:

```text
/regular_paw_storage/57370389d2621a99/Spitoon_dog.png
```

Copy that PNG and submit it.

**Why:** The manual gives the exact input. The program expects **Ctrl+C** during the second line.

## 11. 02ScatteredDog

Find the folder containing numbered pieces:

```bash
find ~ -iname '*scattered*'
ls ~/go_to_opt/Videos/'Scattered dog'
```

The 100 pieces are numbered `-50` through `49`. Put them in numerical order:

```bash
for i in $(seq -50 49); do cat ~/go_to_opt/Videos/'Scattered dog'/$i; done > ~/Scattered_dog.png
```

Leave SSH, copy the new PNG and submit it:

```bash
scp a.khaliullova-dog@osctf.nnk.moe:/home/ctf/Scattered_dog.png ~/
```

**Why:** `seq` lists the numbers in order; the loop combines the pieces into a PNG.
