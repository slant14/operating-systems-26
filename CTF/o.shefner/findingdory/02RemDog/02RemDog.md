# 02RemDog: Removing the specified ASCII files

**Author:** Oleg Shefner (`o.shefner`)
**Instance:** `findingdory` (`dogctf`)

## Task and approach

The README asks to remove evil dogs while keeping the friendly ones. Inspect the pictures before using their sizes as a shortcut.

## Procedure

The commands below describe the saved solution or its reproduction in the challenge container. Generated paths and filenames refer to this recorded instance.

```bash
cd /home/ctf/go_to_opt/Doguments
cat unsorted/README
tar -czf /tmp/unsorted-backup.tar.gz unsorted
find unsorted -type f -size 1937c -print
# After checking that every listed file is an evil-dog image:
find unsorted -type f -size 1937c -delete
/usr/local/bin/checker_dog check
```

This run contained 32 ASCII images: 13 evil dogs of 1937 bytes and 19 friendly dogs of 1678 bytes. After the targeted removal, the checker revealed `/regular_paw_storage/e1bb8928678ba2fe/Rem_dog.png`. The byte counts are observations from this instance, not universal rules for arbitrary files.

## Result

Recovered artifact: [Rem_dog.png](../found/Rem_dog.png). The PNG was transferred as a file, preserving its bytes.


## What this demonstrates

Inspect the selection and retain a backup before a destructive filesystem operation.

## Sources

Personal [recovery notes](../found/dogctf_batch.md), the saved PNG,.
