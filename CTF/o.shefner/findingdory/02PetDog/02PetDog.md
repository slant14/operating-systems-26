# 02PetDog: Assembling a mascot from repository files

**Author:** Oleg Shefner (`o.shefner`)
**Instance:** `findingdory` (`dogctf`)

## Task and approach

The `auto_petter` repository contains image fragments and an assembly script. The goal is to provide the correct fragments to the assembler.

## Procedure

The commands below describe the saved solution or its reproduction in the challenge container. Generated paths and filenames refer to this recorded instance.

```bash
cd /home/ctf/go_to_opt/Doguments/auto_petter
git status --short
git log --oneline --all
ls -l frames
cat tools/assemble_mascot.sh
./tools/assemble_mascot.sh
file mascot.png
```

The saved run assembled `mascot.png` from `frames/part1.bin`, `part2.bin`, and `part3.bin`. Peer reports describe recovering altered fragments from earlier Git commits. The personal notes preserve the successful assembly, but not a commit-by-commit restoration transcript. If a fragment is damaged in a fresh instance, inspect its own history and restore the appropriate version before assembling.

## Result

Recovered artifact: [mascot.png](../found/mascot.png). The PNG was transferred as a file, preserving its bytes.


## What this demonstrates

An assembly script and Git history together explain how damaged assets can be recovered.

## Sources

Personal [recovery notes](../found/dogctf_batch.md), the saved PNG,.

Additional comparison: [PR #34](https://github.com/slant14/operating-systems-26/pull/34), [PR #76](https://github.com/slant14/operating-systems-26/pull/76), [PR #77](https://github.com/slant14/operating-systems-26/pull/77), [PR #79](https://github.com/slant14/operating-systems-26/pull/79).
