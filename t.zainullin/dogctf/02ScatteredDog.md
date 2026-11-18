# dogctf — 02ScatteredDog (100 points)

**Flag:** picture file (300x195 PNG, reassembled from scattered pieces).

## Task

`~/go_to_opt/Videos/Scattered dog/` contains 100 files named `-50 … -1`,
`0 … 49` (names start with a dash — careful when passing to shell commands).

## Solution

The PNG magic bytes (`89 50 4e 47`) sit in file **-50** → pieces must be
joined in numeric order (negative first, then zero/positive), which is exactly
`ls | sort -n`:

```sh
cd ~/go_to_opt/Videos/'Scattered dog'
ls | sort -n | sed 's/^/.\//' | xargs cat > /tmp/scattered_dog.png
file /tmp/scattered_dog.png
# PNG image data, 300 x 195, 8-bit/color RGBA, non-interlaced
```

Download the PNG and send it to the bot **as a document without caption**
(rename to `Scattered_dog.png.dat` — a plain `.png` arrives as a photo and the
bot silently ignores photos).

## Result

```
✅ Flag accepted for '02ScatteredDog'.
```
