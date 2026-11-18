# 01TailheadedDog: Combining the beginning and end of a log

**Author:** Oleg Shefner (`o.shefner`)
**Instance:** `findingdory` (`dogctf`)

## Task and approach

The useful path is split across the two ends of `Downloads/dog.log`.

## Procedure

The commands below describe the saved solution or its reproduction in the challenge container. Generated paths and filenames refer to this recorded instance.

```bash
head /home/ctf/go_to_opt/Downloads/dog.log
tail /home/ctf/go_to_opt/Downloads/dog.log
```

The first fragment was `/regular_paw_storage/de0f94a4`; the final fragment was `b59d6d0b/tailheaded.png`. Joining them without an extra newline produced `/regular_paw_storage/de0f94a4b59d6d0b/tailheaded.png`.

## Result

Recovered artifact: [tailheaded.png](../found/tailheaded.png). The PNG was transferred as a file, preserving its bytes.


## What this demonstrates

`head` and `tail` isolate boundary information without printing an entire log.

## Sources

Personal [recovery notes](../found/dogctf_batch.md), the saved PNG,.
