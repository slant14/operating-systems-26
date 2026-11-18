# 03InsecureDog: Using a known filename in an unlistable directory

**Author:** Oleg Shefner (`o.shefner`)
**Instance:** `findingdory` (`dogctf`)

## Task and approach

The note describes a four-letter name made from D, O and G, with one repeated letter. Directory listing is restricted, but testing an exact candidate is possible.

## Procedure

The commands below describe the saved solution or its reproduction in the challenge container. Generated paths and filenames refer to this recorded instance.

```bash
cd /home/ctf/go_to_opt/Doguments/secret_stuff
cat remember.txt
for name in {D,O,G}{D,O,G}{D,O,G}{D,O,G}; do
    stat -- "super_secret/$name.png" 2>/dev/null
done
```

The successful candidate in this instance was `super_secret/OGOD.png`. The loop deliberately checks a superset of the clue's possible names. The image was then downloaded by its exact path.

## Result

Recovered artifact: [OGOD.png](../found/OGOD.png). The PNG was transferred as a file, preserving its bytes.


## What this demonstrates

Directory read permission controls listing; search permission controls lookup of a known name.

## Sources

Personal [recovery notes](../found/dogctf_batch.md), the saved PNG,.
