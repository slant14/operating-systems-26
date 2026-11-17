# Askar Aitov — CTF

## CTF1

### 01BackgroundDog

#### 1. Find the stopped process

```bash
ps auxww
```

The `auxww` options use BSD syntax, so no leading hyphen is needed:

- `a` — include processes belonging to other users that have a terminal.
- `u` — use a user-oriented format, showing the owner, CPU and memory usage, and other details.
- `x` — also include processes without a terminal. Together, `a` and `x` select all processes.
- `ww` — remove the output width limit so long command lines are not truncated.

In my attempt, I found this process:

```text
PID 235, STAT T, COMMAND /usr/local/bin/background_dog /tmp/bgdog
```

The `T` status means the process is stopped.

#### 2. Resume the process and get the image path

```bash
kill -CONT 235
```

The process starts printing the path:

```text
/regular_paw_storage/a14e4f694fe0fe62/Background_dog.png
```

To stop the repeated messages:

```bash
kill -STOP 235
```

#### 3. Download the image

```bash
scp -O a.aitov-dog@osctf.nnk.moe:/regular_paw_storage/a14e4f694fe0fe62/Background_dog.png ./Background_dog.png
```


### 01CatDog

#### 1. Find the files

```bash
find / -type f 2>/dev/null | grep -i 'cat.*dog'
```

In my attempt, I found these files:

```text
/home/ctf/go_to_opt/Pictures/Cat_dog_top
/home/ctf/go_to_opt/Pictures/Cat_dog_bottom
```

Open the directory and check the file types:

```bash
cd /home/ctf/go_to_opt/Pictures
file Cat_dog_top Cat_dog_bottom
```

`Cat_dog_top` was identified as a PNG image, 223 × 300 pixels. The `file` command identifies the format from the contents, even when a file has no extension.

#### 2. Combine the files

The challenge name and file names suggest that these are the beginning and continuation of a single PNG. I tested this hypothesis by combining their contents with `cat`, placing `top` before `bottom`:

```bash
cat Cat_dog_top Cat_dog_bottom > Cat_dog.png
```

The original files are preserved, and the result is written to `Cat_dog.png`.

#### 3. Download the image

```bash
scp -O a.aitov-dog@osctf.nnk.moe:/home/ctf/go_to_opt/Pictures/Cat_dog.png .
```


### 01CompileDog

#### 1. Find the source file

```bash
find / -type f 2>/dev/null | grep -i 'compile.*dog'
```

Find the `.c` file in the results and view its contents:

```bash
cat /home/ctf/go_to_opt/Programs/compile_dog.c
```

#### 2. Compile and run

Compile with `gcc` and run the program:

```bash
gcc /home/ctf/go_to_opt/Programs/compile_dog.c -o /tmp/compile_dog
/tmp/compile_dog
```

The program prints the image path:

```text
/regular_paw_storage/959fdabca497976c/Compile_dog.png
```


### 01EditDog

#### 1. Find EDITME

```bash
find / -iname '*edit*' 2>/dev/null
```

I found `/home/ctf/go_to_opt/Doguments/EDITME`.

#### 2. Read and follow the instructions

```bash
cat /home/ctf/go_to_opt/Doguments/EDITME
```

The file contained an ASCII dog and instructions to make it 7 characters longer while preserving its shape and leaving everything outside the boundary lines unchanged.

I opened the file in the editor:

```bash
vim /home/ctf/go_to_opt/Doguments/EDITME
```

I extended both the back and belly by 7 underscores, moved the head and front legs 7 spaces to the right, and saved the file.

After the correct edit, the terminal displayed a confirmation:

```text
Woof! You have solved Edit_dog!
Here's your dog pic: /regular_paw_storage/25ea83848c810c21/Edit_dog.png
```

#### 3. Download the image

```bash
scp -O a.aitov-dog@osctf.nnk.moe:/regular_paw_storage/25ea83848c810c21/Edit_dog.png ./EditDog.png
```


### 01HelloDog

#### 1. Find the image in go_to_opt

```bash
ls -la /home/ctf/go_to_opt
```

The `/home/ctf/go_to_opt` directory contained `hoi_:3.png`. I chose it as a candidate for HelloDog because `hoi` is a greeting.

#### 2. Download the image

```bash
scp -O 'a.aitov-dog@osctf.nnk.moe:/home/ctf/go_to_opt/hoi_:3.png' ./HelloDog.png
```


### 01HistoryDog

#### 1. Find the history file

```bash
find / -iname '*history*' 2>/dev/null
```

Among the results, I found `/home/ctf/.bash_history`, the Bash command history file.

#### 2. Read the history

```bash
cat /home/ctf/.bash_history
```

The history contained a command that copied the image:

```bash
cp /super_secret_doggo_vault/grandpaw.png /regular_paw_storage/881847c02f97ebbb/grandpaw.png
```

The destination path was:

```text
/regular_paw_storage/881847c02f97ebbb/grandpaw.png
```

#### 3. Download the image

```bash
scp -O a.aitov-dog@osctf.nnk.moe:/regular_paw_storage/881847c02f97ebbb/grandpaw.png ./HistoryDog.png
```


### 01PoweredDog

#### 1. Find the directory

Inside the CTF container, I inspected the parent directories:

```bash
cd ..
ls -la
```

Along the way, I found the `go_to_opt` directory. The target directory, `/opt`, is directly under the filesystem root. I can go there directly:

```bash
cd /opt
ls -la
```

#### 2. Read README.md

```bash
cat README.md
```

The file contains the path to the PoweredDog image. Copy this path.

#### 3. Download the image

Replace `/path/from/README.md` in the command below with the full image path given in `README.md`.

```bash
scp -O a.aitov-dog@osctf.nnk.moe:/path/from/README.md ./PoweredDog.png
```


### 01TailheadedDog

#### 1. Find a candidate file

```bash
find / -iname '*dog*' 2>/dev/null
```

The results included this file:

```text
/home/ctf/go_to_opt/Downloads/dog.log
```

The challenge name, `TailheadedDog`, hints at the `tail` and `head` commands, which display the end and beginning of a file. I therefore chose `dog.log` as a candidate to inspect.

#### 2. Read the beginning of the file

```bash
head -n 20 /home/ctf/go_to_opt/Downloads/dog.log
```

At the beginning, I found the first part of the path:

```text
/regular_paw_storage/6688d00d
```

The nearby hint, `I wonder if there is more at the end of it..`, suggested checking the end of the file. The text drawings were decorative.

#### 3. Read the end of the file

I increased the number of lines shown from the end of the file:

```bash
tail -n 50 /home/ctf/go_to_opt/Downloads/dog.log
```

I found the second part:

