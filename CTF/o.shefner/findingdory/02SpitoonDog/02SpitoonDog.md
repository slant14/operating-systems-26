# 02SpitoonDog: Reading the manual and sending a signal

**Author:** Oleg Shefner (`o.shefner`)
**Instance:** `findingdory` (`dogctf`)

## Task and approach

The program expects a particular dialogue and an interrupt signal. The manual provides the exact phrases for the instance.

## Procedure

The commands below describe the saved solution or its reproduction in the challenge container. Generated paths and filenames refer to this recorded instance.

```bash
zcat /usr/share/man/man1/spitoon.1.gz
/home/ctf/go_to_opt/Programs/spitoon
```

The recorded first line was `How tough am I? How tough am I?! I ran 227 of linux programs this lab`. The next was `Without 227 errors`. After typing the second line, **Ctrl+C was pressed without Enter**. The program printed `/regular_paw_storage/e230d0d6477947df/Spitoon_dog.png`. The number 227 belongs to this run.

## Result

Recovered artifact: [Spitoon_dog.png](../found/Spitoon_dog.png). The PNG was transferred as a file, preserving its bytes.


## What this demonstrates

Terminal control keys generate signals; they are not ordinary characters sent with a newline.

## Sources

Personal [recovery notes](../found/dogctf_batch.md), the saved PNG,.
