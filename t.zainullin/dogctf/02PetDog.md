# dogctf — 02PetDog (100 points)

**Flag:** picture file (`mascot.png`, 300x200 PNG).

## Task

`~/go_to_opt/Doguments/auto_petter/` — a git repo stuck in an **interactive
rebase**. The mascot pipeline says `mascot.png` is assembled from
`frames/part1.bin`, `part2.bin`, `part3.bin` and "Frame parts must be the
final encodes. See the git history for the ongoing frame saga."

The working copy has placeholder/broken parts (88-byte part1, 88-byte part2),
and the history is full of "wip"/"botched" versions:

- part1: `5f21a5f hotfix: pull part1 from rotation…` vs `7f4e16e wip: bump
  part1 color depth (banding got worse…)`
- part2: `d04de5d mascot: design sent the final part2 encode, dropping the
  placeholder` vs `1d1f293 mascot: downscale part2…`
- part3: `95cdcdd mascot: clean re-encode of part3, the import was botched`
  vs `7be9892 wip: experimental resampler on part3 (produces junk…)`

## Solution

Every part has exactly one *real* blob of 38152 bytes in the object database
(the decoys are 88–740 bytes). Map blobs to parts via `git ls-tree` over all
commits:

```
part1 ← 510bfb751a3c41f101e845655b03b83b19e572e4
part2 ← 221288240e972c1b3d93a8120db2013d1d8068ba
part3 ← 82ef5bcd787dd68d4d98cec6969cec2fb716f23c
```

Assemble in the order from `tools/assemble_mascot.sh`
(**part2 + part3 + part1**):

```sh
cd /tmp
git -C ~/go_to_opt/Doguments/auto_petter cat-file -p 22128824… > p2.bin
git -C ~/go_to_opt/Doguments/auto_petter cat-file -p 82ef5bcd… > p3.bin
git -C ~/go_to_opt/Doguments/auto_petter cat-file -p 510bfb75… > p1.bin
cat p2.bin p3.bin p1.bin > mascot.png   # PNG magic check passes (89504e47)
```

Send `Pet_dog.png.dat` (the assembled mascot) to the bot as a document.

## Result

```
✅ Flag accepted for '02PetDog'.
```
