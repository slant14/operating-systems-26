# dogctf — 01CatDog (100 points)

**Flag:** accepted by the bot as a file (the reassembled picture `cat_dog.png`)

## Task

Container: `ssh t.zainullin-dog@osctf.nnk.moe` (instance `findingdory`).
In `~/go_to_opt/Pictures/` there are two files:

```
Cat_dog_top     PNG image data, 223 x 300  (26543 bytes)
Cat_dog_bottom  data                       (26543 bytes)
```

## Idea

"CatDog" — the picture was cut into two byte halves: the top half holds the PNG
header and the first part of the pixel data, the bottom half holds the rest of
the pixel data plus the closing `IEND` chunk. `cat` the two files together and
the PNG becomes valid again.

## Solution

```
cat ~/go_to_opt/Pictures/Cat_dog_top ~/go_to_opt/Pictures/Cat_dog_bottom > /tmp/cat_dog.png
file /tmp/cat_dog.png      # PNG image data, 223 x 300 (now ends with IEND)
```

The reassembled picture also contains a hidden PNG text chunk:

```
strings /tmp/cat_dog.png | tail
...
tEXtJunk
0c044e8aa2b0531224037674f5e41d530b6f374a
IEND
```

(Sending that hex string as text to the bot is NOT accepted — the bot wants
the picture file itself.)

Send `/tmp/cat_dog.png` to the bot **as a document** (Telegram compresses
`.png` files into photos, which the bot ignores; appending `.dat` to the file
name forces a plain document upload). No caption — the bot treats any message
text as a flag attempt.

Bot reply:

```
✅ Flag accepted for '01CatDog'.
```

## Result

```
01CatDog — 100 points
```