```text
21a5063f/tailheaded.png
```

I joined both parts without spaces or additional separators:

```text
/regular_paw_storage/6688d00d21a5063f/tailheaded.png
```

#### 4. Download the image

```bash
scp -O a.aitov-dog@osctf.nnk.moe:/regular_paw_storage/6688d00d21a5063f/tailheaded.png ./TailheadedDog.png
```


### 02MuteDog

#### 1. Find the challenge program

I first noticed the program during a general search for files in the container. The program can also be found by searching for part of the challenge name:

```bash
find / -iname '*mute*' 2>/dev/null
```

The matching result was:

```text
/home/ctf/go_to_opt/Programs/mute_dog
```

#### 2. Run the program and read the hint

```bash
cd /home/ctf/go_to_opt/Programs
./mute_dog
```

When run normally, the program displayed a text drawing with the message:

```text
Terminals are listening
```

The name MuteDog suggested investigating the program's output. The message about terminals gave me a more specific direction: check how the program behaves when its output is saved to files.

The program has two separate output streams: standard output (`stdout`) and standard error (`stderr`). Both normally appear in the terminal, but they can be saved separately. Despite its name, standard error can carry any information the program writes to it.

#### 3. Test the hypothesis by redirecting both streams

Inside the container:

```bash
/home/ctf/go_to_opt/Programs/mute_dog > /tmp/mute.out 2> /tmp/mute.err
```

Here, `>` saves `stdout` to `/tmp/mute.out`, while `2>` saves `stderr` to `/tmp/mute.err`.

I read each file separately:

```bash
cat /tmp/mute.out
```

The first file contained:

```text
/regular_paw_storage/72213e3e
```

```bash
cat /tmp/mute.err
```

The second file contained:

```text
1934391a/Mute_dog.png
```

#### 4. Assemble the path and download the image

I joined the parts without spaces or additional separators:

```text
/regular_paw_storage/72213e3e1934391a/Mute_dog.png
```

```bash
scp -O a.aitov-dog@osctf.nnk.moe:/regular_paw_storage/72213e3e1934391a/Mute_dog.png ./MuteDog.png
```


### 02PetDog

#### 1. Find the project and read the instructions

During earlier searches, I found the `auto_petter` directory, which matched the idea behind PetDog: petting a dog.

```bash
cd /home/ctf/go_to_opt/Doguments/auto_petter
cat README.md
```

The README explains that `mascot.png` is assembled from parts in `frames/` and that the final versions of those parts must be found in the Git history. Running `autopet.sh` only performed a petting session and did not provide an image path.

#### 2. Identify the part versions and assembly order

```bash
git log --all --graph --oneline --decorate -n 30
cat tools/assemble_mascot.sh
```

I found the required changes in the history:

- `36c3551` — `mascot: clean re-encode of part1 from source, the import was botched`.
- `542b1d7` — `mascot: swap placeholder part2 for the final encode from design`.
- `18cbdf3` — `mascot: clean re-encode of part3 from source, the import was botched`.

Commits `58c5969` and `adc0d42` are described as experimental changes that corrupt the result (`garbled` and `mangles the output`). I did not use their versions of the parts.

The assembly script specifies this order:

```bash
cat frames/part2.bin frames/part3.bin frames/part1.bin > mascot.png
```

The required parts are therefore the corrected `part1` and `part3` from the `master` history and the final `part2` from the `dev` history.

#### 3. Extract the selected versions

```bash
git show 36c3551:frames/part1.bin > /tmp/pet-part1.bin
git show 542b1d7:frames/part2.bin > /tmp/pet-part2.bin
git show 18cbdf3:frames/part3.bin > /tmp/pet-part3.bin
```

The `commit:path` notation retrieves the file contents from the specified Git revision.

#### 4. Assemble and check the PNG

```bash
cat /tmp/pet-part2.bin /tmp/pet-part3.bin /tmp/pet-part1.bin > /tmp/PetDog.png
file /tmp/PetDog.png
```

I obtained:

```text
/tmp/PetDog.png: PNG image data, 300 x 200, 8-bit/color RGBA, non-interlaced
```

#### 5. Download and submit

```bash
scp -O a.aitov-dog@osctf.nnk.moe:/tmp/PetDog.png ./PetDog.png
```


### 02RemDog

#### 1. Read the instructions

```bash
cd /home/ctf/go_to_opt/Doguments/unsorted
pwd
ls -la
cat README
```

The README contains this instruction:

```text
I need to remove all the angry dogs from this folder some day...
```

This means I need to remove all files depicting angry dogs from this directory.

#### 2. Identify and remove the matching files

I inspected the file contents with `cat`. For example:

```bash
cat 0fbcdfa7
cat 201bc02c
cat 28ed771d
```

The files contained text drawings of dogs. The `ls -la` output also showed two groups of files by size: 1937 and 1678 bytes. All 17 files whose removal solved the challenge in this attempt were 1937 bytes long.

From `/home/ctf/go_to_opt/Doguments/unsorted`, I ran four commands using the specific file names:

```bash
rm -rf 0fbcdfa7 201bc02c 28ed771d 29fb7be6 328ab3db
rm -rf 5f24a2b3 632ec042 6756dcf5
rm -rf 729f8fab
rm -rf 967e7aae 9a968681 9eecde7e a4cba7b0 b9a1eed0 bffb169f cbad2ac6 d436f0c3
```

#### 3. Get the image path

After the last command, a confirmation appeared:

```text
Woof! You have solved Rem_dog!
Here's your dog pic: /regular_paw_storage/d2bf0603c2058192/Rem_dog.png
```

#### 4. Download the image

```bash
scp -O a.aitov-dog@osctf.nnk.moe:/regular_paw_storage/d2bf0603c2058192/Rem_dog.png ./RemDog.png
```


### 02ScatteredDog

#### 1. Find the challenge directory

```bash
find / -iname '*scattered*' 2>/dev/null
cd "/home/ctf/go_to_opt/Videos/Scattered dog"
ls -la
```

The `Scattered dog` directory directly matches the challenge name. Inside, I found 100 files named from `-50` to `49`.

#### 2. Combine the parts

In the current directory, using Bash:

```bash
cat ./{-50..49} > /tmp/dog.png
```

The shell expands `{-50..49}` into a sequence of numbers from `-50` to `49`. The `cat` command combines the file contents in that order, and `>` writes the result to `/tmp/dog.png`.

The `./` prefix refers to files in the current directory and prevents names beginning with a minus sign from being interpreted as command options.

#### 3. Check the result

```bash
file /tmp/dog.png
```

The result was:

```text
/tmp/dog.png: PNG image data, 300 x 195, 8-bit/color RGBA, non-interlaced
```

