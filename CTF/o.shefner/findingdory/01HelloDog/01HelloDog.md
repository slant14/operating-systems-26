# 01HelloDog: Inspecting the starting directory

**Author:** Oleg Shefner (`o.shefner`)
**Instance:** `findingdory` (`dogctf`)

## Task and approach

The login greeting points to an image already present in the starting directory.

## Procedure

The commands below describe the saved solution or its reproduction in the challenge container. Generated paths and filenames refer to this recorded instance.

```bash
cd /home/ctf/go_to_opt
pwd
ls -la
file 'hoi_:3.png'
```

The remote filename was `hoi_:3.png`. The local evidence copy is named `hoi_3.png`; the change avoids the colon restriction in Windows filenames and does not change the PNG content.

## Result

Recovered artifact: [hoi_3.png](../found/hoi_3.png). The PNG was transferred as a file, preserving its bytes.


## What this demonstrates

Start with the actual working directory and inspect file types before searching the whole filesystem.

## Sources

Personal [recovery notes](../found/dogctf_batch.md), the saved PNG,.
