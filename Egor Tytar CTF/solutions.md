## Dog CTF

### 01BackgroundDog
- **Artifact:** Stopped background job: `/usr/local/bin/background_dog /tmp/bgdog`
- **Method:** So right after logging into the shell, we see there is already a stopped job `[1]+ Stopped /usr/local/bin/background_dog`. It was paused with `SIGSTOP`. So we just bring it back to life by sending `SIGCONT` with `bg 1` to run it in the background, and it immediately finishes and prints our storage path `/regular_paw_storage/f17aee5f6b5d32c8/Background_dog.png`.
- **Commands:**
  ```sh
  bg 1
  ```
- **Output:**
  ```text
  [1]+ /usr/local/bin/background_dog /tmp/bgdog &
  /regular_paw_storage/f17aee5f6b5d32c8/Background_dog.png
  ```
- **Local Download:**
  ```sh
  scp -O e.tytar-dog@osctf.nnk.moe:/regular_paw_storage/f17aee5f6b5d32c8/Background_dog.png "<target_folder>"
  ```
- **Flag Status:** Accepted.

---

### 01CatDog
- **Artifact:** `/home/ctf/go_to_opt/Pictures/Cat_dog_top` and `/home/ctf/go_to_opt/Pictures/Cat_dog_bottom`
- **Method:** So in the Pictures directory, we find two files: `Cat_dog_top` and `Cat_dog_bottom`. Basically, the PNG image was cut in half—the top file has the PNG magic header and the bottom file has the rest with the IEND footer. So we just use `cat` to concatenate them in order into `cat_dog.png`, and `file` confirms it is a valid PNG.
- **Commands:**
  ```sh
  cat "/home/ctf/go_to_opt/Pictures/Cat_dog_top" "/home/ctf/go_to_opt/Pictures/Cat_dog_bottom" > "/home/ctf/cat_dog.png"
  file "/home/ctf/cat_dog.png"
  ```
- **Output:**
  ```text
  /home/ctf/cat_dog.png: PNG image data, 223 x 300, 8-bit/color RGBA, non-interlaced
  ```
- **Local Download:**
  ```sh
  scp -O e.tytar-dog@osctf.nnk.moe:/home/ctf/cat_dog.png "<target_folder>"
  ```
- **Flag Status:** Accepted.

---

### 01CompileDog
- **Artifact:** `/home/ctf/go_to_opt/Programs/compile_dog.c`
- **Method:** So inside the Programs folder, there is a C source file called `compile_dog.c`. We just compile it with `gcc compile_dog.c -o compile_dog` and run `./compile_dog`. It executes and prints out the direct storage path to our picture.
- **Commands:**
  ```sh
  gcc -o /tmp/compile_dog /home/ctf/go_to_opt/Programs/compile_dog.c && /tmp/compile_dog
  ```
- **Output:**
  ```text
  /regular_paw_storage/2b27b69caff3023e/Compile_dog.png
  ```
- **Local Download:**
  ```sh
  scp -O e.tytar-dog@osctf.nnk.moe:/regular_paw_storage/2b27b69caff3023e/Compile_dog.png "<target_folder>"
  ```
- **Flag Status:** Accepted.

---

