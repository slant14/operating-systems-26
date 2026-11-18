# 03AqueousDog: Inspecting another shell's configuration

**Author:** Oleg Shefner (`o.shefner`)
**Instance:** `findingdory` (`dogctf`)

## Task and approach

The clue is stored in the Fish shell configuration rather than the active Bash configuration.

## Procedure

The commands below describe the saved solution or its reproduction in the challenge container. Generated paths and filenames refer to this recorded instance.

```bash
cat /home/ctf/.config/fish/config.fish
# Copy only the encoded payload from the configuration:
printf "%s" "<base64-payload>" | base64 -d
```

Decoding the saved payload revealed `/regular_paw_storage/e523c9b09c90e881/Aqueous_dog.png`. The configuration can be read as data; executing the whole file is unnecessary for decoding the clue.

## Result

Recovered artifact: [Aqueous_dog.png](../found/Aqueous_dog.png). The PNG was transferred as a file, preserving its bytes.


## What this demonstrates

Shell configuration files can contain encoded data, and Base64 is encoding rather than encryption.

## Sources

Personal [recovery notes](../found/dogctf_batch.md), the saved PNG,.
