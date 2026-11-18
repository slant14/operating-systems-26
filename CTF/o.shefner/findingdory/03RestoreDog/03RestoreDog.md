# 03RestoreDog: Recovering an encrypted Vim buffer

**Author:** Oleg Shefner (`o.shefner`)
**Instance:** `findingdory` (`dogctf`)

## Task and approach

A crashed editing session left a Vim swap file. The surviving text provides the key needed to read the recovered encrypted record.

## Procedure

The commands below describe the saved solution or its reproduction in the challenge container. Generated paths and filenames refer to this recorded instance.

```bash
cd /home/ctf/go_to_opt/Doguments
cat crashed.txt
vim -r crashed.txt
```

After recovery, opening the recovered record in Vim with the key from `crashed.txt` revealed `/regular_paw_storage/yjxgcaf/Restore_dog.png`. The previously saved notes retain the instance-specific key; it is not needed in this public explanation. A fresh reproduction should use its own note and keep the original swap file until recovery succeeds.

## Result

Recovered artifact: [Restore_dog.png](../found/Restore_dog.png). The PNG was transferred as a file, preserving its bytes.


## What this demonstrates

A swap file can preserve unsaved editor state, including data absent from the ordinary file.

## Sources

Personal [recovery notes](../found/dogctf_batch.md), the saved PNG,.
