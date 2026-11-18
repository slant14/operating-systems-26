# 02ScatteredDog: Ordering numbered binary fragments

**Author:** Oleg Shefner (`o.shefner`)
**Instance:** `findingdory` (`dogctf`)

## Task and approach

The image is divided into files named from `-50` to `49`. They must be joined in numeric order.

## Procedure

The commands below describe the saved solution or its reproduction in the challenge container. Generated paths and filenames refer to this recorded instance.

```bash
cd "/home/ctf/go_to_opt/Videos/Scattered dog"
for n in $(seq -50 49); do
    cat -- "./$n"
done > /tmp/Scattered_dog.png
file /tmp/Scattered_dog.png
```

The output is the recorded `Scattered_dog.png`. Using `./` and `--` prevents negative filenames from being interpreted as command options. A plain wildcard would use lexical order and corrupt the reconstruction.

## Result

Recovered artifact: [Scattered_dog.png](../found/Scattered_dog.png). The PNG was transferred as a file, preserving its bytes.


## What this demonstrates

Numeric ordering and safe handling of leading hyphens both matter when processing fragments.

## Sources

Personal [recovery notes](../found/dogctf_batch.md), the saved PNG,.
