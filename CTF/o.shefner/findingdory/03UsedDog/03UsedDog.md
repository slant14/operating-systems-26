# 03UsedDog: Inspecting another home directory

**Author:** Oleg Shefner (`o.shefner`)
**Instance:** `findingdory` (`dogctf`)

## Task and approach

The reward is in the `dog` account's home directory rather than the initial user's directory.

## Procedure

The commands below describe the saved solution or its reproduction in the challenge container. Generated paths and filenames refer to this recorded instance.

```bash
ls -la /home
ls -la /home/dog
file /home/dog/Used_dog.png
```

The saved solution retrieved `/home/dog/Used_dog.png` directly. The notes do not record a password change or a privilege escalation, so neither is claimed as part of the solution.

## Result

Recovered artifact: [Used_dog.png](../found/Used_dog.png). The PNG was transferred as a file, preserving its bytes.


## What this demonstrates

Inspect accessible alternate locations before assuming a file must be hidden or protected.

## Sources

Personal [recovery notes](../found/dogctf_batch.md), the saved PNG,.
