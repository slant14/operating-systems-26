# DogCTF — Write-up

## Challenge: BackgroundDog

First, I navigated to the `go_to_opt` directory and inspected the available files and background jobs.

I used `bg %1` to resume the first stopped job in the background. Then I checked the image file using the `file` command.

Commands used:

```
ls
jobs
bg %1
file /regular_paw_storage/ed00980d7436cf56/Background_dog.png
```

## Challenge: CatDog

I inspected the `Pictures` directory and checked both image fragments.

The two files were concatenated into one PNG file using `cat`.

Commands used:

```
cd Pictures
file Cat_dog_top Cat_dog_bottom
cat Cat_dog_top Cat_dog_bottom > Cat_dog.png
file Cat_dog.png
```

## Challenge: CompileDog

I navigated to the `Programs` directory and compiled the provided C source code using GCC.

After executing the compiled program, I checked the resulting image file.

Commands used:

```
cd Programs
ls
gcc compile_dog.c -o compile_dog
./compile_dog
file /regular_paw_storage/400818b66ee473cc/Compile_dog.png
```

## Challenge: EditDog

I navigated to the `Doguments` directory and inspected the `EDITME` file.

I then opened it using Nano to inspect and modify its contents.

Commands used:

```
ls
cd Doguments/
ls
cat EDITME
nano EDITME
```

Found image:

```
/regular_paw_storage/0ed6ed3a37f2c092/Edit_dog.png
```

## Challenge: HelloDog

I inspected the current working directory and checked the PNG file using the `file` command.

Commands used:

```
ls
file hoi_:3.png
```

## Challenge: HistoryDog

I inspected the shell command history to identify useful previously executed commands.

Then I checked the discovered image file.

Commands used:

```
history
file /regular_paw_storage/79600c530db20c41/grandpaw.png
```

## Challenge: PoweredDog

I inspected the `/opt` directory and read the `README.md` file to find useful information.

Then I checked the discovered image file.

Commands used:

```
ls -la /opt
cat /opt/README.md
ls -l /regular_paw_storage/8af4135ba7ad3d8e/powered_dog.png
```

## Challenge: TailheadedDog

I searched the `dog.log` file for occurrences of the word `flag`.

I also inspected the beginning and the end of the log to identify relevant information.

Finally, I checked the discovered image file.

Commands used:

```
grep -in 'flag' Downloads/dog.log | head
head -n 20 Downloads/dog.log
tail -n 20 Downloads/dog.log
ls -l /regular_paw_storage/d15c0f821686c962/tailheaded.png
```

## Challenge: ScatteredDog

I inspected the contents of the `Scattered dog` directory.

The image was split into multiple numbered files. I used `cat` with brace expansion to concatenate the fragments in numerical order.

The resulting file was saved as `/tmp/scattered.png`.

Commands used:

```
ls -la "Videos/Scattered dog"
ls -v "Videos/Scattered dog"
cat -- "Videos/Scattered dog/"{-50..-1} "Videos/Scattered dog/"{0..49} > /tmp/scattered.png
file /tmp/scattered.png
```

## Challenge: ShyDog

I used a command to inspect PNG files matching the `Maybe_Dog` pattern.

Commands used:

```
cd Videos/Shy dog
ls
file *Maybe_Dog*
```

## Challenge: InsecureDog

I navigated to the `secret_stuff` directory and inspected the `remember.txt` file.

Then I entered the `super_secret` directory.

To find the image files, I used Bash brace expansion:

```
ls {D,O,G}{D,O,G}{D,O,G}{D,O,G}.png
```

This generates possible four-character filenames using the letters `D`, `O` and `G`.

After identifying the relevant file, I checked its format using the `file` command.

Commands used:

```
cd Doguments/secret_stuff
cat remember.txt
cd super_secret
ls {D,O,G}{D,O,G}{D,O,G}{D,O,G}.png
file DGOD.png
```

### Challenge: MuteDog

I executed the provided program and redirected both standard output and standard error into a file.

Then I inspected the resulting output.

Commands used:

```
cd Programs
./mute_dog > result 2>&1
cat result
```

Found image:

```
/regular_paw_storage/c9bff8df342cb80a/Mute_dog.png
```

### Challenge: SpitoonDog

I inspected the manual page for `spitoon` and executed the program.

When prompted, I entered:

```
How tough am I? How tough am I?! I ran 32 of linux programs this lab
Without 32 errors
```

I pressed `Ctrl+C` without pressing Enter after the second line.

Commands used:

```
cd Programs
man spitoon
./spitoon
```

Found image:

```
/regular_paw_storage/1fb0dd0b0d7f1b2e/Spitoon_dog.png
```

### Challenge: RestoreDog

I inspected the current directory and the `crashed.txt` file.

Then I searched for a Vim swap file and recovered its contents using Vim recovery mode.

Commands used:

```
ls
cd Doguments
ls
cat crashed.txt
ls -la
vim -r .crashed.txt.swp
```

The recovered Vim content revealed:

```
/regular_paw_storage/y16pcfy/Restore_dog.png
```

To exit Vim without saving changes:

```
:q!
```