#### 4. Download the image

```bash
scp -O a.aitov-dog@osctf.nnk.moe:/tmp/dog.png ./ScatteredDog.png
```


### 02ShyDog

#### 1. Find the challenge directory

```bash
find / -iname '*shy*' 2>/dev/null
```

The `/home/ctf/go_to_opt/Videos/Shy dog` directory had also appeared in earlier searches.

```bash
cd "/home/ctf/go_to_opt/Videos/Shy dog"
ls -la
```

The directory contains many files with different extensions and similar names. The file names alone do not identify the correct image.

#### 2. Find the actual PNG image

Next, check the file contents: the `.png` extension alone does not guarantee that a file contains an image.

```bash
file * | grep -i 'PNG image data'
```

The `file` command identifies the type from the contents, while `grep` keeps the results recognized as PNG.

Based on this check, I selected:

```text
IMG_Maybe_Dog_315c73bf.png
```

#### 3. Download the image

```bash
scp -O 'a.aitov-dog@osctf.nnk.moe:/home/ctf/go_to_opt/Videos/Shy\ dog/IMG_Maybe_Dog_315c73bf.png' ./ShyDog.png
```


### 02SpitoonDog

#### 1. Find the program

Inside the CTF container:

```bash
find / -iname '*spitoon*' 2>/dev/null
```

The program had also appeared earlier in the file listing for `Programs`:

```text
/home/ctf/go_to_opt/Programs/spitoon
```

#### 2. Interpret the hint and read the manual

When run without the correct responses, the program displayed:

```text
Welcome to the salty spitoon, how tough are ya?
Listen, kid: come back when you are more of a man
```

The word `man` suggested checking the program's manual:

```bash
man spitoon
```

To display the entire manual without a pager:

```bash
man spitoon | cat
```

The manual provided the exact sequence of responses and instructed me to press Ctrl+C after the second response, without pressing Enter.

#### 3. Pass the test

Run the program interactively, without redirecting its output:

```bash
/home/ctf/go_to_opt/Programs/spitoon
```

At the prompt `Welcome to the salty spitoon, how tough are ya?`, enter this response on a single line:

```text
How tough am I? How tough am I?! I ran 234 of linux programs this lab
```

Press **Enter**.

After the program responds with `Yea, so?`, type:

```text
Without 234 errors
```

**Do not press Enter. Press Ctrl+C immediately.** Do not type `^C` literally; it represents the keyboard shortcut.

Ctrl+C sends the running program a `SIGINT` signal. This normally interrupts the program, but here the program handles it as part of the test.

In my attempt, I received this confirmation:

```text
Right this way sir: /regular_paw_storage/e950823f78da4f65/Spitoon_dog.png, sorry to keep you waiting
```

#### 4. Download and submit

```bash
scp -O a.aitov-dog@osctf.nnk.moe:/regular_paw_storage/e950823f78da4f65/Spitoon_dog.png ./SpitoonDog.png
```


### 03AqueousDog

#### 1. Interpret the hint

The challenge provided an image of a sea creature named `shellfish.png`.

The word `shellfish` can be split into `shell` and `fish`. In the context of a command-line challenge, this suggested trying the **fish** shell.

#### 2. Start fish

```bash
fish
```

After starting the shell, I received this path:

```text
/regular_paw_storage/6c16fd50b2a76c24/Aqueous_dog.png
```

#### 3. Download the image

```bash
scp -O a.aitov-dog@osctf.nnk.moe:/regular_paw_storage/6c16fd50b2a76c24/Aqueous_dog.png ./AqueousDog.png
```


### 03ChainedDog

#### 1. Find the start of the chain

The name ChainedDog suggested looking for a chain. While reviewing an earlier file listing, I noticed `start` and `end` among many `file_....txt` files in `/home/dog/Downloads`.

```bash
cat /home/dog/Downloads/start
```

The directory belongs to the user `dog`: `/home/dog/Downloads`.

#### 2. Identify the structure of the parts

Each file begins with a line pointing to the next file. For example, one link in the chain contains:

```text
Next: file_t27hpod0yr.txt
```

Binary data follows the first line, sometimes preceded by an additional prefix consisting of `Content:` followed by a space.

The `end` file contains `Next: end`, and its data includes the `IEND` marker used at the end of a PNG. This suggested assembling the parts in chain order, including the contents of `end` exactly once.

Simply concatenating the original files would add the `Next:` and `Content:` headers to the image. I therefore remove them during assembly and preserve the binary data byte for byte.

#### 3. Create a C program

Inside the container:

```bash
nano /tmp/chain.c
```

```c
#include <stdio.h>
#include <string.h>

int main(void) {
    char name[256] = "start", next[256];
    char path[512], line[512], prefix[9];
    FILE *out = fopen("/tmp/ChainedDog.png", "wb");

    while (1) {
        sprintf(path, "/home/dog/Downloads/%s", name);
        FILE *in = fopen(path, "rb");

        fgets(line, sizeof(line), in);
        sscanf(line, "Next: %255s", next);

        fread(prefix, 1, 9, in);
        if (memcmp(prefix, "Content: ", 9) != 0)
            fseek(in, -9, SEEK_CUR);

        int c;
        while ((c = fgetc(in)) != EOF)
            fputc(c, out);

        fclose(in);

        if (strcmp(name, "end") == 0)
            break;

        strcpy(name, next);
    }

    fclose(out);
    return 0;
}
```

The program reads the next file name from the first line. It then reads nine bytes: if they match `Content:` followed by a space, the prefix is skipped; otherwise, the read position is moved back. The remaining bytes are appended to the image. The program stops after processing `end`.

#### 4. Compile and assemble the image

```bash
gcc /tmp/chain.c -o /tmp/chain
/tmp/chain
file /tmp/ChainedDog.png
```

The result is written to `/tmp/ChainedDog.png`. Its format can be checked with `file`, and the complete image can be viewed after downloading it.

#### 5. Download the image

```bash
scp -O a.aitov-dog@osctf.nnk.moe:/tmp/ChainedDog.png ./ChainedDog.png
```


### 03InsecureDog

#### 1. Find and read the hint

I found the file by searching for part of its name:

```bash
find /home /opt -iname '*rem*' 2>/dev/null
```

The result was:

```text
/home/ctf/go_to_opt/Doguments/secret_stuff/remember.txt
```

```bash
cat /home/ctf/go_to_opt/Doguments/secret_stuff/remember.txt
```

The hint stated:

- The image is in the `super_secret` directory inside `secret_stuff` in `Doguments`.
- The name consists of four letters chosen from `D`, `O`, and `G`, with one letter appearing twice.
- The extension is `.png`; the examples given were `DOGG.png`, `GOGO.png`, and `DOOG.png`.

