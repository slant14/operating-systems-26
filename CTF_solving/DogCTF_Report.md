# DogCTF — Report

**Result:** 11 of 22 flags (50%).

The task was to locate dog-related PNG files in a Linux environment accessed through SSH. The images themselves were the flags, not text strings hidden inside them. The numbering below follows my actual discovery order.

**Source note:** My personal notes (`dogctf.rtf`) and earlier terminal output confirm the specific details marked as such. Where I did not record the exact commands, the procedure is reconstructed from the reference walkthrough (`dogctf.md`). These are reproducible methods, not a verbatim transcript of my session.

## 1. Results

| No. | Flag | Main concept |
|---:|---|---|
| 1 | `01HelloDog` | File listing |
| 2 | `01PoweredDog` | Navigation and reading text |
| 3 | `01EditDog` | Text editing |
| 4 | `01BackgroundDog` | Shell job control |
| 5 | `02ShyDog` | File signatures and filtering |
| 6 | `02MuteDog` | Permissions and printable strings |
| 7 | `02SpitoonDog` | Printable strings in a binary |
| 8 | `01CompileDog` | C compilation and execution |
| 9 | `03RestoreDog` | Vim swap recovery |
| 10 | `01CatDog` | Binary concatenation |
| 11 | `01TailheadedDog` | Reading the beginning and end of a log |

The `01`, `02`, and `03` prefixes are part of the flag identifiers; they do not represent my discovery order.

## 2. Flag-by-flag solutions

The starting challenge directory was `/home/ctf/go_to_opt`. Commands below assume Bash inside the assigned DogCTF container, unless explicitly marked as local.

### 1. 01HelloDog

**Location:** `/home/ctf/go_to_opt/hoi_:3.png`.

I started by listing the files in the current directory. The PNG was visible directly in the output.

```bash
ls -la
```

The terminal listing showed `hoi_:3.png`, with a size of 60446 bytes. No reconstruction or decoding was required.

To confirm its actual type, I could use:

```bash
file 'hoi_:3.png'
```

**Explanation:** `ls -l` prints detailed file information; `-a` includes hidden entries. `file` identifies the content type rather than relying on the extension. Quoting a filename prevents the shell from interpreting special characters or spaces.

### 2. 01PoweredDog

**Location:** `/opt/README.md`.

Reconstructed procedure: inspect `/opt` and read the README to obtain the image path.

```bash
cd /opt
ls -la
cat README.md
```

The reference records the following path:

```text
/regular_paw_storage/c0f7dd844643ddc3/powered_dog.png
```

**Explanation:** `cd` changes the working directory; `cat` writes file contents to standard output. An absolute path begins with `/` and does not depend on the current directory. The README is a clue, rather than the PNG flag itself.

### 3. 01EditDog

**Location:** `Doguments/EDITME`.

I found a text file containing a dog drawing. I needed to make the dog **nine characters longer**. 

```bash
cd ~/go_to_opt/Doguments
cat EDITME
vim EDITME
```

In Vim, `i` enters insert mode, `Esc` returns to normal mode, and `:wq` saves and exits. After making the required edit, the challenge provided the PNG path. The reference names the resulting file `Edit_dog.png`.

**Explanation:** Vim is a modal text editor. Editing the file was required by the challenge, unlike merely reading its contents. I did not record the exact drawing or resulting absolute path in my personal notes.

### 4. 01BackgroundDog

**Initial clue:** a stopped job displayed immediately after login.

```text
[1]+  Stopped  /usr/local/bin/background_dog /tmp/bgdog
```

I ran:

```bash
jobs
fg %1
```

`jobs` showed the same stopped process. `fg %1` resumed job number 1 in the foreground, after which I obtained the flag information.

**Explanation:** A stopped process has not exited; its execution is suspended. `jobs` lists jobs known to the current shell. `%1` refers to a shell job number, not an operating-system PID. `fg` resumes a stopped job and makes it the foreground job. `bg` would resume a job in the background instead.

### 5. 02ShyDog

**Location:** `Videos/Shy Dog/`.

I used tab completion after typing `cd Sh` while in the `Videos` directory. Bash completed the directory name, showing how to handle the space.

```bash
cd ~/go_to_opt/Videos
cd Shy\ Dog
file * | awk '/PNG/{print $1}'
```

The matching file was:

```text
Photo_Maybe_Dog_a4ccecc8.png
```

**Explanation:** `\ ` escapes a literal space; `cd "Shy Dog"` is equivalent. `*` expands to the non-hidden matching filenames. `file` examines file contents. The pipe passes its output to `awk`, which selects lines containing `PNG` and prints their first whitespace-separated field. The first field may include the trailing colon printed by `file`.

### 6. 02MuteDog

