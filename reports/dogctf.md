## 01HelloDog
The home folder contains hoi_:3.png — the flag. But the ':' in the filename breaks scp, so rename it first.

**In container:**
```bash
cd /home/ctf/go_to_opt
mv '/home/ctf/go_to_opt/hoi_:3.png' /home/ctf/go_to_opt/hoi.png
```

**On local machine:**
```bash
scp e.efremova-dog@osctf.nnk.moe:/home/ctf/go_to_opt/hoi.png .
```

## 01CatDog
Pictures contains two halves of one PNG — concatenate them with cat.

**In container:**
```bash
cd /home/ctf/go_to_opt/Pictures
ls
# Cat_dog_bottom  Cat_dog_top

cat Cat_dog_top Cat_dog_bottom > new_cat
```

**On local machine:**
```bash
scp e.efremova-dog@osctf.nnk.moe:/home/ctf/go_to_opt/Pictures/new_cat ./new_cat.png
```

## 01CompileDog
Programs contains the source compile_dog.c — compile and run it; it prints the image path.

**In container:**
```bash
cd /home/ctf/go_to_opt/Programs

gcc compile_dog.c
./a.out
# /regular_paw_storage/ebe7eedfb0049c02/Compile_dog.png
```

**On local machine:**
```bash
scp e.efremova-dog@osctf.nnk.moe:/regular_paw_storage/ebe7eedfb0049c02/Compile_dog.png .
```

## 01EditDog 
Doguments/EDITME asks to make the dog 4 characters longer; edit the ASCII art inside the safety lines.

**In container:**
```bash
cd /home/ctf/go_to_opt/Doguments
nano EDITME
# added 4 characters (' ' or '_') to the dog, saved (Ctrl+O, Enter, Ctrl+X)
# → /regular_paw_storage/82d0e2b8c26b9910/Edit_dog.png
```

**On local machine:**
```bash
scp e.efremova-dog@osctf.nnk.moe:/regular_paw_storage/82d0e2b8c26b9910/Edit_dog.png .
```

## 01PoweredDog
The README in /opt contains the path to the powered dog image.

**In container:**
```bash
cd /opt 
cat README.md
# → /regular_paw_storage/1c022590000d699a/powered_dog.png
```

**On local machine:**
```bash
scp e.efremova-dog@osctf.nnk.moe:/regular_paw_storage/1c022590000d699a/powered_dog.png .
```

## 01TailheadedDog 
Downloads/dog.log stores the two halves of the image path at its very beginning and very end; use head and tail.

**In container:**
```bash
cd /home/ctf/go_to_opt/Downloads

head -n 15 dog.log
# Look, a dog image path:
# /regular_paw_storage/88183fb7

tail -n 15 dog.log
# c0172868/tailheaded.png

# (full path: /regular_paw_storage/88183fb7c0172868/tailheaded.png)
```

**On local machine:**
```bash
scp e.efremova-dog@osctf.nnk.moe:/regular_paw_storage/88183fb7c0172868/tailheaded.png .
```

## 02RemDog 
unsorted contains friendly and angry dogs; angry ones (1937 B) contain the bared-teeth fragment 'd8888888888b', friendly ones (1678 B) do not.

**In container:**
```bash
cd /home/ctf/go_to_opt/Doguments/unsorted
grep -l 'd8888888888b' *
# 036f9caa 05852ac0 09a3ee19 17d5a2dc 1b44a6f3 2053b119 2f542425 4566193f
# 6d9c396f 9678dab3 a0c99b8d a2f5db0c a7cf759c b68f99bb b696f51b cd772e58
rm 036f9caa 05852ac0 09a3ee19 17d5a2dc 1b44a6f3 2053b119 2f542425 4566193f 6d9c396f 9678dab3 a0c99b8d a2f5db0c a7cf759c b68f99bb b696f51b cd772e58
# Here's your dog pic: /regular_paw_storage/7493e38ea1864ea4/Rem_dog.png
```

**On local machine:**
```bash
scp e.efremova-dog@osctf.nnk.moe:/regular_paw_storage/7493e38ea1864ea4/Rem_dog.png .
```

## 02SpitoonDog 
man spitoon documents the exact phrases and the SIGINT (Ctrl+C) requirement — press Ctrl+C instead of Enter after the second answer.

**In container:**
```bash
cd /home/ctf/go_to_opt/Programs

man spitoon
# man page reveals the exact answers and the Ctrl+C (SIGINT) trick

./spitoon
# Welcome to the salty spitoon, how tough are ya?
# > How tough am I? How tough am I?! I ran 20 of linux programs this lab
# Yea, so?
# > Without 20 errors   [Ctrl+C instead of Enter]
# Right this way sir: /regular_paw_storage/13eb9885f1975074/Spitoon_dog.png, sorry to keep you waiting
```

**On local machine:**
```bash
scp e.efremova-dog@osctf.nnk.moe:/regular_paw_storage/13eb9885f1975074/Spitoon_dog.png .
```

## 03InsecureDog
remember.txt gives the filename pattern (4 chars from D/O/G, one repeated); super_secret has d-wx--x--x so ls fails, but fopen on a guessed name works — brute-force it.

**In container:**
```bash
cd /home/ctf/go_to_opt/Doguments/secret_stuff
cat remember.txt
# hint: filename consists of D, O, G, length 4, one letter appears twice

nano main.c
# brute-forces all combinations of D, O, G using fopen() to bypass 'ls' permission denial

gcc main.c -o main
./main
# Found: /home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/DDGO.png
```
*(main.c)*
```c
#include <stdio.h>

int main() {
    char letters[3] = {'D', 'O', 'G'};
    char name[64];
    for (int a = 0; a < 3; a++) {
        for (int b = 0; b < 3; b++) {
            for (int c = 0; c < 3; c++) {
                for (int d = 0; d < 3; d++) {
                    snprintf(name, sizeof(name), "/home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/%c%c%c%c.png", letters[a], letters[b], letters[c], letters[d]);
                    FILE *f = fopen(name, "rb");
                    if (f) {
                        printf("Found: %s\n", name);
                        fclose(f);
                    }
                }
            }
        }
    }
    return 0;
}
```

**On local machine:**
```bash
scp e.efremova-dog@osctf.nnk.moe:/home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/DDGO.png .
```

## 03RestoreDog
crashed.txt contains the vim recovery key; vim -r on the swap file restores the encrypted notes after entering the key.

**In container:**
```bash
cd /home/ctf/go_to_opt/Doguments
cat crashed.txt
# recovery key: b5c8ec4bb76c43cc
vim -r .crashed.txt.swp
# enter key: b5c8ec4bb76c43cc
# recovered path: /regular_paw_storage/v03kvn6/Restore_dog.png
```

**On local machine:**
```bash
scp e.efremova-dog@osctf.nnk.moe:/regular_paw_storage/v03kvn6/Restore_dog.png .
```

## 03UsedDog
Used_dog.png is in /home/dog — copy it directly from there.

**In container:**
```bash
cd /home/dog
```

**On local machine:**
```bash
scp e.efremova-dog@osctf.nnk.moe:/home/dog/Used_dog.png .
```