I initially considered the file a candidate for RemDog because of the word `remember`. However, obtaining the image solved **03InsecureDog**.

#### 2. Investigate the directory listing restriction

```bash
cd /home/ctf/go_to_opt/Doguments/secret_stuff/super_secret
ls -la
```

I could enter the directory, but `ls` returned `Permission denied`.

#### 3. Find the existing file name

Inside `super_secret`, I ran:

```bash
for pic in {D,O,G}{D,O,G}{D,O,G}{D,O,G}.png; do
  [ -f "$pic" ] && echo "$pic"
done
```

Bash generates 81 four-letter combinations using `D`, `O`, and `G`. These include every name matching the hint. The `[ -f "$pic" ]` check tests a specific file name without listing the directory contents.

I found:

```text
GGOD.png
```

The full path was:

```text
/home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/GGOD.png
```

#### 4. Download the image

```bash
scp -O a.aitov-dog@osctf.nnk.moe:/home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/GGOD.png ./InsecureDog.png
```


### 03NetDog

#### 1. Find the clues

The name NetDog suggested checking for a network service. Earlier output had shown the `/usr/local/bin/netdog` process and the `/usr/share/netdog.png` file.

I did not have permission to download `/usr/share/netdog.png` directly, so I checked whether the running service could provide the image.

#### 2. Identify the open port

I read the TCP socket tables directly from `/proc`:

```bash
cat /proc/net/tcp /proc/net/tcp6
```

The table contained two entries with status `0A`, indicating a listening socket:

```text
local_address       st    uid
00000000:0016       0A    1000
00000000:1296       0A       0
```

The port number after the colon is written in hexadecimal:

- `0016` = 22 — SSH.
- `1296` = 4758 — the second listening port, which I chose to investigate.

If needed, convert the value using this Bash command:

```bash
printf '%d\n' 0x1296
```

The address `00000000` means the service listens on all IPv4 interfaces. To connect from the same container, I used `127.0.0.1`.

#### 3. Retrieve the image through the service

```bash
cat < /dev/tcp/127.0.0.1/4758 > /tmp/NetDog.png
```

`/dev/tcp/address/port` is a special redirection feature in Bash. The shell opens a TCP connection, `cat` reads the service's response, and `>` saves it to a file. No separate networking program was needed.

I checked the result:

```bash
file /tmp/NetDog.png
```

The output was:

```text
/tmp/NetDog.png: PNG image data, 300 x 222, 8-bit/color RGBA, non-interlaced
```

Connecting to the selected port returned a PNG image.

#### 4. Download and submit

In a new terminal **on my Mac**, outside the SSH session:

```bash
scp -O a.aitov-dog@osctf.nnk.moe:/tmp/NetDog.png ./NetDog.png
```

I enter the password for the current container, then open the local image:

```bash
open ./NetDog.png
```

I check the image and send `NetDog.png` to the bot as a file.

If the container is recreated, I need to check the listening ports again and retrieve the image into `/tmp/NetDog.png`.


### 03ProDog

#### 1. Find the process and interpret the hint

```bash
ps auxww
```

The output showed a launcher loop and a short-lived process:

```text
root  ... /bin/bash /usr/local/sbin/prodog_loop
ctf   ... /usr/local/bin/prodog
```

The hint, `TheQuickster.png`, suggested speed. The `prodog` process could finish between checking `ps` and manually entering `kill`, resulting in `No such process`.

#### 2. Catch and pause prodog

```bash
while ! pkill -STOP -x prodog; do sleep 0.1; done
```

The command retries every 0.1 seconds and stops once it successfully sends `SIGSTOP`. The `-x` option matches the exact process name `prodog`.

Once paused, the process remains available for further actions. Find its PID:

```bash
pgrep -x prodog
```

In my attempt, the PID was:

```text
2107
```

#### 3. Terminate the process and check the challenge

In this attempt, terminating the captured process with `SIGKILL` solved the challenge:

```bash
kill -9 2107
```

After this sequence, I received the path:

```text
/regular_paw_storage/ba364c5302510b64/Pro_dog.png
```

#### 4. Download and submit

```bash
scp -O a.aitov-dog@osctf.nnk.moe:/regular_paw_storage/ba364c5302510b64/Pro_dog.png ./ProDog.png
```


### 03RestoreDog

#### 1. Find the recovery file

I had previously found `crashed.txt` and the hidden `.crashed.txt.swp` file in `/home/ctf/go_to_opt/Doguments`:

```bash
ls -la /home/ctf/go_to_opt/Doguments
```

The name `crashed.txt` and the presence of a `.swp` file gave me a specific clue for RestoreDog: Vim can store unsaved changes in a swap file and recover them after a crash.

```bash
cat /home/ctf/go_to_opt/Doguments/crashed.txt
```

The original text contained the recovery key:

```text
097055e70ac113b2
```

#### 2. Recover the text in Vim

```bash
vim -r /home/ctf/go_to_opt/Doguments/crashed.txt
```

The `-r` option starts recovery from a swap file.

I enter the recovery key.

After successfully saving the recovered text, I exit the editor.

I read the recovered file with:

```bash
cat /tmp/restored.txt
```

```text
/regular_paw_storage/oqzxuvp/Restore_dog.png
```

#### 3. Download the image

```bash
scp -O a.aitov-dog@osctf.nnk.moe:/regular_paw_storage/oqzxuvp/Restore_dog.png ./RestoreDog.png
```


### 03SpyDog

#### 1. Find clues in the project

During earlier searches, I found `cat_tracker`, a cat surveillance project. Its name and purpose fit the idea behind SpyDog.

```bash
cd /home/dog/Doguments/cat_tracker
cat README.md suspects.txt
```

The README mentions a `cat_cam/` directory that is missing from the current file listing. The `suspects.txt` file also contains a note about a leaked camera archive:

```text
note: do NOT re-upload the cat cam archive. the fence has seen enough.
```

This prompted me to check the project's history.

#### 2. Check the full Git history

```bash
git -c safe.directory="$PWD" log --all --oneline --name-status -n 20
```

The `-c safe.directory="$PWD"` option allows Git to work with the current repository, owned by another user, for this command only.

I found these messages in the history:

```text
542c774 I hope they are really gone this time
3d6e467 double-check: no frames left anywhere
907f2c2 history rewrite: purge the leaked frames from every commit
b1ef6b4 SECURITY: cat cam archive leaked to the neighbor's fence
```

They indicate that the history was rewritten to remove the frames.

#### 3. Find unreachable objects

```bash
git -c safe.directory="$PWD" fsck --full --no-reflogs --unreachable
```

