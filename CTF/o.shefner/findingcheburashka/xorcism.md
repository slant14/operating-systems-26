# CTF `xorcism` — report

**Instance:** `findingcheburashka` (`cctf`).

## Inspecting the check

The program in `demon.c` reads a password, checks that it has 33 characters, and compares each character with:

```c
enc[i] ^ key[i % 4]
```

Both arrays are present in the source. The password can therefore be recovered directly by applying the same XOR operation; brute force is unnecessary.

## Solution

The following script reads the arrays from the supplied source instead of copying a password from another instance:

```python
from pathlib import Path
import re

source = Path("demon.c").read_text()

def read_array(name):
    pattern = r"\b" + name + r"\s*\[[^\]]+\]\s*=\s*\{([^}]+)\}"
    match = re.search(pattern, source, re.S)
    if match is None:
        raise ValueError("Missing array: " + name)
    return bytes(int(x, 16) for x in re.findall(r"0x([0-9a-fA-F]+)", match[1]))

enc = read_array("enc")
key = read_array("key")
password = bytes(value ^ key[i % len(key)] for i, value in enumerate(enc))
assert len(password) == 33
print(password.decode("ascii"))
```

Save it as `decode.py` and pass its output to the original program:

```bash
gcc -Wall -Wextra demon.c -o demon
python3 decode.py | ./demon
```

The program's own comparison checks the reconstructed password and prints the flag. The four-byte key repeats across all 33 positions. Using the local arrays matters because the letter case and some characters vary between instances.