**Location:** `Programs/mute_dog`.

Instead of the input/output redirection technique in the reference walkthrough, I used a simpler approach. I added read permission and examined the printable strings in the binary manually.

```bash
cd ~/go_to_opt/Programs
chmod +r mute_dog
strings mute_dog
```

I noticed two nearby pieces of output and combined them into the path:

```text
/regular_paw_storage/afee22497ad0aa07/Mute_dog.png
```

**Explanation:** `chmod +r` adds read permission (the affected classes depend on the umask when no class is specified). `strings` extracts sequences of printable characters from a file, including an executable. It does not execute or decompile the program. The path pieces were found by reading the output manually.

### 7. 02SpitoonDog

**Location:** `Programs/spitoon`.

I inspected the printable strings in the file and manually found the flag path, without using `awk` to filter the output. The reference also includes a read-permission step if needed.

```bash
cd ~/go_to_opt/Programs
chmod +r spitoon
strings spitoon
```

The result pointed to `Spitoon_dog.png` under `/regular_paw_storage/`.

**Explanation:** Linux distinguishes read (`r`), write (`w`) and execute (`x`) permissions. Reading an executable with `strings` is different from executing it with `./spitoon`. This method worked because the clue was stored as printable text in the file.

### 8. 01CompileDog

**Location:** `Programs/compile_dog.c`.

Reconstructed procedure: compile the supplied C program and run the resulting executable to obtain the PNG path.

```bash
cd ~/go_to_opt/Programs
ls -la
gcc compile_dog.c -o test.out
./test.out
```

The resulting flag is named `Compile_dog.png` in the reference.

**Explanation:** GCC translates C source code into a native executable; `-o` specifies its output filename. `./test.out` explicitly executes a program in the current directory. The current directory is not usually in the command search path (`PATH`).

### 9. 03RestoreDog

**Location:** `Doguments/.crashed.txt.swp`.

Reconstructed procedure: inspect the crashed text file and reveal the hidden Vim swap file.

```bash
cd ~/go_to_opt/Doguments
cat crashed.txt
ls -la
vim -r .crashed.txt.swp
```

The recovered editing buffer contained the path to `Restore_dog.png`. Depending on the installed Vim version, `vim -r crashed.txt` can also recover from the swap associated with the original filename.

**Explanation:** A swap file is a temporary recovery file used by Vim to preserve editing data. Its name starts with a dot, so normal `ls` omits it. `vim -r` invokes recovery mode. Recovery can expose unsaved changes that are not present in the original text file.

### 10. 01CatDog

**Location:** `Pictures/`.

Reconstructed procedure: the image is stored in two binary fragments, which must be joined in the correct order.

```bash
cd ~/go_to_opt/Pictures
ls
cat Cat_dog_top Cat_dog_bottom > Cat_dog.png
file Cat_dog.png
```

**Result:** `Cat_dog.png`.

**Explanation:** `cat` copies the bytes of its inputs sequentially; it is not limited to text. `>` redirects standard output to a file, creating it or truncating an existing file. The order matters because PNG is a structured binary format. `>>` would append instead of replacing the output, and running it repeatedly could corrupt the result.

### 11. 01TailheadedDog

**Location:** `Downloads/dog.log`.

Reconstructed procedure: examine the first five and last fifteen lines of the log and combine the two pieces of the path.

```bash
cd ~/go_to_opt/Downloads
head -n 5 dog.log
tail -n 15 dog.log
```

The resulting image was `tailheaded.png` according to the reference. The exact combined path was not recorded in my notes.

**Explanation:** `head` reads the beginning of a file; `tail` reads the end. `-n` specifies the number of lines. This avoids manually searching an entire long log when the clue is split between its beginning and end.

## 3. Retrieving the PNGs from the VM

The flags had to be copied to my local machine as images. I initially attempted SCP while still inside the VM, so `~/Desktop/` referred to the remote user's home directory, not my Mac. The custom SSH gateway also produced extra login output.

The standard SCP method, when the server supports it, is to run the following from a separate **local** terminal, not from the remote shell:

```bash
mkdir -p ~/Desktop/DogCTF
scp 'USER@HOST:/absolute/path/to/flag.png' ~/Desktop/DogCTF/
```

Legacy SCP mode is `scp -O ...` if required by the server. The actual successful transfer commands were not saved in my notes, so I cannot confirm which transfer mode ultimately worked.

## 4. Conclusion

I retrieved eleven PNG flags, reaching the required 50% threshold. The most useful techniques were recognizing hidden files, understanding job control, extracting strings from executable files, and distinguishing binary file contents from their names. My own methods for Mute Dog and Spitoon Dog used manual inspection of `strings` output; they should not be confused with the alternative methods in the reference walkthrough.