After a history rewrite, old objects can remain on disk until Git removes them during garbage collection. The command found several unreachable commits and trees, along with two `blob` objects containing file contents:

```text
unreachable blob 08ca2e5e96b523a8db7112cd0fa8ad8c05acdedc
unreachable blob 303e089ca1820947d5ccc48c85dddfbb77313538
```

#### 4. Extract the files and identify their types

In the same repository directory:

```bash
git -c safe.directory="$PWD" cat-file -p 08ca2e5e96b523a8db7112cd0fa8ad8c05acdedc > /tmp/spy1.bin
git -c safe.directory="$PWD" cat-file -p 303e089ca1820947d5ccc48c85dddfbb77313538 > /tmp/spy2.bin
file /tmp/spy1.bin /tmp/spy2.bin
```

I obtained:

```text
/tmp/spy1.bin: data
/tmp/spy2.bin: PNG image data, 300 x 225, 8-bit/color RGBA, non-interlaced
```

The second object turned out to be a PNG. I chose the `.bin` extension when extracting the files; it does not determine the actual content type.

#### 5. Download and submit

```bash
scp -O a.aitov-dog@osctf.nnk.moe:/tmp/spy2.bin ./SpyDog.png
```


### 03UsedDog

#### 1. Find the file

```bash
find /home -iname '*used*dog*' 2>/dev/null
```

I found the file at:

```text
/home/dog/Used_dog.png
```

#### 2. Download the image

```bash
scp -O a.aitov-dog@osctf.nnk.moe:/home/dog/Used_dog.png ./UsedDog.png
```


## CTF2

### cavern

From the initial cavern.c file (which is already compiled in the container), I can see two functions that I can use in my solution: **offering_new()** and **offering_set_secret()**.

The main idea of the program bellow is to find the addresses of this two functions in the binary files by his names by using dlsym() function.

**offering_new** allow me to create the new offer, and  **offering_set_secret** allow me to set the secret to my previously created offer. After all, I can just print the secret from the offer.

```c
#include <dlfcn.h>
#include <stdio.h>

struct offering;

typedef struct offering *(*new_fn)(void);
typedef void (*set_secret_fn)(struct offering *);


#define BUF_SIZE 64
struct offering {
    char guess[BUF_SIZE];
    char secret[BUF_SIZE];
};

static void *find_function(const char *name)
{
    void *address = dlsym(RTLD_DEFAULT, name);

    return address;
}

void run(void)
{
    new_fn make =
        (new_fn)find_function("offering_new");

    set_secret_fn set_secret =
        (set_secret_fn)find_function("offering_set_secret");

    struct offering *offer = make();

    set_secret(offer);

    for(int i = 0; i < 55; ++i) {
        printf("%c", offer->secret[i]);
    }
}
```


### legacy

#### How the code works

The `position()` function computes a number from the characters of a string. It starts with `h = 0` and performs the following operation for each character:

```c
h = h * 67 + *s++;
```

`*s` is the current character. The expression `*s++` reads the character and advances the pointer to the next one. The `while (*s)` loop stops at the null character at the end of the string.

The `add_data()` function uses the result as an array index:

```c
unsigned pos = position(data);
document[pos] = *data;
```

Each string therefore determines both a character and its position. For example, `"d0mKc"` supplies the letter `d`, while `position()` determines where to write it.

The array `char document[69] = {0};` contains 69 elements with indices from 0 to 68.

#### Why the program crashes

Unsigned arithmetic depends on the bit width of the type. For an 18-bit `unsigned`:

```text
2^18 = 262144
Valid values: 0–262143
```

On overflow, unsigned arithmetic wraps modulo `2^18`. On a modern machine, `unsigned` typically has 32 bits, so the intermediate results differ.

For example, with `"d0mKc"`, the original function returns `2030043149` using a 32-bit `unsigned`, but `13` using 18-bit arithmetic. Writing to index `2030043149` goes beyond the bounds of the 69-element array and causes undefined behavior, which manifests as a segmentation fault in this challenge.

#### The fix

To reproduce the original overflow behavior, I take the remainder after division by `262144` at each step.

```c
h = (h * 67 + *s++) % 262144;
```

`%` computes the remainder after division. For the strings chosen in this challenge, the restored indices fall within the array bounds. This is a property of the challenge's input data, not of every hash computed modulo `262144`.

#### Full corrected code

```c
#include <limits.h>
#include <stdio.h>


unsigned position(const char *s)
{
    unsigned h = 0;
    while (*s) {
        h = (h * 67 + *s++) % 262144;
    }
    return h;
}

void diagnostics(void)
{
    printf("== Diagnostics: ==\n");
    printf(" unsigned has %d bits\n", (int)(sizeof(unsigned) * CHAR_BIT));
    printf(" check value: %u\n", position("X5O!P%@AP[4\\PZX54(P^)7CC)7}$EICAR-STANDARD-ANTIVIRUS-TEST-FILE!$H+H*"));
    printf("== End of Diagnostics ==\n\n");
}

void add_data(char* document, const char* data)
{
    unsigned pos = position(data);
    document[pos] = *data;
}

void decrypt_document(void)
{
    char document[69] = {0};

    add_data(document, "d0mKc");
    add_data(document, "d02t9");
    add_data(document, "H0HaR");
    add_data(document, "_0GVv");
    add_data(document, "_0GVq");
    add_data(document, "g1A7T");
    add_data(document, "i0Xhz");
    add_data(document, "_0GVh");
    add_data(document, "10Ikz");
    add_data(document, "_0GVl");
    add_data(document, "l0oTk");
    add_data(document, "O0CDf");
    add_data(document, "w0MrP");
    add_data(document, "e0tqV");
    add_data(document, "00BEw");
    add_data(document, "o0KiL");
    add_data(document, "91CLd");
    add_data(document, "t07CD");
    add_data(document, "d02tH");
    add_data(document, "s1aRc");
    add_data(document, "O0CDO");
    add_data(document, "d02sx");
    add_data(document, "c0ehx");
    add_data(document, "Y1KqE");
    add_data(document, "_0GVc");
    add_data(document, "t07CX");
    add_data(document, "_0GW8");
    add_data(document, "D0djH");
    add_data(document, "41WrJ");
    add_data(document, "o0Ki9");
    add_data(document, "o2tuq");
    add_data(document, "H1zSK");
    add_data(document, "_1yII");

    printf("== Decrypted Document: ==\n");
    printf("cctf{%s}\n", document);
    printf("== End of Decrypted Document ==\n\n");
}

int main()
{
    diagnostics();

    printf("Attempting to decrypt the document...\n");

    decrypt_document();

    return 0;
}
```


### vaslang

