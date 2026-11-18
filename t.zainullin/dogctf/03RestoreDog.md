# dogctf - 03RestoreDog

**Flag:** accepted by the bot as a file (`Restore_dog.png`)

## Task

Container: `ssh t.zainullin-dog@osctf.nnk.moe` (instance `findingdory`).
In `~/go_to_opt/Doguments/` there is a broken editing session:

```
crashed.txt       "My session crashed while I was writing down where I hid
                   my favorite dog... Good thing vim keeps swap files around.
                   I encrypted my notes, so here is the recovery key:
                   7026deee8c6bd4be"
.crashed.txt.swp  vim swap file (vim 9.1, encrypted)
```

## Idea

The notes were written by **root** (the swap header shows the original path
`/root/go_to_opt/Doguments/crashed.txt`) and vim encrypts the swap file. Vim's
built-in recovery (`vim -r`) restores the buffer; the recovery key from
`crashed.txt` decrypts it.

`python3` is root-only (`-rwx------`), so the interactive key prompt has to be
driven through `script` with a piped stdin.



```
cd ~/go_to_opt/Doguments
printf '7026deee8c6bd4be\r\r:w! /tmp/recovered.txt\r:q!\r' \
  | TERM=dumb timeout 20 script -qec 'vim -n -r .crashed.txt.swp' /dev/null
```

The vim output shows the restored buffer - the hidden path:

```
/regular_paw_storage/6guyio0/Restore_dog.png
```

Then verify and download the picture:

```
file /regular_paw_storage/6guyio0/Restore_dog.png
# PNG image data, 300 x 211, 8-bit/color RGBA
```
