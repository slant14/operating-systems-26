# dogctf — 01CompileDog (100 points)

**Flag:** picture file (`Compile_dog.png`, 300x162 PNG).

## Task

`.bash_history` shows a toy workflow (`silly_dog_program.c` →
`woof_executable`), but the real task is `~/go_to_opt/Programs/compile_dog.c`
(~8 KB of a toy virtual machine: `PAWPA/PAWPB` registers, `SCRATCH`, `FETCH`,
`NIP`, `YAP_OUT`, `BARK_OFF` opcodes over a `woofscript[]` byte array).

## Solution

Just compile and run it — the VM "barks" the reward path:

```sh
cd ~/go_to_opt/Programs
gcc compile_dog.c -o compile_dog
./compile_dog
# /regular_paw_storage/723125a31047b3fd/Compile_dog.png
```

Copy the PNG out (direct path access, the parent dir is not listable) and
send `Compile_dog.png.dat` to the bot as a document.

Note: sending the compiled binary or the program's text output does NOT work —
every dogctf flag is a picture file.

## Result

```
✅ Flag accepted for '01CompileDog'.
```
