# 01CatDog: Reconstructing a split PNG

**Author:** Oleg Shefner (`o.shefner`)
**Instance:** `findingdory` (`dogctf`)

## Task and approach

Two files in `Pictures` are consecutive byte ranges of one image. Their order matters: the PNG header must come first.

## Procedure

The commands below describe the saved solution or its reproduction in the challenge container. Generated paths and filenames refer to this recorded instance.

```bash
cd /home/ctf/go_to_opt/Pictures
cat Cat_dog_top Cat_dog_bottom > /tmp/Cat_dog.png
file /tmp/Cat_dog.png
```

The recorded solution joined `Cat_dog_top` and `Cat_dog_bottom` in that order. Concatenation preserves the original bytes; opening the pieces as separate pictures cannot restore the missing data.

## Result

Recovered artifact: [Cat_dog.png](../found/Cat_dog.png). The PNG was transferred as a file, preserving its bytes.


## What this demonstrates

Binary concatenation is different from visually stitching two images.

## Sources

Personal [recovery notes](../found/dogctf_batch.md), the saved PNG,.
