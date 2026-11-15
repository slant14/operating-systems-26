## Challenge 1 - 01HelloDog
I connected to my VM and found a hoi_:3.png, which was suspecious. 
   ```
   ctf@81f864f54cfb$ ls
   Doguments  Downloads  Pictures  Programs  Videos  hoi_:3.png
   ```
I download it using scp:
   ```
scp d.naboishchikov-dog@osctf.nnk.moe:"/home/ctf/go_to_opt/hoi_:3.png" ~/cctf/
   ```
I've got the file and send it to bot
   ![Screenshot](images/Pasted%20image%2020261109130427.png)

## Challenge 2 - 02ScatteredDog
In `/home/ctf/go_to_opt/Videos/Scattered dog` I found many files with some binary code
```
-1   -13  -17  -20  -24  -28  -31  -35  -39  -42  -46  -5   -8  10  14  18  21  25  29  32  36  4   43  47  6
-10  -14  -18  -21  -25  -29  -32  -36  -4   -43  -47  -50  -9  11  15  19  22  26  3   33  37  40  44  48  7
-11  -15  -19  -22  -26  -3   -33  -37  -40  -44  -48  -6   0   12  16  2   23  27  30  34  38  41  45  49  8 
-12  -16  -2   -23  -27  -30  -34  -38  -41  -45  -49  -7   1   13  17  20  24  28  31  35  39  42  46  5   9
```
I decided that this is a png code, splitted to files, so I wrote a C code, which opens all files and join all code from each file into one file, which I downloaded and added `.png` expansion and sended to bot.
```c
#include <stdio.h>  
#include <stdlib.h>  
  
int main() {  
    FILE* res = fopen("res.txt", "wb");  
  
    for (int i =-50; i<= 50; i++) {  
        char name[5];  
        sprintf(name, "%d", i);  
        FILE* file = fopen(name, "rb");  
        if (file == NULL) {continue;}  
        fseek(file,0, SEEK_END);  
        long size = ftell(file);  
        fseek(file,0, SEEK_SET);  
        char* buffer = malloc(8*size);  
  
        fread(buffer, 1, size, file);  
        fclose(file);  
        fwrite(buffer, 1, size, res);  
        free(buffer);  
    }  
    fclose(res);  
}
```

## Challenge 3 - 01CatDog
I used cat and >> to join two parts of files in one
``` c
ctf@81f864f54cfb$ cd Pictures/
ctf@81f864f54cfb$ ls
Cat_dog_bottom  Cat_dog_top
ctf@81f864f54cfb$ cat Cat_dog_top >> new_cat
ctf@81f864f54cfb$ cat Cat_dog_bottom >> new_cat
ctf@81f864f54cfb$
```

## Challenge 4 - 01TailheadedDog
There was a big logs file in downloads, so I used head and tail commands to look ta its start and end to get two parts of a path to the picture and downloaded it and sended to the bot.
```
ctf@81f864f54cfb$ ls
dog.log
ctf@81f864f54cfb$ head -n 15 dog.log
Look, a dog image path:
/regular_paw_storage/946c28cf

I wonder if there is more at the end of it..

⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⣿⣿⣦⡀⠀⠀⠀⠀⠀⢀⣠⣤⣄⡀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⣿⡄⠀⠀⠀⣴⣿⣿⣿⣿⣿⡆⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣧⠀⠀⣼⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⠀⢰⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀
⠀⣠⣴⣦⣤⡀⠀⠈⢿⣿⣿⣿⣿⣿⡏⠀⢸⣿⣿⣿⣿⣿⣿⡟⠀⠀⠀⠀⠀⠀
⣸⣿⣿⣿⣿⣿⣆⠀⠈⠻⢿⣿⣿⠟⠀⠀⠈⢿⣿⣿⣿⣿⠟⠀⠀⠀⠀⠀⠀⠀
⣿⣿⣿⣿⣿⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠀⠀⠀⣠⣶⣾⣷⣦⡀
⢻⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⣿⣿⣿⣧
⠈⢿⣿⣿⣿⣿⣿⡇⠀⠀⠀⣀⣤⣶⣶⣶⣤⡀⠀⠀⠀⠀⣾⣿⣿⣿⣿⣿⣿⡟
⠀⠀⠀⠙⠿⢿⠿⠋⠀⢀⣴⣾⣿⣿⣿⣿⣿⣿⣿⣷⡀⠀⠀⣿⣿⣿⣿⣿⣿⡿⠁
ctf@81f864f54cfb$ tail -n 15 dog.log

aab7294e/tailheaded.png

I wonder if there is more at the start of it..

░▄▀▄▀▀▀▀▄▀▄░░░░░░░░░
░█░░░░░░░░▀▄░░░░░░▄░
█░░▀░░▀░░░░░▀▄▄░░█░█
█░▄░█▀░▄░░░░░░░▀▀░░█
█░░▀▀▀▀░░░░░░░░░░░░█
█░░░░░░░░░░░░░░░░░░█
█░░░░░░░░░░░░░░░░░░█
░█░░▄▄░░▄▄▄▄░░▄▄░░█░
░█░▄▀█░▄▀░░█░▄▀█░▄▀░
░░▀░░░▀░░░░░▀░░░▀░░░
ctf@81f864f54cfb$
```

