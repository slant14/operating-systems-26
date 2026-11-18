# dogctf - 01CompileDog

**Flag:** picture file (`Compile_dog.png`, 300x162 PNG).

## Task

`.bash_history` shows a toy workflow (`silly_dog_program.c` →
`woof_executable`), but the real task is `~/go_to_opt/Programs/compile_dog.c`
(~8 KB of a toy virtual machine: `PAWPA/PAWPB` registers, `SCRATCH`, `FETCH`,
`NIP`, `YAP_OUT`, `BARK_OFF` opcodes over a `woofscript[]` byte array).



Just compile and run it - the VM "barks" the reward path:

```sh
cd ~/go_to_opt/Programs
gcc compile_dog.c -o compile_dog
./compile_dog
# /regular_paw_storage/723125a31047b3fd/Compile_dog.png
```