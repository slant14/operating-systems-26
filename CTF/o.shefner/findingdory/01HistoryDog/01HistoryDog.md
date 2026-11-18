# 01HistoryDog: Reading shell history

**Author:** Oleg Shefner (`o.shefner`)
**Instance:** `findingdory` (`dogctf`)

## Task and approach

A previous shell command contains the reward path. History is evidence of earlier activity even when the current directory has no obvious flag.

## Procedure

The commands below describe the saved solution or its reproduction in the challenge container. Generated paths and filenames refer to this recorded instance.

```bash
cat /home/ctf/.bash_history
grep -n "regular_paw_storage" /home/ctf/.bash_history
```

The recorded history entry referenced `/regular_paw_storage/155b69acc8fb0712/grandpaw.png`. That path was used to retrieve the image.

## Result

Recovered artifact: [grandpaw.png](../found/grandpaw.png). The PNG was transferred as a file, preserving its bytes.


## What this demonstrates

The saved history file and the interactive shell history can expose useful paths.

## Sources

Personal [recovery notes](../found/dogctf_batch.md), the saved PNG,.
