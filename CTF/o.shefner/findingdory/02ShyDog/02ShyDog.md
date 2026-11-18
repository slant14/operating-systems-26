# 02ShyDog: Identifying files by their signatures

**Author:** Oleg Shefner (`o.shefner`)
**Instance:** `findingdory` (`dogctf`)

## Task and approach

The directory contains 1000 candidates. Names and extensions are unreliable, so inspect the actual file format.

## Procedure

The commands below describe the saved solution or its reproduction in the challenge container. Generated paths and filenames refer to this recorded instance.

```bash
cd "/home/ctf/go_to_opt/Videos/Shy dog"
file -- * | grep "PNG image data"
```

Signature inspection identified `IMG_Maybe_Dog_2a705290.png` as the real image. It was saved locally as `Shy_dog.png`. The expected PNG signature begins with hexadecimal `89 50 4E 47 0D 0A 1A 0A`.

## Result

Recovered artifact: [Shy_dog.png](../found/Shy_dog.png). The PNG was transferred as a file, preserving its bytes.


## What this demonstrates

File signatures provide stronger evidence than filename extensions.

## Sources

Personal [recovery notes](../found/dogctf_batch.md), the saved PNG,.
