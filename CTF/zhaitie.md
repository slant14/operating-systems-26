# zhaitie

**Category** cctf — Binary decompression / static library API

## Solution

1. I read `zhaitie-AGENTS.md` to learn the library API and selected `libzhaitie-aarch64-macos.a` for my Apple Silicon Mac. The compressed input was `test.zai`.

2. The documentation specified this sequence: call `zhaitie_init()` once, create a context with `razzhaitie_new()`, pass exactly 24 bytes on the first call, and then pass exactly the number of bytes returned by each `razzhaitie()` call. A return value of zero ends the stream.

3. I wrote `decompress.c` using the documented function declarations, an output callback that writes decompressed chunks with `fwrite()`, and a loop that reads each required input window from `test.zai`. The result was written to `presentation.bin`.

   ```c
   typedef struct razzhaitie_ctx razzhaitie_ctx;
   extern void zhaitie_init(void);
   extern const char *zhaitie_version(void);
   extern razzhaitie_ctx *razzhaitie_new(void);
   extern int razzhaitie(razzhaitie_ctx *, const unsigned char *, int,
                        void (*)(const unsigned char *, int));
   ```

   The key loop was:

   ```c
   int needed = 24;
   while (needed > 0) {
       unsigned char *buffer = malloc((size_t)needed);
       if (!buffer) return 1;
       size_t count = fread(buffer, 1, (size_t)needed, input);
       if (count != (size_t)needed) { free(buffer); return 1; }
       int next = razzhaitie(ctx, buffer, needed, on_output);
       free(buffer);
       if (next < 0) return 1;
       needed = next;
   }
   ```

4. I compiled the decompressor against the macOS static library and ran it.

   ```bash
   cd ~/Downloads
   clang -O2 -o decompress decompress.c libzhaitie-aarch64-macos.a
   ./decompress
   file presentation.bin
   ```

5. I searched the restored file for the flag.

   ```bash
   strings presentation.bin | grep -i 'cctf'
   ```

   ```text
   cctf{NAtur4l_1NtELLig3nCe_oVeR_a1_d0Cs}
   ```

## Подробный разбор

В задании была только скомпилированная библиотека, сжатый файл `test.zai` и документация `AGENTS.md`. Я выбрала статическую библиотеку для macOS ARM64, так как работала на Mac с Apple Silicon.

По документации я написала C-программу, которая один раз вызывает `zhaitie_init()`, создаёт контекст `razzhaitie_new()`, считывает первые 24 байта и затем строго соблюдает размер каждого следующего блока, возвращаемый `razzhaitie()`. Распакованные данные библиотека передаёт в callback; там я сразу записывала их в `presentation.bin` через `fwrite()`. После компиляции и запуска я нашла флаг в восстановленном файле командой `strings presentation.bin | grep -i 'cctf'`.
