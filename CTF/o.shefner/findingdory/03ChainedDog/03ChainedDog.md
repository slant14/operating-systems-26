# 03ChainedDog: Following links and preserving binary content

**Author:** Oleg Shefner (`o.shefner`)
**Instance:** `findingdory` (`dogctf`)

## Task and approach

Files under `/home/dog/Downloads` form a linked sequence. A `Next:` field selects the next file and a `Content:` field carries part of the image.

## Procedure

The commands below describe the saved solution or its reproduction in the challenge container. Generated paths and filenames refer to this recorded instance.

```bash
cat /home/dog/Downloads/start
```

The saved reconstruction followed 272 files to the `end` marker and appended their binary content in traversal order. The implementation split each file once at the byte delimiter `\nContent: `, followed the `Next: ` name, and kept a set of visited names to detect cycles. The payload must not be decoded as text or stripped: either operation can corrupt PNG bytes.

## Result

Recovered artifact: [Chained_dog.png](../found/Chained_dog.png). The PNG was transferred as a file, preserving its bytes.


## What this demonstrates

A linked sequence is ordered by its pointers, not by filenames or directory listing order.

## Sources

Personal [recovery notes](../found/dogctf_batch.md), the saved PNG,.
