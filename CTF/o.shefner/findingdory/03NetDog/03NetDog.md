# 03NetDog: Receiving an image from a local TCP service

**Author:** Oleg Shefner (`o.shefner`)
**Instance:** `findingdory` (`dogctf`)

## Task and approach

A service inside the container sends the image over TCP. Inspect listening sockets to locate it.

## Procedure

The commands below describe the saved solution or its reproduction in the challenge container. Generated paths and filenames refer to this recorded instance.

```bash
cat /proc/net/tcp
nc 127.0.0.1 4644 > /tmp/Net_dog.png
file /tmp/Net_dog.png
```

The recorded listener used port **4644**. `/proc/net/tcp` represents ports in hexadecimal, so convert the observed value before connecting. The port differs across instances; 4644 is the saved value for this account.

## Result

Recovered artifact: [Net_dog.png](../found/Net_dog.png). The PNG was transferred as a file, preserving its bytes.


## What this demonstrates

Redirect network output to a file to preserve a binary payload.

## Sources

Personal [recovery notes](../found/dogctf_batch.md), the saved PNG,.
