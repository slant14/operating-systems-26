# DogCTF

## 01BackgroundDog

When loaded to vm, line `[1]+  Stopped                 /usr/local/bin/background_dog /tmp/bgdog` is shown
use `fg 1` to return process to foreground and get path

## Several dogs at once

`01HelloDog`, `03UsedDog`

run `find / -type f -iname "*.png" 2>/dev/null` to find all .png in ctf container

## 01TailheadedDog

- go to ~/Downloads
- use `head -n 10 dog.log` and `tail -n 10 dog.log` to get 2 parts of the key

## 01EditDog

- go to ~/Doguments
- edit `EDITME`

## 01PoweredDog

- go to /opt
- `cat README.md`

## 03InsecureDog
- go to `/home/ctf/go_to_opt/Doguments/secret_stuff`
- `chmod +r super_secret`
- get your dog

## 02RemDog

- go to `/home/ctf/go_to_opt/Doguments/unsorted`
- use `grep -l "8" * | xargs rm` (only angry dogs contain '8' in ther content, so we find names of the files that contain '8' and remove them)

## 03RestoreDog

- go to `/home/ctf/go_to_opt/Doguments`
- `cat crashed.txt` to get encryption key
- `vim crashed.txt` to open with vim, restore via key and get path

## 01CatDog

- open `/home/ctf/go_to_opt/Pictures`
- use `cat Cat_dog_top Cat_dog_bottom > test.png`

## 01HistoryDog

- use `history`
- find path in history

## 02SpitoonDog'

- go to `/home/ctf/go_to_opt/Programs`
- use `man ./spitoon`
- get run `./spitoon` with correct phrase

## 02MuteDog

- go to `/home/ctf/go_to_opt/Programs`
- use `cat ./mute_dog > test` and `cat ./mute_dog 2> test1` to get normal and error output. get key

## 01CompileDog

- go to `/home/ctf/go_to_opt/Programs`
- use `gcc compile_dog.c -o compile_dog && ./compile_dog` 

## 02ShyDog

- go to /home/ctf/go_to_opt/Videos/Shy dog
- use `file * | grep -i "PNG image data"` (get file signatures and grep only png)

## 03NetDog

- use `netstat -lnt` to check ports available to connect
- use `nc 0.0.0.0 3512 > test2.png` to download dog


# CCTF

## cavern

### Given

cavern.c program, which loads shared object, checks the guess and gives some answer

### Steps

- cavern.c contains `guess()` function, which return pointer to `offering` structure (after freeing this structure)
- free() function doesnt remove content of the freed area, just marks it as unused, so content of the `offering` structure still exist in place
- `secret` field of structure is stored with 64 byte offset from the pointer to the structure, so it can be directly printed from `run()` function of shared object 


## legacy

### Steps

- rewrite gcc as '18-byte for unsigned' compiler 
- add 0x3FFFF (2^18 - 1) mask to position function

### xorcism

### Givem

some program, which checks given password with encrypted hardcoded password

### Steps

remove checking part, replace it with just pinting decrypted hardcoded version

``` C
for (int i = 0; i < PWLEN; i++) {
        printf("%c", enc[i] ^ key[i % 4]);
    }
```

# ThreadCTF

## decrypt

### Given

Bot gives you the torrent file and shard archive

### Steps

- You need to download `decrapinator-x86_64-linux` from `Decrapinator.v2.Linux.MacOS.Cracked.torrent`
- `decrapinator-x86_64-linux` works via `./decrapinator-x86_64-linux decrypt [shard_location]` command
- one run of decrapinator takes around 10 seconds and may fail, so you need to write thread based program to run several instances of decrapinator at once
- program uses `"./decrapinator-x86_64-linux decrypt ./shards/%d.shard | tail -n 1"` command to run decrapinator and stdout (several symbols in one line for successfull run or nothing for failed run)
- I used 500 threads (10 for each shard) to deal with failed runs and then combined successfull attempts in one string


## princess

### Given

system contains several cabins that create zombie processes, we need to find that cabins and kill them, but we cannot kill cabins, that contain living princesses

### Steps

- Run `ps -eo pid,ppid,stat,comm,args --forest` to see the process tree: each cabin is a parent of a princess and several other processes
- A princess with state `Z` is a zombie. Its `PPID` identifies the cursed cabin
- write a program that scans `/proc` and reads each process's `stat` file to find zombie processes whose names start with `princess_`
- check that the parent process runs `/usr/local/bin/cabin`, then recheck that the princess is still a zombie belonging to that parent
- Open a process file descriptor for the cabin and use it to send `SIGTERM`. This prevents the signal from being sent to a different process if the cabin's PID is reused
- Repeat the scan as new cabins appear, tracking cabin PIDs to avoid counting the same cabin twice, until signals have been sent to 50 cabins


## princess 2

same things except you cannot use bash commands

run same c program but with 100 cabins instead of 50 (and wait 15 minutes)


