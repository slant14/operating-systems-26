# 01PoweredDog: Following the directory hint

**Author:** Oleg Shefner (`o.shefner`)
**Instance:** `findingdory` (`dogctf`)

## Task and approach

The name `go_to_opt` suggests inspecting `/opt`. A README there provides the next location.

## Procedure

The commands below describe the saved solution or its reproduction in the challenge container. Generated paths and filenames refer to this recorded instance.

```bash
ls -la /opt
cat /opt/README.md
```

The README revealed `/regular_paw_storage/a6a929afb8ed8396/powered_dog.png`. No recursive search through the restricted storage directory was necessary.

## Result

Recovered artifact: [powered_dog.png](../found/powered_dog.png). The PNG was transferred as a file, preserving its bytes.


## What this demonstrates

A direct clue can locate a file even when its parent directory cannot be enumerated.

## Sources

Personal [recovery notes](../found/dogctf_batch.md), the saved PNG,.