## Challenge 5 - 01BackgroundDog
When I entered the system I saw the stopped process:
`[1]+  Stopped                 /usr/local/bin/background_dog /tmp/bgdog`
And decided to use `fg %1` to continue its execution in foreground. 
![Screenshot](images/Pasted%20image%2020261110144544.png)
I downloaded picture by this path and send it to the bot.

## Challenge 6 - 03InsecureDog
In `/home/ctf/go_to_opt/Doguments/secret_stuff` I found a tip about file in `super_secret` directory, where I have no direct access, but i had a pattern of the name of the file, which looks like `XXXX.png`, where X in {D, O, G}, so I wrote a C language script to iterate all names and try to get it.
```c
#include <stdio.h>  
#include <stdlib.h>  
  
int main() {  
  
    char letters[3] = {'D', 'O', 'G'};  
    for (int a=0; a < 3; a++) {  
        for (int b=0; b < 3; b++) {  
            for (int c=0; c < 3; c++) {  
                for (int d=0; d < 3; d++) {  
                    char name[25];  
                    sprintf(name, "super_secret/%c%c%c%c.png",  
                            letters[a], letters[b],  
                            letters[c], letters[d]);  
                    FILE *file = fopen(name, "rb");  
  
                    if (file != NULL) {  
                        printf("%s", name);  
                        fclose(file);  
                    }  
                }  
            }  
        }  
    }  
}
```
I obtained ODGG.png, dowloaded it and sended to the bot

## Challenge 7 - 01CompileDog
I found non-compiled C code in `/home/ctf/go_to_opt/Programs`, file `compile_dog.c ` and compiled it via `gcc compile_dog.c -o res` and executed it via `./res`. It printed the path `/regular_paw_storage/e3d4107f4f32ed14/Compile_dog.png`, which I downloaded and sended to the bot.

## Challenge 8 - 01HistoryDog
![Screenshot](images/Pasted%20image%2020261110155359.png)
I used `history` to print the bash history and found the command of copying super secret dog to ` /regular_paw_storage/bf71ed046bb1946c/grandpaw.png` on line 13 and downloaded and sended to the bot.

## Challenge 9 - 01EditDog
In `/home/ctf/go_to_opt/Doguments` I found `EDITME` file with dog, initially it looked like:
![Screenshot](images/Pasted%20image%2020261110160000.png)
![Screenshot](images/Pasted%20image%2020261110155917.png)
So I added 9 paces on lines with legs and head and 9 `_` to body part and autochecker sended the `/regular_paw_storage/220e98823520b07c/Edit_dog.png` , so I downloaded and sended to the bot.

## Challenge 10 - 02RemDog
In `/home/ctf/go_to_opt/Doguments/unsorted` I was asked to delete all angry dogs. I started opening files via `nano` and detected that all angry and normal dogs the same, so I decided to use grep and copied the path of the nose of the angry dog and tryied to find all files wich such part via `grep -rl -e 'd8888888888b\.' /home/ctf/go_to_opt/Doguments/unsorted`, where `r` allows recursion, `l` prints filenames, `e` means that next is a pattern.
![Screenshot](images/Pasted%20image%2020261110162009.png)
And removed all found files and obtained the answer image. 

## Challenge 11 - 03RestoreDog
I found a `crashed.txt` file in `/home/ctf/go_to_opt/Doguments` with recovery code `cf5c714a988f7f61`, so I used `vim` and choose `R - recovery option` and recovered the crashed file using Vim to obtain recovered file which contained the path `/regular_paw_storage/0qgxyby/Restore_dog.png`.

## Challenge 12 - 02SpitoonDog
I found `spitoon` file in `/home/ctf/go_to_opt/Programs` and tryied to run it, but could not answer the question correctly, so program fail message made a tip to check `man spitoon` to know more about this program. I found the right way to solve the task: on the 1st question answer `How tough am I? How tough am I?! I ran 154 of linux programs this lab`, after type `Without 154 errors` and press Ctrl+C to send SIGINT signal, but not press Enter. Ctrl+C sends SIGINT to the foreground process, while Enter would submit the input normally through stdin.
![Screenshot](images/Pasted%20image%2020261110170512.png)