#### What caused the problem

The language uses definitions of the form `name: commands` and execution commands of the form `name!`. Numbers are pushed onto the stack; `+` and `*` perform arithmetic modulo 251; `:` duplicates the top of the stack, and `.` removes it. A function call is also stored as an interpreter command.

Originally, `xdef()` looked up the address of a called function as soon as it read the definition. At that point, the `funcs` array contained only previously read functions.

As a result, the following order did not work:

```text
main: letter 0
letter: 65
main!
```

When the first line is read, `letter` is still unknown, even though its definition appears below.

The definitions in the original `inp.vas` were shuffled. For example, the first line used `cool_357`, which was not defined until line 1314.

#### The idea behind the fix

I introduce a new command type, `U`, meaning unresolved: a call whose address is not yet known.

When reading a definition, I save the name of the called function. When an execution command is encountered, the interpreter goes through the stored commands, finds the functions by name, fills in their addresses, and changes type `U` to a regular call of type `C`.

#### Change 1. Add a name to struct term

**Location:** the `struct term` declaration at the beginning of `vaslang.c`.

**Before:**

```c
struct term {
    char ty;
    union {
        struct term *target;
        unsigned char val;
    };
};
```

**After:**

```c
struct term {
    char ty;
    union {
        struct term *target;
        unsigned char val;
    };
    char *name;
};
```

I added the `char *name` field after the `union`. It stores the function name for a command of type `U`. The `target` field still stores the address of the function's first command for a call of type `C`.

#### Change 2. Save the name in xdef() instead of looking it up immediately

**Location:** the final `else` branch inside the loop that parses the function body in `xdef()`. It handles words that are neither numbers nor operators.

**Before:**

```c
        } else {
            ftermp->ty = 'C';
            for (int i = 0; funcs + i < funcp; i++)
                if (strcmp(funcs[i].name, term) == 0)
                    ftermp->target = funcs[i].body;
            if (ftermp->target == NULL)
                errorf("func %s undefined", term);
        }
```

The old code immediately marked the command as a call of type `C`, iterated over the functions already defined, and compared their names with the current word, `term`. If a match was found, it saved the body's address in `target`. If no address was found, it reported an error.

**After:**

```c
        } else {
            ftermp->ty = 'U';
            ftermp->name = term;
        }
```

#### Change 3. Resolve the saved names in xcmd()

**Location:** the `xcmd()` function, which is called when a command containing `!` is read.

**Before — the entire function:**

```c
void xcmd(char* s)
{
    char *name = strsep(&s, "!");
    for (int i = 0; funcs + i < funcp; i++)
        if (strcmp(funcs[i].name, name) == 0)
            return run(funcs[i].body);
    errorf("func %s undefined", name);
}
```

**After — the entire function:**

```c
void xcmd(char* s)
{
    char *name = strsep(&s, "!");

    for (struct term *p = terms; p < termp; p++) {
        if (p->ty != 'U')
            continue;

        for (int i = 0; funcs + i < funcp; i++) {
            if (strcmp(funcs[i].name, p->name) == 0) {
                p->target = funcs[i].body;
                p->ty = 'C';
                break;
            }
        }
        if (p->ty == 'U')
            errorf("func %s undefined", p->name);
    }

    for (int i = 0; funcs + i < funcp; i++)
        if (strcmp(funcs[i].name, name) == 0)
            return run(funcs[i].body);
    errorf("func %s undefined", name);
}
```

I added a loop that finds function addresses for deferred calls. It runs when an execution command such as `main!` is encountered, rather than on every function call within the program.

#### Build and verification

After compiling the interpreter and testing it with the original program, I confirmed that the program ran to completion and produced the flag.


### xorcism

The main check in the original source file is:

```c
if ((unsigned char)in[i] != (enc[i] ^ key[i % 4])) {
    puts("nope");
    return 1;
}
```

For the password to be accepted, each character must equal `enc[i] ^ key[i % 4]`. I can therefore calculate the password directly from the arrays using the same formula.

`i % 4` is the remainder when the index is divided by four. It makes the key repeat:

```text
i:      0 1 2 3 4 5 6 7 ...
i % 4:  0 1 2 3 0 1 2 3 ...
```

#### Solution code

```c
#include <stdio.h>
#include <string.h>

#define PWLEN 33

static const unsigned char enc[PWLEN] = {
    0x47, 0x6d, 0x3f, 0x6e, 0x44, 0x12, 0x05, 0x4b, 0x58, 0x4f, 0x2e, 0x50, 0x71, 0x64, 0x69, 0x4b, 0x68, 0x55, 0x32, 0x6a, 0x68, 0x4f, 0x3b, 0x3a, 0x56, 0x4d, 0x05, 0x6b, 0x52, 0x6c, 0x35, 0x41, 0x02
};

static const unsigned char key[4] = { 0x37, 0x21, 0x5a, 0x0f };

int main() {
    char result[PWLEN + 1];

    for (int i = 0; i < PWLEN; ++i) {
        result[i] = (char)(enc[i] ^ key[i % 4]);
    }

    result[PWLEN] = '\0';
    printf("%s", result);
    return 0;
}
```


### zhaitie

#### Interface described in AGENTS.md

The required sequence is:

1. Call `zhaitie_init()` once before using any other library functions.
2. Obtain a decompression context using `razzhaitie_new()`.
3. Read the first **24 bytes** of the compressed file.
4. Call `razzhaitie(ctx, buffer, size, write_output)`.
5. The returned number specifies the size of the next input block. Read exactly that many bytes and call the function again with the same context.
6. When the function returns `0`, decompression is complete. By that point, the entire output has been delivered through the callback.

My solution implements the algorithm described in `AGENTS.md`.

#### How the callback works

`razzhaitie()` returns the size of the next input block. It delivers the decompressed data separately by calling my `write_output(data, length)` function.

Inside `write_output()`, `fwrite()` immediately writes the received bytes to the output file. The `data` pointer must not be saved for later use because the memory belongs to the library.

The callback does not accept an extra argument for passing the file, so I declared `output_file` outside the functions, making it accessible to both `main()` and `write_output()`.

#### Full code

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct razzhaitie_ctx razzhaitie_ctx;
typedef void (*razzhaitie_output_cb)(const unsigned char *data, int length);

void zhaitie_init(void);
razzhaitie_ctx *razzhaitie_new(void);
int razzhaitie(razzhaitie_ctx *ctx, const unsigned char *data, int length, razzhaitie_output_cb output);

static FILE *output_file;

static void write_output(const unsigned char *data, int length)
{
    fwrite(data, 1, length, output_file);
}

