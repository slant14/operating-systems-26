# dogctf — 01HelloDog (100 points)

**Flag:** accepted by the bot as a file (the dog picture `hoi_:3.png`)

## Task

Container: `ssh t.zainullin-dog@osctf.nnk.moe`. The bot says: solve the
challenge in the container, then **send the dog picture file** to the bot.

## Investigation

- `~/.bash_history` shows a previous session (DogOS tour: creating puppy
  folders, compiling a "woof" program, copying
  `grandpaw.png` from `/super_secret_doggo_vault` into
  `/regular_paw_storage/0084980c5fa61b59/`).
- `~/.bash_profile` sets up `PROMPT_COMMAND` that runs
  `/usr/local/bin/checker_dog check` on every prompt — this is how the tasks
  are graded inside the container.
- Looking for images in the container found these candidates:
  - `~/go_to_opt/hoi_:3.png` — "hoi :3" is literally a dog greeting ("hello")
  - `~/go_to_opt/Doguments/secret_stuff/super_secret/OGDG.png` (directory has
    `chmod 311`, but the file itself is readable by exact path)
  - `grandpaw.png` copy in `/regular_paw_storage/0084980c5fa61b59/`

## Solution

The greeting picture `hoi_:3.png` is the "hello dog". Download it and send it
to the bot **as a document** (a compressed Telegram photo is ignored; the bot
also treats any text/caption as a flag attempt, so send the file with **no
caption**):

```
# from the container:
md5sum ~/go_to_opt/hoi_:3.png
# (download the file, then send it to @InnoFlagranteBot as a document)
```

Important: switch to the right bot instance first (`/instance findingdory`),
otherwise the bot checks the file against the other CTF's tasks.

Bot reply:

```
✅ Flag accepted for '01HelloDog'.
```

## Result

```
01HelloDog — 100 points
```
