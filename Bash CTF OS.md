## Edit_dog
`cd Doguments/`
`nano EDITME`
replace everything by:
```
Make the dog 4 characters longer. Preserve the integrity of the dog.

DO NOT EDIT ANYTHING OUTSIDE THESE SAFETY LINES
\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\


			                       .-.
     (__________________________________()6 `-,
     (   ______________________________   /''"`
     //\\                             //\\
jgs  "" ""                            "" ""


\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
DO NOT EDIT ANYTHING OUTSIDE THESE SAFETY LINES
```
(just do it 4 charachters longer)


## Rem_dog
`cd Doguments/unsorted`
`nano README`
see each file by `nano filename` and remove all files with angry dogs by `rm filename`

## HelloDog
`ls`
`mv hoi_:3.png hoi.png`
(because I have Windows, it doesn't accept `:`)
## Background_dog
`jobs`
`fg 1`
`ctrl+z`
`cd /regular_paw_storage/369d92cd96ac388b/Background_dog.png`

## Powered_dog
`cd opt`
copy existing `.png` file

## Used_dog
`cd ..`
`cd ..`
`cd dog`
and just copy existing `.png` file
## Compiled_dog

`cd Programs`
`gcc compile_dog.c -o comp`
`./comp`

## Shy_dog
`cd 'Videos/Shy dog'`
`file -- * | grep -E 'PNG image data'`
we foung the only file which is real `.png` file. it is the flag

## History_dog
`history`
on line 13 there is an actual path to flag

## Scratted_dog
`cd Videos/Scattered dog`
in files there are pieces of picture. Combine them using C sode
``` c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    FILE *res = fopen("res.txt", "wb");
    if (!res) {
        exit(1);
    }

    for (int i = -50; i <= 50; ++i) {
        char name[16];
        snprintf(name, sizeof name, "%d", i);

        FILE *f = fopen(name, "rb");
        if (!f) continue;

        unsigned char buf[4096];
        size_t n;
        while ((n = fread(buf, 1, sizeof buf, f)) > 0) {
            fwrite(buf, 1, n, res);
        }
        fclose(f);
    }

    fclose(res);
    return 0;
}
```
`cat res.txt > res.png`
`res.png` is the flag
## Spitoon_dog
`cd Programs`
try `./spitoon` - we don't know answers
`men spitoon`
so for the question `Welcome to the salty spitoon, how tough are ya?` 
answer `How tough am I? How tough am I?! I ran 242 of linux programs this lab`
for the `Yea, so?`
answer `Without 242 errors` than precc ctrl+C without pressing Enter


