# dogctf — 03InsecureDog (100 points)

**Flag:** accepted by the bot as a file (`OGDG.png`)

## Task

Container: `ssh t.zainullin-dog@osctf.nnk.moe` (instance `findingdory`).
In `~/go_to_opt/Doguments/` there is a `secret_stuff/super_secret/` directory
whose permissions are locked down:

```
.bash_profile:  chmod 311 /home/ctf/go_to_opt/Doguments/secret_stuff/super_secret
```

`311` means `--wx--x--x`: you can enter the directory and create files, but you
can NOT list its contents.

## Idea

The protection is incomplete ("insecure"): directory listing is blocked, but a
file inside is still world-readable, so opening it **by its exact name** works
fine. The file name was visible in the earlier directory listing (before the
permissions were tightened) and via `find`:

```
find / -xdev -iname '*.png' | grep -v 'Shy dog'
...
/home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/OGDG.png
```

## Solution

```
file ~/go_to_opt/Doguments/secret_stuff/super_secret/OGDG.png
# PNG image data, 300 x 225, 8-bit/color RGBA
```

Download `OGDG.png` and send it to the bot as a plain document (no caption).
Bot reply:

```
✅ Flag accepted for '03InsecureDog'.
```

## Result

```
03InsecureDog — 100 points
```
