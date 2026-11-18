# Dog CTF solutions by Daniil Chernykh

Here presented ways to catch some flags. I am not currently able to identity which flag is corresponding to which challenge in the telegram bot, but I left suggestions. For most of `cd` commands it is suggested that you in the initial ctf library (i.e. you just logged in).

> [!note]
> All the commands are handrewritten, so probably I missed some flags or did a few typos. Just ignore them pls :)

---

## 01BackgroundDog

```bash
fg %1
```
`/regular_paw_storage/08fc353ab2182cd5/Background_dog.png`

## 01CatDog

```bash
cd Pictures
touch Cat_dog.png
cat Cat_dog_top >> Cat_dog.png
cat Cat_dog_bottom >> Cat_dog.png
pwd
```
`/home/ctf/go_to_opt/Pictures/Cat_dog.png`

## 01CompileDog

```bash
cd Programs
gcc compile_dog.c -o dog.out
./dog.out
```
`/regular_paw_storage/ee28261f4a4d0324/Compile_dog.png`

## 01EditDog

```bash
cd Doguments
vim EDITME
```
`/regular_paw_storage/cea0e76b10488717/Edit_dog.png`

## 01HelloDog

```bash
pwd
```
`/home/ctf/go_to_opt/hoi_:3.png`

## 01HistoryDog

```bash
history
```
`/regular_paw_storage/d72728827830e46b/grandpaw.png`

## 01PoweredDog

```bash
cat /opt/README.md
```
`/regular_paw_storage/164474aa13ff39bf/powered_dog.png`

## 01TailheadedDog

```bash
cd Downloads
head dog.log -n 2
tail dog.log -n 14
```
`/regular_paw_storage/dca5e1200c9092d5/tailheaded.png`

---

## 02MuteDog

```bash
cd Programs
./mute_dog > output.txt 2> errors.txt
touch both.txt
cat output.txt >> both.txt
cat errors.txt >> both.txt
echo '' >> both.txt # New line. Otherwise the output is kinda bad
cat both.txt
```
`/regular_paw_storage/e4967c98c71b5ab6/Mute_dog.png`

## 02PetDog

```bash
cd Doguments/auto_petter
vim config/petting.cfg # Resolve merge conflict
git add config/
git rebase --continue
# Somehow resolve binary merge conflicts for three files.
# Tried to take (ours, ours, ours), (theirs, ours, theirs), (ours, ours, theirs)
# Do not work. :(
vim src/petting_core.sh # One more merge conflict
git add src/
git rebase --continue
```
`not_found`

## 02RemDog
```bash
cd Doguments/unsorted
grep -rlZ . -e '8' | xargs -r rm --
# -l = filenames
# -Z = separate with zero bit
# xargs -0 = run with args
```
`/regular_paw_storage/5a6d2f788a0aa4c3/Rem_dog.png`

## 02ScatteredDog

```bash
cd 'Videos/Scattered Dog'
vim script.sh
./script.sh
```

`script.sh`
```bash
#!/bin/bash

touch scattered.png
for ((i=-50; i<50; i++)); do
        cat "./%i" >> scattered.png
done
```
`/home/ctf/go_to_opt/Videos/Scattered\\\ dog/scattered.png`

## 02ShyDog

`not_found`

## 02SpitoonDog

`not_found`

---

## 03ChainedDog?
```bash
cp /home/dog/Downloads ~/wtf/
cd ~/wtf/Downloads
vim script.c
gcc script.c -o script.out
./script.out
```

`script.c`
```c
#include <stdio.h>
#include <string.h>

#define MAXFILENAME 128


/**
 * returns code
 * 	0 == OK
 * 	1 == EOF reached
 */
int skip(FILE* file, int chars) {
	for (int i = 0; i < chars; i++) {
		int c = fgetc(file);
		if (c == EOF) {
			printf("EOF reached in SKIP(%d)\n", chars);
			return 1;
		}
	}

	return 0;
}


int main(void) {
	char filename[MAXFILENAME];
	char next[MAXFILENAME];
	FILE* output = fopen("result.png", "wb");

	if (output == NULL) {
		printf("Cannot open result.png\n");
		return 1;
	}

	strcpy(filename, "start"); // The very beginning...

	while (1) {
		printf("READING %s...\n", filename);

		FILE* file = fopen(filename, "rb");

		if (file == NULL) {
			printf("Cannot open %s\n", filename);
			return 1;
		}

		skip(file, 6); // Skip "Next: " 

		// Catcn `next` filename
		int i = 0;
		while (1) {
		       	char c = (char)fgetc(file);
			if (c == '\n') {
				next[i] = '\0';
				break;
			}
			next[i++] = c;
		}

		skip(file, 9); // Skip "Content: "

        // Write the rest of the file content to some output file
		while (1) {
			int i = fgetc(file);
			if (i == EOF) break;
			fputc((char)i, output);
		}

		fclose(file);

		if (strcmp(filename, next) == 0) break;

		strcpy(filename, next);
	}

	fclose(output);
}
```
`/home/ctf/wtf/Downloads/result.png`

## 03NetDog

```bash
netdog # Get port
nc 127.0.0.1 1564 > netdog.png # Use port
pwd
```
`/home/ctf/go_to_opt/netdog.png`

## 03ProDog

From `htop` we know that some process `prodog` appears for half of the second once in a five seconds. I attempted to terminate or interact in another way with `prodog_loop`, but got nothing. To catch the ProDog we need to catch that fastly-dying process. To do so, I used this script:
```bash
while true; do
    pid=$(pgrep -x prodog)
    if [[ -n "$pid" ]]; then
        kill -15 "$pid"
    fi
done
```
`/regular_paw_storage/43c05fa0884de292/Pro_dog.png`

## 03RestoreDog

```bash
cd Doguments
cat crashed.txt # Get bypass code
vim crashed.txt -r # Insert bypass code, get the flag
```
`/regular_paw_storage/wm37ozy/Restore_dog.png`

## 03SpyDog

```bash
cp /home/dog/Doguments/cat_tracker ~/wtf/ -r # For better git access
cd ~/wtf/cat_tracker
git fsck --unreachable
# Traverse unreachable commits.
# Some contain one frame, some two.
# We need the second one
git checkout abaf8d705631358a0380531516332497790f99c6
# ./cat_cam/frame_0047.png is the spy dog photo
```
`/home/ctf/wtf/cat_tracker/cat_cam/frame_0047.png`

## 03UsedDog

Explore `/home`

`/home/dog/Used_dog.png`

---

## DDOG.png (03InsecureDog?)

```bash
cd Doguments/secret_stuff
chmod +r super_secret
ls super_secret
```
`/home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/DDOG.png`