int main(void)
{
    FILE *input_file = fopen("test.zai", "rb");
    output_file = fopen("presentation.bin", "wb");

    zhaitie_init();
    razzhaitie_ctx *ctx = razzhaitie_new();

    unsigned char *buffer = NULL;
    int size = 24;

    while (size > 0) {
        buffer = realloc(buffer, size);
        if (fread(buffer, 1, size, input_file) != (size_t)size)
            break;
        size = razzhaitie(ctx, buffer, size, write_output);
    }

    free(buffer);
    fclose(input_file);
    fclose(output_file);
    return 0;
}
```

##### Reading blocks

`size` is initially `24`. On each iteration, `realloc()` allocates or resizes the buffer to the required size. On the first call, `buffer == NULL`, so `realloc()` performs the initial memory allocation.

`fread()` reads the next block from the current file position and advances that position. After the call to `razzhaitie()`, `size` holds the size of the next block.


## CTF3

### decrypt

The core idea is to give 10 trials to each shard to be executes. Then choose the one which is successfully done his job. Then just sum all succesfull result in one file, which is flag.

```c
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

#define SHARDS 50
#define ATTEMPTS 10
#define MAX_PROCESSES (SHARDS * ATTEMPTS)

int main()
{
    pid_t pids[MAX_PROCESSES];

    for (int i = 0; i < MAX_PROCESSES; i++) {
        pids[i] = fork();

        if (pids[i] == -1) {
           return 1;
        }

        if (pids[i] == 0) {
            char shard[32];
            char temporary[32];
            snprintf(shard, sizeof(shard), "%d.shard", i / ATTEMPTS);
            snprintf(temporary, sizeof(temporary), "%d.tmp.txt", i + 1);

            freopen(temporary, "w", stdout);

            execl("./decrapinator-universal-darwin",
                  "decrapinator-universal-darwin",
                  "decrypt", shard, (char *)NULL);
            _exit(1);
        }

    }


    for (int i = 0; i < MAX_PROCESSES; i++) {
        int status;
        waitpid(pids[i], &status, 0);

        char temporary[32];
        char output[32];
        snprintf(temporary, sizeof(temporary), "%d.tmp.txt", i + 1);
        snprintf(output, sizeof(output), "%d.txt", i / ATTEMPTS);

        if (status == 0) {
            rename(temporary, output);
        }
    }


    char flag[1000];
    int length = 0;

    for (int i = 0; i < SHARDS; i++) {
        char filename[32];
        snprintf(filename, sizeof(filename), "%d.txt", i);

        FILE *file = fopen(filename, "r");

        int ch;
        while ((ch = fgetc(file)) != EOF) {
            flag[length++] = (char)ch;
        }

        fclose(file);
    }

    flag[length] = '\0';
    printf("Flag: %s\n", flag);

    return 0;
}
```


### discodetective — automatic defense and evidence-based proof

#### Defense protocol

The server provides the exact response in a machine-readable format:

```text
!!THOUGHT-STRIKE sigil=4d1f type=IMPULSE flavor=PARANOIA verb=REBUKE form=kebab action="revoke your own production access" window_ms=3000
!!WARD-WINDOW ms=3000 count=1
```

I need to transform `action` and send:

```text
WARD 4d1f REBUKE revoke-your-own-production-access
```

Transformation formats:

- `kebab`: words are separated by hyphens; the code converts letters to lowercase and replaces non-alphanumeric characters with `-`.
- `reversed`: the words in kebab form appear in reverse order.
- `shout`: the kebab form is converted to uppercase.

#### My C code

```c
#include <arpa/inet.h>
#include <ctype.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static void answer(int socket_fd, char *line)
{
    static char wards[16][2048];
    static int pending;

    if (strstr(line, "!!THOUGHT-STRIKE ") == line) {
        char sigil[32], verb[32], form[32], action[1024], formed[1024];
        sscanf(line, "!!THOUGHT-STRIKE sigil=%31s type=%*s flavor=%*s "
                     "verb=%31s form=%31s action=\"%1023[^\"]\"",
               sigil, verb, form, action);

        for (int i = 0; action[i]; i++) {
            unsigned char c = (unsigned char)action[i];
            action[i] = isalnum(c) ? (char)tolower(c) : '-';
        }
        strcpy(formed, action);

        if (!strcmp(form, "shout"))
            for (int i = 0; formed[i]; i++)
                formed[i] = (char)toupper((unsigned char)formed[i]);

        if (!strcmp(form, "reversed")) {
            char *words[512];
            int count = 0;
            for (char *p = strtok(action, "-"); p; p = strtok(NULL, "-"))
                words[count++] = p;
            formed[0] = '\0';
            while (count) {
                strcat(formed, words[--count]);
                if (count)
                    strcat(formed, "-");
            }
        }

        sprintf(wards[pending++], "WARD %s %s %s\n", sigil, verb, formed);
    }

    if (strstr(line, "!!WARD-WINDOW ") == line) {
        for (int i = 0; i < pending; i++) {
            write(socket_fd, wards[i], strlen(wards[i]));
            printf("[auto] %s", wards[i]);
        }
        pending = 0;
    }
}

int main(void)
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_port = htons(9999)
    };
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);
    connect(socket_fd, (struct sockaddr *)&address, sizeof(address));

    struct pollfd inputs[2] = {
        {.fd = socket_fd, .events = POLLIN},
        {.fd = STDIN_FILENO, .events = POLLIN}
    };
    char buffer[4096], line[8192];
    int length = 0;
    setbuf(stdout, NULL);

    for (;;) {
        poll(inputs, 2, -1);
        if (inputs[0].revents & (POLLIN | POLLHUP)) {
            ssize_t count = read(socket_fd, buffer, sizeof(buffer));
            if (count <= 0)
                break;
            fwrite(buffer, 1, count, stdout);
            for (ssize_t i = 0; i < count; i++) {
                line[length++] = buffer[i];
                if (buffer[i] == '\n') {
                    line[length] = '\0';
                    answer(socket_fd, line);
                    length = 0;
                }
            }
        }
        if (inputs[1].revents & (POLLIN | POLLHUP)) {
            ssize_t count = read(STDIN_FILENO, buffer, sizeof(buffer));
            if (count <= 0)
                break;
            write(socket_fd, buffer, count);
        }
    }
}
```


### fortforks — reading a FIFO through a chain of fork() calls

#### The problem

The flag was at the end of a long stream from the FIFO `/home/ctf/secrets`. Processes I started were killed after five seconds. Losing the FIFO connection restarted the transmission, so I needed to keep reading beyond a single process's lifetime.

#### How my C code works

I open the FIFO in nonblocking mode and save incoming data to `/home/ctf/dump.txt`. Every four seconds, `fork()` creates a child that inherits the open file descriptors. The parent exits, while the child resets its timer and continues reading through the same connection. This preserves the stream while keeping each process below the time limit. The code briefly waits when no data is available and stops at EOF after data has been received.

```c
#define _POSIX_C_SOURCE 200809L