### 01EditDog
- **Artifact:** `/home/ctf/go_to_opt/Doguments/EDITME`
- **Method:** So inside `Doguments/EDITME`, there is an ASCII drawing of a dog with a note saying to make it 4 characters longer without breaking the drawing. So we open it in `nano`, and edit. As soon as we save it, a background daemon validates the art and prints the link to our picture.
- **Commands:**
  Inspect original file:
  ```sh
  cat /home/ctf/go_to_opt/Doguments/EDITME
  ```
  Edit with `nano`:
  ```sh
  nano /home/ctf/go_to_opt/Doguments/EDITME
  ```
  Original dog body:
  ```text
                           .-.
       (__________________()6 `-,
       (   ______________   /''"`
       //\\             //\\
  jgs  "" ""            "" ""
  ```
  Modified dog body:
  ```text
                               .-.
       (______________________()6 `-,
       (   __________________   /''"`
       //\\                 //\\
  jgs  "" ""                "" ""
  ```
- **Output:**
  ```text
  Woof! You have solved Edit_dog!
  Here's your dog pic: /regular_paw_storage/93673146eedfde60/Edit_dog.png
  ```
- **Local Download:**
  ```sh
  scp -O e.tytar-dog@osctf.nnk.moe:/regular_paw_storage/93673146eedfde60/Edit_dog.png "<target_folder>"
  ```
- **Flag Status:** Accepted.

---

### 01HelloDog
- **Artifact:** `/home/ctf/go_to_opt/hoi_:3.png`
- **Method:** So right when we SSH into the machine, we are dropped into `/home/ctf/go_to_opt`. We just run `ls` and immediately see a picture named `hoi_:3.png` sitting right there in the directory.
- **Local Download:**
  ```sh
  scp -O e.tytar-dog@osctf.nnk.moe:/home/ctf/go_to_opt/hoi_:3.png "<target_folder>"
  ```
- **Flag Status:** Accepted.

---

### 01HistoryDog
- **Artifact:** `/home/ctf/.bash_history`
- **Method:** So here we need to find where the image went, and the first place to check is the shell history. When we inspect `/home/ctf/.bash_history`, we see that an earlier command copied `grandpaw.png` from `/super_secret_doggo_vault/` to `/regular_paw_storage/7b601ce976cdc28d/grandpaw.png`. So we just grab the image from that storage path.
- **Commands:**
  ```sh
  cat /home/ctf/.bash_history
  ```
- **Output:**
  ```text
  cp /super_secret_doggo_vault/grandpaw.png /regular_paw_storage/7b601ce976cdc28d/grandpaw.png
  ```
- **Local Download:**
  ```sh
  scp -O e.tytar-dog@osctf.nnk.moe:/regular_paw_storage/7b601ce976cdc28d/grandpaw.png "<target_folder>"
  ```
- **Flag Status:** Accepted.

---

### 01PoweredDog
- **Artifact:** `/opt/README.md`
- **Method:** So the landing directory is named `go_to_opt`, which is basically a hint telling us to go to `/opt`. When we `cd /opt` and check `ls`, there is a `README.md`. We `cat README.md` and inside it gives us the paw storage path to `powered_dog.png`.
- **Commands:**
  ```sh
  cat /opt/README.md
  ```
- **Output:**
  ```text
  Congrats on discovering your address!
  Here is your dog image: /regular_paw_storage/e527eea918905517/powered_dog.png
  ```
- **Local Download:**
  ```sh
  scp -O e.tytar-dog@osctf.nnk.moe:/regular_paw_storage/e527eea918905517/powered_dog.png "<target_folder>"
  ```
- **Flag Status:** Accepted.

---

### 01TailheadedDog
- **Artifact:** `/home/ctf/go_to_opt/Downloads/dog.log` (1.45 MB)
- **Method:** So we have a huge log file filled with junk text, but the name `TailheadedDog` hints at `head` and `tail`. The head gives the first part of the storage directory `/regular_paw_storage/f6e3719d`, and the tail gives the rest `aa82dac9/tailheaded.png`. So we combine them together to get the full image path.
- **Commands:**
  ```sh
  head -n 10 /home/ctf/go_to_opt/Downloads/dog.log
  tail -n 30 /home/ctf/go_to_opt/Downloads/dog.log
  ```
- **Output:**
  - Head prefix: `/regular_paw_storage/f6e3719d`
  - Tail suffix: `aa82dac9/tailheaded.png`
  - Result: `/regular_paw_storage/f6e3719daa82dac9/tailheaded.png`
- **Local Download:**
  ```sh
  scp -O e.tytar-dog@osctf.nnk.moe:/regular_paw_storage/f6e3719daa82dac9/tailheaded.png "<target_folder>"
  ```
- **Flag Status:** Accepted.

---

### 02MuteDog
- **Artifact:** `/home/ctf/go_to_opt/Programs/mute_dog`
- **Method:** So when we run `./mute_dog`, it looks like it prints nothing to the screen. But what is actually happening is that it splits the flag path across both standard output (fd 1) and standard error (fd 2). So we redirect stdout to `/tmp/out` and stderr to `/tmp/err`. When we cat both files together, `stdout` gives the first half of the path and `stderr` gives the second half.
- **Commands:**
  ```sh
  /home/ctf/go_to_opt/Programs/mute_dog > /tmp/out 2> /tmp/err
  cat /tmp/out && cat /tmp/err
  ```
- **Output:**
  - `stdout`: `/regular_paw_storage/59e2b053`
  - `stderr`: `08652094/Mute_dog.png`
  - Combined path: `/regular_paw_storage/59e2b05308652094/Mute_dog.png`
- **Local Download:**
  ```sh
  scp -O e.tytar-dog@osctf.nnk.moe:/regular_paw_storage/59e2b05308652094/Mute_dog.png "<target_folder>"
  ```
- **Flag Status:** Accepted.

---

### 02PetDog
- **Artifact:** `/home/ctf/go_to_opt/Doguments/auto_petter`
- **Method:** So the assembly script was failing because the mascot frames got corrupted by recent commits. When we check `git log --oneline`, we see the initial commit `e60deac` still has the original intact mascot frames. So we just run `git checkout e60deac -- frames/` to restore  from that commit, and then the assembly script runs and builds our picture.
- **Commands:**
  ```sh
  cd /home/ctf/go_to_opt/Doguments/auto_petter

  git log --oneline -n 6

  git checkout e60deac -- frames/

  ./tools/assemble_mascot.sh
  file mascot.png
  ```
- **Output:**
  ```text
  mascot.png assembled OK (114456 bytes)
  mascot.png: PNG image data, 300 x 200, 8-bit/color RGBA, non-interlaced
  ```
- **Local Download:**
  ```sh
  scp -O e.tytar-dog@osctf.nnk.moe:/home/ctf/go_to_opt/Doguments/auto_petter/mascot.png "<target_folder>"
  ```
- **Flag Status:** Accepted.

---

### 02RemDog
- **Artifact:** `/home/ctf/go_to_opt/Doguments/unsorted/`
- **Method:** So this folder had calm dogs and angry dogs, and we needed to remove only the angry ones. Looking at the files with `ls -l`, some dogs are 1,678 bytes and some dogs are 1,937 bytes. So firstly, we preview the 1,937 files using `find . -size 1937c | wc -l` to see how many there are. Once we confirm it, we run `find . -size 1937c -delete`, and then the checker verifies that only the calm ones are left.
- **Commands:**
  ```sh

  find /home/ctf/go_to_opt/Doguments/unsorted -type f -size 1937c | head -n 5
  find /home/ctf/go_to_opt/Doguments/unsorted -type f -size 1937c | wc -l

  find /home/ctf/go_to_opt/Doguments/unsorted -type f -size 1937c -delete
  ```
- **Output:**
  ```text
  Woof! You have solved Rem_dog!
  Here's your dog pic: /regular_paw_storage/32ec8ff6601182eb/Rem_dog.png
  ```
- **Local Download:**
  ```sh
  scp -O e.tytar-dog@osctf.nnk.moe:/regular_paw_storage/32ec8ff6601182eb/Rem_dog.png "<target_folder>"
  ```
- **Flag Status:** Accepted.

---

### 02ScatteredDog
- **Artifact:** Directory `/home/ctf/go_to_opt/Videos/Scattered dog` containing 100 slices named from `-50` through `49`.
- **Method:** So in `Videos/Scattered dog`, we find 100 small binary files with names ranging from `-50` up to `49`. Basically, a PNG was chopped into pieces using negative and positive integer indices. So to put it back together, we just write a simple bash loop from `-50` to `49`, appending each chunk into `scattered_dog.png`. 
- **Commands:**
  ```sh
  for i in $(seq -50 49); do cat "/home/ctf/go_to_opt/Videos/Scattered dog/$i"; done > /home/ctf/scattered_dog.png
  file /home/ctf/scattered_dog.png
  ```
- **Output:**
  ```text
  /home/ctf/scattered_dog.png: PNG image data, 300 x 195, 8-bit/color RGBA, non-interlaced
  ```
- **Local Download:**
  ```sh
  scp -O e.tytar-dog@osctf.nnk.moe:/home/ctf/scattered_dog.png "<target_folder>"
  ```
- **Flag Status:** Accepted.

---

### 02ShyDog
- **Artifact:** `/home/ctf/go_to_opt/Videos/Shy dog` containing hundreds of randomly named decoy files.
- **Method:** So inside the `Videos/Shy dog` folder, there are a bunch of decoy files that look like pictures. Instead of checking them one by one, we just run `file *` and grep for `PNG image data`. It immediately filters out all the garbage text decoys and points us to the only genuine PNG, which is `PIC_Maybe_Dog_e4394ccc.png`.
- **Commands:**
  ```sh
  file "/home/ctf/go_to_opt/Videos/Shy dog"/* | grep "PNG image data"
  ```
- **Output:**
  ```text
  /home/ctf/go_to_opt/Videos/Shy dog/PIC_Maybe_Dog_e4394ccc.png: PNG image data, 150 x 200, 8-bit colormap, non-interlaced
  ```
- **Local Download:**
  ```sh
  scp -O 'e.tytar-dog@osctf.nnk.moe:/home/ctf/go_to_opt/Videos/Shy\ dog/PIC_Maybe_Dog_e4394ccc.png' "<target_folder>"
  ```
- **Flag Status:** Accepted.

---

### 02SpitoonDog
- **Artifact:** `/home/ctf/go_to_opt/Programs/spitoon`
- **Method:** So here we run `./spitoon`. Reading `man spitoon` reveals the required dialogue lines and notes that the program handles `SIGINT`.
- **Interactive Steps:**
  1. Prompt 1: Type `How tough am I? How tough am I?! I ran 142 of linux programs this lab`
  2. Prompt 2: Type `Without 142 errors` and press `Ctrl+C`.
- **Output:**
  ```text
  Right this way sir: /regular_paw_storage/8d38f7730689b8eb/Spitoon_dog.png, sorry to keep you waiting
  ```
- **Local Download:**
  ```sh
  scp -O e.tytar-dog@osctf.nnk.moe:/regular_paw_storage/8d38f7730689b8eb/Spitoon_dog.png "<target_folder>"
  ```
- **Flag Status:** Accepted.

---

### 03AqueousDog
- **Artifact:** `/home/ctf/.config/fish/config.fish`
- **Method:** So the container has the `fish` shell installed alongside `bash`. When we check the fish configuration file at `/home/ctf/.config/fish/config.fish`, we find a base64 encoded string. So we just echo that string into `base64 -d`, and it decodes to the image path `/regular_paw_storage/972181104cee992b/Aqueous_dog.png`.
- **Commands:**
  ```sh
  echo 'L3JlZ3VsYXJfcGF3X3N0b3JhZ2UvOTcyMTgxMTA0Y2VlOTkyYi9BcXVlb3VzX2RvZy5wbmc=' | base64 -d
  ```
- **Output:**
  ```text
  /regular_paw_storage/972181104cee992b/Aqueous_dog.png
  ```
- **Local Download:**
  ```sh
  scp -O e.tytar-dog@osctf.nnk.moe:/regular_paw_storage/972181104cee992b/Aqueous_dog.png "<target_folder>"
  ```
- **Flag Status:** Accepted.

---

### 03ChainedDog
- **Artifact:** Directory `/home/dog/Downloads` containing `start`, hundreds of `file_*.txt`, and `end`.
- **Method:** So in `/home/dog/Downloads`, all the files are linked like a linked list. Each file has two lines: Line 1 has `Next: <filename>`, and Line 2 has `Content: ` followed by raw binary bytes. To get the image, we start from `start`, extract the content, read the next filename, and keep going until `end`. To extract the bytes cleanly without breaking binary data, we use `tail -n +2` to drop the first line, and then `tail -c +10` to strip the 9-byte `Content: ` prefix and append the rest to our PNG.
- **Commands:**
  ```sh
  rm -f /home/ctf/chained_dog.png && curr="start"
  while [ -n "$curr" ]; do
    tail -n +2 "/home/dog/Downloads/$curr" | tail -c +10 >> /home/ctf/chained_dog.png
    [ "$curr" = "end" ] && break
    curr=$(head -n 1 "/home/dog/Downloads/$curr" | awk '{print $2}')
  done
  file /home/ctf/chained_dog.png
  ```
- **Output:**
  ```text
  /home/ctf/chained_dog.png: PNG image data, 225 x 300, 8-bit/color RGB, non-interlaced
  ```
- **Local Download:**
  ```sh
  scp -O e.tytar-dog@osctf.nnk.moe:/home/ctf/chained_dog.png "<target_folder>"
  ```
- **Flag Status:** Accepted.

---

### 03InsecureDog
- **Artifact:** `/home/ctf/go_to_opt/Doguments/secret_stuff/remember.txt` and `super_secret/`
- **Method:** So in `Doguments/secret_stuff/super_secret`, the directory permissions are `d-wx--x--x`. That means we have execute permission to access files if we know their names, but no read permission, so `ls` fails. In `remember.txt`, the note says the filename is 4 letters made of D, O, and G, ending in `.png`. We just run a quick bash loop over `{D,O,G}` trying to test each combination. It hits the real file `ODGG.png`, and we grab it from there.
- **Commands:**
  ```sh
  for a in D O G; do for b in D O G; do for c in D O G; do for d in D O G; do
    f="/home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/${a}${b}${c}${d}.png"
    [ -f "$f" ] && echo "FOUND: $f"
  done; done; done; done
  ```
- **Output:**
  ```text
  FOUND: /home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/ODGG.png
  ```
- **Local Download:**
  ```sh
  scp -O e.tytar-dog@osctf.nnk.moe:/home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/ODGG.png "<target_folder>"
  ```
- **Flag Status:** Accepted.

---

### 03NetDog
- **Artifact:** Root process `/usr/local/bin/netdog` listening on TCP port 3284 (`0.0.0.0:3284`).
- **Method:** So for this one, we check what network ports are listening locally using `ss -tlpn` or `netstat`. We see there is a local service listening on port 3284. So we just connect to it using `nc 127.0.0.1 3284` and redirect the output into `net_dog.png`. The service sends the raw PNG stream over TCP and we get our picture.
- **Commands:**
  ```sh
  nc -w 2 127.0.0.1 3284 > /home/ctf/netdog.png
  file /home/ctf/netdog.png
  ```
- **Output:**
  ```text
  /home/ctf/netdog.png: PNG image data, 300 x 222, 8-bit/color RGBA, non-interlaced
  ```
- **Local Download:**
  ```sh
  scp -O e.tytar-dog@osctf.nnk.moe:/home/ctf/netdog.png "<target_folder>"
  ```
- **Flag Status:** Accepted.

---

### 03UsedDog
- **Artifact:** `/home/dog/Used_dog.png`
- **Method:** So here we want to find what files were recently modified on the system this year without guessing exact dates. We run `find / -newermt "2026-01-01" 2>/dev/null` to filter for files touched in 2026, and we notice there is another user on the system named `dog` with home directory `/home/dog`. When we check their folder, we find `Used_dog.png` sitting right there at `/home/dog/Used_dog.png`.
- **Commands:**
  ```sh
  find / -newermt "2026-01-01" 2>/dev/null | grep -E "^/home/"

  ls -la /home/dog/Used_dog.png
  file /home/dog/Used_dog.png
  ```
- **Output:**
  ```text
  -rw-r--r-- 1 dog dog 123852 Nov 14 14:30 /home/dog/Used_dog.png
  /home/dog/Used_dog.png: PNG image data, 300 x 200, 8-bit/color RGBA, non-interlaced
  ```
- **Local Download:**
  ```sh
  scp -O e.tytar-dog@osctf.nnk.moe:/home/dog/Used_dog.png "<target_folder>"
  ```
- **Flag Status:** Accepted.

---

## C CTF

### cavern
- **Artifact:** `cavern` (binary), `cavern.c` (source code)
- **Method:** So here the `cavern` binary loads an external shared object and runs our `run()` function. In `cavern.c`, when we call `guess()`, it allocates 128 bytes on the heap for a struct with two 64-byte fields: `guess` and `secret`. It decrypts the flag into `secret`, but then it calls `free()` on the struct and still returns the dangling pointer. In this challenge environment, the freed chunk enters glibc's per-thread cache (`tcache`), where allocator metadata overwrites the beginning of the chunk. The flag at offset 64 remains readable, so our shared library immediately prints `ptr + 64`. 
- **Commands:**
  ```sh
  nano offer.c
  ```
  Inside `offer.c`:
  ```c
  #include <stdio.h>

  extern void* guess(char*);

  void run(void) {
      char *ptr = (char *)guess("heist");
      if (ptr) {
          printf("\n[+] Flag: %s\n\n", ptr + 64);
      }
  }
  ```
  Compile and execute:
  ```sh
  gcc -shared -fPIC -o offer.so offer.c
  ./cavern ./offer.so
  ```
- **Output:**
  ```text
  "Wrong," the demons cackle. "Again!"

  [+] Flag: cctf{REm3m8eR_tHeR3_Is_nO_5p0oN_fREe_fr0M_nasa1_d3M0ns}
  ```
- **Flag:** `cctf{REm3m8eR_tHeR3_Is_nO_5p0oN_fREe_fr0M_nasa1_d3M0ns}`
- **Flag Status:** Accepted.

---

### legacy
- **Artifact:** `legacy.c` (source code), `task.txt` (task description)
- **Method:** So the task says this code was written for an old 18-bit machine where `unsigned` has 18 bits. But on our modern machine, `unsigned` is 32 bits. Because of that, `position()` calculates a huge number that goes way out of bounds of the 69-byte array and segfaults. On the original hardware, when an 18-bit register overflows, the higher bits just drop off. So in our code, we just take the hash calculation and do `% (1 << 18)`, which is modulo 262144. That simulates the 18-bit register overflow, our diagnostics check value matches 237614, and the document decrypts cleanly.
- **Commands:**
  Edit `legacy.c` (via `nano legacy.c`):

  Line 9 (in `position()`):
  Old code:
  ```c
  h = h * 67 + *s++;
  ```
  New code:
  ```c
  h = (h * 67 + *s++) % (1 << 18);
  ```
  Compile and run:
  ```sh
  gcc -o legacy legacy.c && ./legacy
  ```
- **Output:**
  ```text
  == Diagnostics: ==
   unsigned has 32 bits
   check value: 237614
  == End of Diagnostics ==

  Attempting to decrypt the document...
  == Decrypted Document: ==
  cctf{The_gOod_O1d_D4y5_With_90oD_OLd_C}
  == End of Decrypted Document ==
  ```
- **Flag:** `cctf{The_gOod_O1d_D4y5_With_90oD_OLd_C}`
- **Flag Status:** Accepted.

---

### vaslang
- **Artifact:** `vaslang.c` (source code), `inp.vas` (program code), `task.txt` (task description)
- **Method:** So `vaslang.c` is a stack-based interpreter, but it fails on `inp.vas` because of three things. Firstly, functions in `inp.vas` call other functions before they are even defined, so we can't look them up immediately during parsing. Instead, we save the function name string and add a `resolve_all()` step right before `main!` runs to link them. Secondly, the code has useless operations that try to pop from an empty stack, so we add bounds checks like `vsp > vs` so it doesn't underflow. And thirdly, the call and value stacks were only 512 entries, so we increase them to 65536 so it doesn't overflow the stack depth.
- **Commands:**
  Executed locally in the task folder:
  ```sh
  nano vaslang.c
  ```
  Apply the following modifications to `vaslang.c`:

  1. **Add `target_name` to `struct term` (Lines 9–16)**  
     Stores the function name string so calls can be resolved after all definitions are parsed.
     
     Old code:
     ```c
     struct term {
         char ty;
         union {
             struct term *target;
             unsigned char val;
         };
     };
     ```
     New code:
     ```c
     struct term {
         char ty;
         char *target_name;
         union {
             struct term *target;
             unsigned char val;
         };
     };
     ```

  2. **Expand Stack Limits (Lines 26–30)**  
     Increase call stack `fs` and value stack `vs` from 512 to 65536 to prevent stack overflow.
     
     Old code:
     ```c
     struct term *fs[512] = {0};
     unsigned char vs[512] = {0};
     struct term **fsp = fs;
     unsigned char *vsp = vs;
     ```
     New code:
     ```c
     struct term *fs[65536] = {0};
     unsigned char vs[65536] = {0};
     struct term **fsp = fs;
     unsigned char *vsp = vs;
     ```

  3. **Add `resolve_all()` Helper (Line 32, inserted before `run()`)**  
     Iterates through all parsed call terms (`'C'`) and links their targets to the defined functions.
     
     Old code:
     ```c
     /* none (new function) */
     ```
     New code:
     ```c
     void resolve_all(void)
     {
         static int done = 0;
         if (done) return;
         done = 1;

         for (struct term *t = terms; t < termp; t++) {
             if (t->ty == 'C') {
                 char *name = t->target_name;
                 int found = 0;
                 for (int i = 0; funcs + i < funcp; i++) {
                     if (strcmp(funcs[i].name, name) == 0) {
                         t->target = funcs[i].body;
                         found = 1;
                         break;
                     }
                 }
                 if (!found) errorf("Function %s! not found", name);
             }
         }
     }
     ```

  4. **Stack Underflow Guards in `run()` (Lines 39–56)**  
     Prevent popping from an empty stack on `+`, `*`, `:`, and `.`.
     
     Old code:
     ```c
             case '+':
                 l = *--vsp;
                 r = *--vsp;
                 *vsp++ = (l + r) % 251;
                 break;
             case '*':
                 l = *--vsp;
                 r = *--vsp;
                 *vsp++ = (l * r) % 251;
                 break;
             case ':':
                 l = *--vsp;
                 *vsp++ = l;
                 *vsp++ = l;
                 break;
             case '.':
                 vsp--;
                 break;
     ```
     New code:
     ```c
             case '+':
                 l = (vsp > vs) ? *--vsp : 0;
                 r = (vsp > vs) ? *--vsp : 0;
                 *vsp++ = (l + r) % 251;
                 break;
             case '*':
                 l = (vsp > vs) ? *--vsp : 0;
                 r = (vsp > vs) ? *--vsp : 0;
                 *vsp++ = (l * r) % 251;
                 break;
             case ':':
                 if (vsp > vs) {
                     l = *(vsp - 1);
                     *vsp++ = l;
                 }
                 break;
             case '.':
                 if (vsp > vs)
                     vsp--;
                 break;
     ```

  5. **Invoke `resolve_all()` on Command Execution in `xcmd()` (Line 79)**  
     Resolves all forward function references right before `main!` executes.
     
     Old code:
     ```c
     void xcmd(char* s)
     {
         char *name = strsep(&s, "!");
     ```
     New code:
     ```c
     void xcmd(char* s)
     {
         resolve_all();
         char *name = strsep(&s, "!");
     ```

  6. **Defer Function Lookup in `xdef()` (Lines 119–125)**  
     Instead of looking up the function immediately during parse, save its name with `strdup`.
     
     Old code:
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
     New code:
     ```c
             } else {
                 ftermp->ty = 'C';
                 ftermp->target_name = strdup(term);
             }
     ```

  7. **Stream stdin in `main()` without Interactive Prompt (Lines 153–165)**  
     Remove the hardcoded test lines and prompt so input is cleanly piped from `inp.vas`.
     
     Old code:
     ```c
     int main()
     {
         char buf[640 * 1024];
         xrom("hello: 104 : 248 + : 180 * : 111 0");
         xrom("hello!");
         while (1) {
             printf("> ");
             if (fgets(buf, sizeof(buf), stdin) == NULL)
                 return 0;
             *strchrnul(buf, '\n') = 0;
             xline(buf);
         }
     }
     ```
     New code:
     ```c
     int main()
     {
         char buf[640 * 1024];
         while (fgets(buf, sizeof(buf), stdin) != NULL) {
             buf[strcspn(buf, "\r\n")] = 0;
             if (buf[0])
                 xline(buf);
         }
         return 0;
     }
     ```

  Compile and run with `inp.vas`:
  ```sh
  gcc -o vaslang vaslang.c && ./vaslang < inp.vas
  ```
- **Output:**
  ```text
  cctf{y0U_coulD_haV3_solveD_ThIS_The_FUN_w4y}
  ```
- **Flag:** `cctf{y0U_coulD_haV3_solveD_ThIS_The_FUN_w4y}`
- **Flag Status:** Accepted.

---

### xorcism
- **Artifact:** `demon.c` (source code), `task.txt` (task description)
- **Method:** So looking at `demon.c`, it asks for a 33-character password and verifies each byte against `enc[i] ^ key[i % 4]`. Because XOR is self-inverting, if $A \oplus B = C$, then $C \oplus B = A$. So we don't need to guess anything. We just delete the input checking loop, and inside `main()` we compute `in[i] = enc[i] ^ key[i % 4]` directly for all 33 bytes and print it out to get our flag.
- **Commands:**
  Edit `demon.c`:

  Lines 16–33 (in `main()`):
  Old code:
  ```c
      fputs("crack me!\n", stdout);
      fputs("password: ", stdout);
      if (!fgets(in, sizeof in, stdin))
          return 1;
      in[strcspn(in, "\n")] = '\0';

      if (strlen(in) != PWLEN) {
          puts("nope");
          return 1;
      }

      for (int i = 0; i < PWLEN; i++) {
          if ((unsigned char)in[i] != (enc[i] ^ key[i % 4])) {
              puts("nope");
              return 1;
          }
      }
  ```
  New code:
  ```c
      for (int i = 0; i < PWLEN; i++)
          in[i] = enc[i] ^ key[i % 4];
  ```

  Compile and run:
  ```sh
  gcc -o demon demon.c && ./demon
  ```
- **Output:**
  ```text
  correct! flag: cctf{pL3Ase_doN7_F3ed_tHe_NA54l_DeMoNs}
  ```
- **Flag:** `cctf{pL3Ase_doN7_F3ed_tHe_NA54l_DeMoNs}`
- **Flag Status:** Accepted.

---

### zhaitie
- **Artifact:** `test.zai` (compressed file), `libzhaitie-aarch64-macos.a` / `libzhaitie-x86_64-linux.a` (static library), `zhaitie-AGENTS.md` (API specification), `task.txt` (task description)
- **Method:** So we are given a compressed file `test.zai` and a precompiled static library `libzhaitie.a`. According to the documentation, we first call `zhaitie_init()`, then create a context with `razzhaitie_new()`. We read the initial 24 bytes from the file as a header, pass it to `razzhaitie()`, and it writes decompressed chunks through our callback function while returning how many bytes it needs for the next window. So we just loop, reading whatever chunk size it asks for and passing it back, streaming decompressed chunks to stdout into our presentation file containing the flag.
- **Commands:**
  Executed locally in the task folder:
  Create the decompression client `decompress.c`:
  ```c
  #include <stdio.h>

  void zhaitie_init(void);
  void* razzhaitie_new(void);
  int razzhaitie(void* ctx, const void* in, int len, void (*cb)(const void*, int));

  void on_chunk(const void* chunk, int len) {
      fwrite(chunk, 1, len, stdout);
  }

  int main(void) {
      FILE *fp = fopen("test.zai", "rb");
      if (!fp) return 1;

      zhaitie_init();
      void *ctx = razzhaitie_new();

      char buf[1048576];
      int n = fread(buf, 1, 24, fp);

      while (n > 0) {
          int next = razzhaitie(ctx, buf, n, on_chunk);
          if (next <= 0) break;
          n = fread(buf, 1, next, fp);
      }

      fclose(fp);
      return 0;
  }
  ```
  Compile and run:
  ```sh
  gcc -o decompress decompress.c libzhaitie-aarch64-macos.a
  ./decompress > presentation.out
  tail -n 2 presentation.out
  ```
- **Output:**
  ```text
  cctf{NAtura1_iNtelligenCe_oVer_a1_d0Cs}
  ```
- **Flag:** `cctf{NAtura1_iNtelligenCe_oVer_a1_d0Cs}`
- **Flag Status:** Accepted.

---

## Threads CTF

### needle
- **Artifact:** `/home/ctf/meeting` (binary), `letter` (task description)
- **Method:** So basically, the `meeting` binary is setuid and it loads our `./libmeeting.so` using `dlopen`, then runs `meeting()` on a worker thread. Inside this binary, there is a target thread called `retrieval` that computes the flag. Because the memory layout is fixed, we can just run `nm meeting | grep retrieval` to find the global `pthread_t` handle at `0x404128` in `.bss`. Then in our shared library, we just call `pthread_join` on that exact address to wait until `retrieval` finishes and returns the pointer to our flag.
- **Commands:**
  Discovery of the target thread variable in symbol table:
  ```sh
  nm meeting | grep retrieval
  ```

  Create `libmeeting.c`
  ```c
  #include <pthread.h>
  #include <stdio.h>

  void *meeting(void *arg) {
      void *flag = NULL;
      if (pthread_join(*(pthread_t *)0x404128, &flag) == 0 && flag) {
          printf("\nFLAG: %s\n", (char *)flag);
      }
      return NULL;
  }
  ```
  Compile and run:
  ```sh
  gcc -shared -fPIC -o libmeeting.so libmeeting.c
  ./meeting
  ```
- **Output:**
  ```text
  FLAG: threadctf{sHoOT1Ng_SoMe_atoMIC_fe3T}

  The fabric is nowhere to be found! Polythread is very angry, so you'll have to stay out of faywild for a while.
  Thankfully, your name is still with you, so you are free to lose it another day.
  ```
- **Flag:** `threadctf{sHoOT1Ng_SoMe_atoMIC_fe3T}`
- **Flag Status:** Accepted.

---

### fortyforks
- **Artifact:** `/home/ctf/secrets` (named pipe), server environment with 5-second process lifetime limit
- **Method:** So here the server is streaming the flag into a named pipe `/home/ctf/secrets`, but the catch is that any process gets killed after 5 seconds. In Unix, when a process calls `fork()`, the child gets a copy of the parent's file descriptors that point to the exact same open file description in the kernel, so both processes share the same pipe buffer. So we just set up an alarm with `SIGALRM` every 2 seconds. When it fires, the parent forks a child and exits with `_exit(0)`, and the child inherits the open pipe and keeps reading. This way, we reset the 5-second lifetime limit on each fork without ever closing the pipe, and at the end we get the flag.
- **Commands:**
  Executed locally in the task folder:

  Create `reader.c` 
  ```c
  #include <errno.h>
  #include <fcntl.h>
  #include <signal.h>
  #include <unistd.h>

  static void renew(int sig) {
      if (fork() > 0) _exit(0);
      alarm(2);
  }

  int main(void) {
      signal(SIGALRM, renew);
      alarm(2);

      int fd = open("/home/ctf/secrets", O_RDONLY);
      if (fd < 0) return 1;

      char buf[4096];
      while (1) {
          ssize_t n = read(fd, buf, sizeof(buf));
          if (n < 0) {
              if (errno == EINTR) continue;
              return 1;
          }
          if (n == 0) break;
          write(1, buf, n);
      }
      return 0;
  }
  ```
  Compile and start the background reader:
  ```sh
  gcc -o reader reader.c
  nohup ./reader </dev/null >/home/ctf/reader.log 2>&1 &
  ```
  Inspect output log:
  ```sh
  cat /home/ctf/reader.log
  ```
- **Output:**
  ```text
  Congratulations, Agent. The flag is: threadctf{oNLy_pr0per_1IneaG3_g37_To_livE_1N_cAst1E}
  ```
- **Flag:** `threadctf{oNLy_pr0per_1IneaG3_g37_To_livE_1N_cAst1E}`
- **Flag Status:** Accepted.

---

### princess
- **Artifact:** Forest environment with background `cabin` and `princess` processes
- **Method:** So in the forest there are many cabin processes, and 50 of them are cursed. When we look at `ps -ef`, we can see that these cursed cabins create child processes that end up as defunct zombies, because the parent cabins never call `wait()` or `waitpid()` to clean them up. So our loop just greps for defunct processes, grabs their parent PIDs, checks that the command name is actually `cabin`, and sends `kill -9`. In this challenge environment, only the cursed cabins leak defunct processes like that, so once we kill all 50 cursed cabins, the server drops our reward in `/home/ctf/flag.txt`.
- **Commands:**
  Executed locally in the task folder:

  Discovery and preview of cursed cabins:
  ```sh
  ps -ef | grep -E 'cabin|defunct' | head -n 20

  ps -ef | grep '[d]efunct' | awk '$3 != 1 {print $3}' | while read p; do ps -p "$p" -o pid,comm= | grep cabin; done | head -n 10
  ```

  Safely terminate 50 verified cursed cabins:
  ```sh
  killed=0
  while [ "$killed" -lt 50 ]; do
      for pid in $(ps -ef | grep '[d]efunct' | awk '$3 != 1 {print $3}'); do
          if ps -p "$pid" -o comm= 2>/dev/null | grep -q 'cabin'; then
              if kill -9 "$pid" 2>/dev/null; then
                  killed=$((killed + 1))
                  echo "Stopped: $killed/50"
                  if [ "$killed" -ge 50 ]; then
                      break 2
                  fi
              fi
          fi
      done
      sleep 0.2
  done
  ```

  Read the generated flag:
  ```sh
  cat /home/ctf/flag.txt
  ```
- **Output:**
  ```text
  === The Forest ===
  The forest falls silent. 50 cursed cabins lie in ruin, and
  the story is finally over. Your reward waits in your home
  directory.

  threadctf{wHA7_do_yoU_meaN_7hEY_are_4lRE4dy_de4D}
  ```
- **Flag:** `threadctf{wHA7_do_yoU_meaN_7hEY_are_4lRE4dy_de4D}`
- **Flag Status:** Accepted.
