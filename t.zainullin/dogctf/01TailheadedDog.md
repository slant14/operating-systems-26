# dogctf — 01TailheadedDog (100 points)

**Flag:** picture file (`tailheaded.png`).

## Task

`~/go_to_opt/Downloads/dog.log` — a huge file. Its head (line 2) mentions
`/regular_paw_storage/7e1e731d`, which does not exist ("I wonder if there is
more at the end of it..").

## Solution

The path is **split between the head and the tail** of the log:

- head: `/regular_paw_storage/` + `7e1e731d`
- tail (the ASCII-art section): another hex fragment `9aa6e5b8` and the
  filename `tailheaded.png` (found by `grep -oE '[0-9a-f]{8,}'` and
  `grep -n '9aa6e5b8'`).

Concatenate the fragments and the directory exists:

```
/regular_paw_storage/7e1e731d9aa6e5b8/tailheaded.png
```

(The parent dir is `drwx--x--x` root-owned — listing is denied, but direct
access to a known name works.) Copy it out and send `tailheaded.png.dat` to
the bot as a document.

## Result

```
✅ Flag accepted for '01TailheadedDog'.
```
