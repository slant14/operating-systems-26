# Bash CTF (dogctf)

Every flag is a dog image, downloaded with:

```bash
scp -O a.fedotova-dog@osctf.nnk.moe:<path_on_server> ./
```

## 1. 01HelloDog

Path: `/home/ctf/go_to_opt/hoi_:3.png`

```bash
ls -la
scp -O a.fedotova-dog@osctf.nnk.moe:/home/ctf/go_to_opt/hoi_:3.png ./
```

The image is in the home folder.

## 2. 01BackgroundDog

Path: `/regular_paw_storage/9a43927853707292/Background_dog.png`

```bash
fg                 # bring the background job forward to read the path it prints
# Ctrl+Z to suspend it
scp -O a.fedotova-dog@osctf.nnk.moe:/regular_paw_storage/9a43927853707292/Background_dog.png ./
```

`background_dog` runs as a background process and keeps printing the path. `Ctrl+Z` sends SIGTSTP and the job becomes Stopped.

## 3. 01CatDog

Path: `/home/ctf/go_to_opt/Pictures/cat_dog.png`

```bash
cd Pictures
cat Cat_dog_top Cat_dog_bottom > ./cat_dog.png
scp -O a.fedotova-dog@osctf.nnk.moe:/home/ctf/go_to_opt/Pictures/cat_dog.png ./
```

`cat` concatenates the two binary halves into one PNG.

## 4. 01EditDog

Path: `/regular_paw_storage/a4364f9b0ad4ffa3/Edit_dog.png`

```bash
cd Doguments
nano EDITME        # extend the ASCII dog by exactly 5 chars, inside the safety lines; save
scp -O a.fedotova-dog@osctf.nnk.moe:/regular_paw_storage/a4364f9b0ad4ffa3/Edit_dog.png ./
```

Edit the file inside the allowed region. On save the checker prints the path.

## 5. 01CompileDog

Path: `/regular_paw_storage/e6b7c918c21f0de4/Compile_dog.png`

```bash
cd Programs
gcc compile_dog.c -o compile
./compile
scp -O a.fedotova-dog@osctf.nnk.moe:/regular_paw_storage/e6b7c918c21f0de4/Compile_dog.png ./
```

Compile the C source with gcc and run it; the program prints the path.

## 6. 01HistoryDog

Path: `/regular_paw_storage/b542a6fcfc01ea57/grandpaw.png`

```bash
cat ~/.bash_history
# line found: cp /super_secret_doggo_vault/grandpaw.png /regular_paw_storage/b542a6fcfc01ea57/grandpaw.png
scp -O a.fedotova-dog@osctf.nnk.moe:/regular_paw_storage/b542a6fcfc01ea57/grandpaw.png ./
```

`~/.bash_history` stores earlier commands; one `cp` exposes a hidden path.

## 7. 01TailheadedDog

Path: `/regular_paw_storage/ff86157d95d8ef50/tailheaded.png`

```bash
cd Downloads
head dog.log         # first half: /regular_paw_storage/ff86157d
tail -n 20 dog.log   # second half: 95d8ef50/tailheaded.png
scp -O a.fedotova-dog@osctf.nnk.moe:/regular_paw_storage/ff86157d95d8ef50/tailheaded.png ./
```

`head` and `tail` read the start and end of a large file. The path is split between them; join the halves.

## 8. 02SpitoonDog

Path: `/regular_paw_storage/d92804aab20bdac7/Spitoon_dog.png`

```bash
cd Programs
man spitoon          # read the instructions
./spitoon
# answer: How tough am I? How tough am I?! I ran 218 of linux programs this lab
# then:   Without 218 errors
# then press Ctrl+C (without Enter)
scp -O a.fedotova-dog@osctf.nnk.moe:/regular_paw_storage/d92804aab20bdac7/Spitoon_dog.png ./
```

The program installs a SIGINT handler. Normally Ctrl+C terminates a process; here the handler catches the raw signal and grants access.

## 9. 02MuteDog

Path: `/regular_paw_storage/382e788faaa97f6d/Mute_dog.png`

```bash
cd Programs
./mute_dog > out.txt     # stdout half: /regular_paw_storage/382e788f
./mute_dog 2> err.txt    # stderr half: aaa97f6d/Mute_dog.png
cat out.txt
cat err.txt
scp -O a.fedotova-dog@osctf.nnk.moe:/regular_paw_storage/382e788faaa97f6d/Mute_dog.png ./
```

The path is split across the two output streams: stdout (`>`) and stderr (`2>`). Redirect each to a file and join the halves.

## 10. 02RemDog

Path: `/home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/DGOO.png`

```bash
cd Doguments/secret_stuff
cat remember.txt
ls -la super_secret/{DDOG,DDGO,DODG,DOGD,DGDO,DGOD,ODDG,ODGD,OGDD,GDDO,GDOD,GODD,DOOG,DOGO,DGOO,ODOG,ODGO,OODG,OOGD,OGOD,OGDO,GDOO,GODO,DOGG,DGOG,DGGO,ODGG,OGDG,OGGD,GODG,GOGD,GDOG,GDGO,GGOD,GGDO}.png
# -> DGOO.png
scp -O a.fedotova-dog@osctf.nnk.moe:/home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/DGOO.png ./
```

`super_secret` has permissions `d-wx--x--x`: execute (open a file by exact name) but no read (list contents). So the directory cannot be listed, but a known filename still opens. The name is a 4-letter D/O/G combination with one doubled letter, so the answer is `DGOO`. The brace expansion tries every candidate name; the one that exists is `DGOO.png`.

## 11. 02ShyDog

Path: `/home/ctf/go_to_opt/Videos/Shy dog/Backup_Maybe_Dog_5775bcca.png`

```bash
cd 'Videos/Shy dog'
file * | grep image      # the only real picture: Backup_Maybe_Dog_5775bcca.png (PNG)
cp Backup_Maybe_Dog_5775bcca.png /tmp/shy.png
scp -O a.fedotova-dog@osctf.nnk.moe:/tmp/shy.png ./
```

The folder holds hundreds of decoy files whose extensions lie. `file` shows each file's real type from its content, so the one real image stands out.

## 12. 02ScatteredDog

Path: pieces in `/home/ctf/go_to_opt/Videos/Scattered dog/` (named `-50` to `49`)

```bash
ls 'Scattered dog' | sort -n            # see the range numerically: -50 to 49
cat 'Scattered dog'/{-50..49} > /tmp/scattered.png
file /tmp/scattered.png                 # PNG image data, 300 x 195
scp -O a.fedotova-dog@osctf.nnk.moe:/tmp/scattered.png ./
```

The dog image is split into 100 pieces named `-50` to `49`. `sort -n` sorts the names as numbers to find the range. `cat` with `{-50..49}` joins the pieces in numeric order back into the image.

## 13. 03RestoreDog

Path: `/regular_paw_storage/xdys3it/Restore_dog.png`

```bash
cd Doguments
cat crashed.txt        # recovery key: 0541578907a3ec71
vim -r crashed.txt     # recover from the swap file, enter the key, path appears
# exit vim: Esc then :q!
scp -O a.fedotova-dog@osctf.nnk.moe:/regular_paw_storage/xdys3it/Restore_dog.png ./
```

vim keeps a swap file (`.crashed.txt.swp`) to protect unsaved data after a crash. `vim -r` recovers it; the decryption key is in the readable `crashed.txt`.
