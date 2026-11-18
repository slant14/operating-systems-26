# dogctf — 02MuteDog (100 points)

**Flag:** picture file (`Mute_dog.png`, 220x300 PNG).

## Task

`~/go_to_opt/Programs/mute_dog` is an executable with `--wx--x--x`
permissions — it can be *run* but not *read* (no `strings`, no `file`).

"Mute dog" — a dog that is afraid to speak when a terminal is listening,
i.e. when its stdout is a TTY (it checks `isatty(1)`):

- **stdout is a TTY** (interactive ssh session): it stays silent — prints
  only a big Braille-ASCII art of a dog and the line
  `Terminals are listening`;
- **stdout is NOT a TTY** (pipe or file redirect): it feels safe and
  prints the reward path.

Verified live on the dog container (paramiko, `get_pty` on/off):

```
exec WITHOUT a PTY:                          exec WITH a PTY:
stdout: /regular_paw_storage/b7fea52c        (huge braille dog art)
stderr: 4862e50d/Mute_dog.png                "Terminals are listening"
```

Fun detail: the path is even split between two streams — in the clean
non-PTY case `/regular_paw_storage/b7fea52c` goes to stdout and
`4862e50d/Mute_dog.png` to stderr. `setsid` does NOT help: the dog looks
at `isatty(1)` only, not at the controlling terminal.

## Solution

In an interactive terminal just run it through a pipe (a pipe makes
stdout a non-TTY, so the dog "speaks"):

```sh
~/go_to_opt/Programs/mute_dog | cat
# or equivalently:
~/go_to_opt/Programs/mute_dog > /tmp/o.txt 2>&1; cat /tmp/o.txt
# -> /regular_paw_storage/b7fea52c4862e50d/Mute_dog.png
```

(`/regular_paw_storage/<12hex>/` dirs are root-owned `drwx--x--x`:
listing is denied, direct access by full path works.)

```sh
cp /regular_paw_storage/b7fea52c4862e50d/Mute_dog.png /tmp/
```

Send `Mute_dog.png.dat` to the bot as a document (a plain `.png` arrives
as a "photo" and the bot ignores it — use a `.dat`-suffixed document
without caption).

## Result

```
✅ Flag accepted for '02MuteDog'.
```
