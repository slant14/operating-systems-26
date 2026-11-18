# 01CompileDog: Compiling the supplied C program

**Author:** Oleg Shefner (`o.shefner`)
**Instance:** `findingdory` (`dogctf`)

## Task and approach

The clue is a C source file in `Programs`. It must be compiled before its output can reveal the reward location.

## Procedure

The commands below describe the saved solution or its reproduction in the challenge container. Generated paths and filenames refer to this recorded instance.

```bash
gcc /home/ctf/go_to_opt/Programs/compile_dog.c -o /tmp/compile_dog
/tmp/compile_dog
```

The earlier run printed `/regular_paw_storage/7881aeec9f35f5a0/Compile_dog.png`. The image was downloaded from that exact path.

## Result

Recovered artifact: [Compile_dog.png](../found/Compile_dog.png). The PNG was transferred as a file, preserving its bytes.


## What this demonstrates

A source file describes a program; the compiler produces the executable that can be run.

## Sources

Personal [recovery notes](../found/dogctf_batch.md), the saved PNG,.
