# 01EditDog: Editing fixed-width ASCII art

**Author:** Oleg Shefner (`o.shefner`)
**Instance:** `findingdory` (`dogctf`)

## Task and approach

The file `Doguments/EDITME` contains an ASCII dog and protected boundary text. The required edit changes the body width while preserving the rest of the file.

## Procedure

The commands below describe the saved solution or its reproduction in the challenge container. Generated paths and filenames refer to this recorded instance.

```bash
cd /home/ctf/go_to_opt/Doguments
cat EDITME
cp EDITME /tmp/EDITME.backup
nano EDITME
```

In this instance, five lines were extended by **nine columns**. The saved file used LF line endings. The checker then printed `Woof! You have solved Edit_dog!` and `/regular_paw_storage/6394577d2b2356c5/Edit_dog.png`. Other students report different widths, so their number is not applicable to this run.

## Result

Recovered artifact: [Edit_dog.png](../found/Edit_dog.png). The PNG was transferred as a file, preserving its bytes.


## What this demonstrates

Whitespace and line endings are part of the input when a checker compares text structure.

## Sources

Personal [recovery notes](../found/dogctf_batch.md), the saved PNG,.