#include <fcntl.h>
#include <time.h>
#include <unistd.h>

static double now(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int main(void)
{
    double started = now();
    int fd = open("/home/ctf/secrets", O_RDONLY | O_NONBLOCK);
    int out_fd = open("/home/ctf/dump.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    char buffer[65536];
    int seen_data = 0;

    for (;;) {
        if (now() - started >= 4.0) {
            if (fork())
                _exit(0);
            started = now();
        }

        ssize_t count = read(fd, buffer, sizeof(buffer));
        if (count == 0 && seen_data)
            break;
        if (count <= 0) {
            struct timespec pause = {.tv_nsec = 10000000};
            nanosleep(&pause, NULL);
            continue;
        }
        seen_data = 1;

        for (ssize_t offset = 0; offset < count;)
            offset += write(out_fd, buffer + offset, count - offset);
    }
}
```


### needle

#### How the program works

The program creates a thread named `retrieval` that reads the flag into allocated memory:

```c
static void* retrieve_the_fabric(void* arg) {
    char* flag_fabric_storage = malloc(256);
    FILE* f = fopen("/root/flag.txt", "r");
    if (f) {
        fgets(flag_fabric_storage, 256, f);
        fclose(f);
    }
    sleep(1);
    return flag_fabric_storage;
}
```

The program then loads my library and obtains the address of the `meeting` function:

```c
void* lib = dlopen("./libmeeting.so", RTLD_NOW);
meeting_func_t meeting = (meeting_func_t)dlsym(lib, "meeting");
pthread_create(&meeting_occurence, NULL, meeting, NULL);
```

The expected function signature is:

```c
void* meeting(void* arg);
```

#### How I found the flag path

I found the flag path directly in the provided source code in this call:

```c
fopen("/root/flag.txt", "r");
```

#### Why my library can read root's file

I observed that the challenge program runs with root privileges. In the provided source code, privileges are not dropped before loading the library and calling `meeting()`.

`dlopen()` loads the library into the current process, and `pthread_create()` runs my function in a thread within that same process. The `meeting()` code therefore runs with the challenge program's effective privileges and can open `/root/flag.txt` directly.

#### Code

```c
#include <stdio.h>
#include <unistd.h>

void* meeting(void* arg)
{
    (void)arg;

    char flag[256];
    FILE* f = fopen("/root/flag.txt", "r");

    if (fgets(flag, sizeof(flag), f) != NULL) {
        fputs(flag, stdout);
    }

    fclose(f);
    return NULL;
}
```

### princess

#### 1. Inspect the processes

I started with:

```bash
ps auxww
```

The output showed cabins, living princesses, and zombies:

```text
the_far_cabin princess_moth
[princess_moth] <defunct>
```

However, `ps auxww` does not show `PPID`, so I switched to the following command to identify parent-child relationships:

```bash
ps -eo pid,ppid,stat,args --forest
```

The columns are:

- `PID` — the process's own ID.
- `PPID` — its parent process's ID.
- `STAT` — the state; `Z` means zombie, while `S` means a sleeping but living process.
- `COMMAND` — the process's command line; `--forest` displays the parent-child tree.

#### 2. Find the zombie princess's parent

An example from my successful attempt:

```text
    PID    PPID STAT COMMAND
    655       1 S    the_near_cabin princess_heart
    656     655 Z     \_ [princess_heart] <defunct>
    657     655 S     \_ the_axe
    658     655 S     \_ the_locket
    659     655 S     \_ the_gem
```

Princess `656` is already dead: its state is `Z`. Its `PPID` is `655`, so the target cabin is process **655**.

#### 3. Terminate the target cabin

For the example above:

```bash
kill -TERM 655
```

After running the command, I checked the tree again:

```bash
ps -eo pid,ppid,stat,args --forest
```

#### 4. Repeat manually until confirmation

I repeated two steps:

1. Inspect the tree and find a child process named `princess_*` with state `Z`.
2. Send `SIGTERM` to its parent cabin.

When the latest output showed several matching cabins, I could terminate them with a single command. The last such command in my attempt was:

```bash
kill -TERM 1193 1185 1176
```

#### 5. Retrieve the flag

After the final command, a confirmation appeared:

```text
=== The Forest ===
The forest falls silent. 50 cursed cabins lie in ruin, and
the story is finally over. Your reward waits in your home
directory.
```

I checked my home directory and read the flag:

```bash
ls
cat flag.txt
```

### princess2 — automatically destroying cursed cabins in C

#### How my code works

- `for (;;)` repeats the search until the flag is found.
- `fopen()`, `fread()`, and `fwrite()` check `/home/ctf/flag.txt`. If the file contains data, I print it and finish with `return 0`.
- `opendir()` and `readdir()` scan `/proc`. `atoi()` converts directory names into PIDs; `pid <= 1` skips nonnumeric entries and PID 1.
- `/proc/<PID>/stat` contains process information. `fscanf()` skips the PID and reads the process name, state, and parent PID into `name`, `state`, and `parent`.
- The `if` condition selects zombie processes (`Z`) whose names begin with `princess_` and whose parent PID is greater than 1. `kill(parent, SIGTERM)` terminates the parent cabin, not the zombie princess.
- `closedir()` closes the directory, and `nanosleep()` waits 0.1 seconds before the next scan to reduce CPU usage.

```c
#include <dirent.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(void)
{
    for (;;) {
        char text[1024];
        FILE *flag = fopen("/home/ctf/flag.txt", "r");
        if (flag) {
            size_t size = fread(text, 1, sizeof(text), flag);
            fclose(flag);
            if (size) {
                fwrite(text, 1, size, stdout);
                return 0;
            }
        }

        DIR *directory = opendir("/proc");
        struct dirent *entry;
        while ((entry = readdir(directory))) {
            int pid = atoi(entry->d_name);
            if (pid <= 1)
                continue;

            char path[64], state = 0, name[64] = "";
            int parent = 0;
            sprintf(path, "/proc/%d/stat", pid);
            FILE *file = fopen(path, "r");
            if (!file)
                continue;
            fscanf(file, "%*d (%63[^)]) %c %d", name, &state, &parent);
            fclose(file);

            if (state == 'Z' && parent > 1 && !strncmp(name, "princess_", 9))
                kill(parent, SIGTERM);
        }
        closedir(directory);

        struct timespec pause = {.tv_nsec = 100000000};
        nanosleep(&pause, NULL);
    }
}
```
