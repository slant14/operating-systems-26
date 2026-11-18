# 02MuteDog: Separating standard output and standard error

**Author:** Oleg Shefner (`o.shefner`)
**Instance:** `findingdory` (`dogctf`)

## Task and approach

The program emits information through separate output streams. Capturing them individually makes the reward path visible.

## Procedure

The commands below describe the saved solution or its reproduction in the challenge container. Generated paths and filenames refer to this recorded instance.

```bash
/home/ctf/go_to_opt/Programs/mute_dog > /tmp/mute.stdout 2> /tmp/mute.stderr
cat /tmp/mute.stdout
cat /tmp/mute.stderr
```

The recorded run revealed `/regular_paw_storage/a9b79bbb777e2336/Mute_dog.png`. The saved notes do not identify which captured stream held the path, so both are inspected here.

## Result

Recovered artifact: [Mute_dog.png](../found/Mute_dog.png). The PNG was transferred as a file, preserving its bytes.


## What this demonstrates

File descriptors 1 and 2 are independent; redirecting only stdout can miss useful output.

## Sources

Personal [recovery notes](../found/dogctf_batch.md), the saved PNG,.
