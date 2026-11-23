# zhaitie

**Category** cctf

## Solution

1. I reviewed the supplied files: `zhaitie-AGENTS.md`, the compressed `test.zai`, and static libraries for Linux and macOS. As I was using an Apple Silicon Mac, I selected `libzhaitie-aarch64-macos.a`.

   ```bash
   cd ~/Downloads
   ls -lh libzhaitie-aarch64-macos.a test.zai
   ```

2. I read the API contract in `zhaitie-AGENTS.md`. It specifies one call to `zhaitie_init()`, then `razzhaitie_new()`, followed by `razzhaitie()` calls with exactly sized input windows. The first window must be 24 bytes; every later window is determined by the preceding return value.

3. I wrote `decompress.c`. It opens `test.zai` for binary reading and `presentation.bin` for binary output, sets up a callback to write the decompressed data, and initializes the library.

   ```bash
   nano decompress.c
   ```

   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <stdint.h>
   #include <limits.h>

   typedef struct razzhaitie_ctx razzhaitie_ctx;
   extern void zhaitie_init(void);
   extern const char *zhaitie_version(void);
   extern razzhaitie_ctx *razzhaitie_new(void);
   extern int razzhaitie(razzhaitie_ctx *ctx,
                        const unsigned char *data, int size,
                        void (*callback)(const unsigned char *, int));

   static FILE *output;

   void on_output(const unsigned char *data, int size) {
       if (size > 0 && fwrite(data, 1, (size_t)size, output)
                       != (size_t)size) {
           perror("Writing output");
           exit(1);
       }
   }

   int main(void) {
       FILE *input = fopen("test.zai", "rb");
       if (!input) {
           perror("Cannot open test.zai");
           return 1;
       }

       output = fopen("presentation.bin", "wb");
       if (!output) {
           perror("Cannot create output");
           fclose(input);
           return 1;
       }

       zhaitie_init();
       fprintf(stderr, "Version: %s\n", zhaitie_version());
       razzhaitie_ctx *ctx = razzhaitie_new();
       int needed = 24;

       while (needed > 0) {
           unsigned char *buffer = malloc((size_t)needed);
           if (!buffer) {
               fprintf(stderr, "Memory allocation failed\n");
               return 1;
           }
           size_t count = fread(buffer, 1, (size_t)needed, input);
           if (count != (size_t)needed) {
               fprintf(stderr, "Unexpected end of file\n");
               free(buffer);
               return 1;
           }
           int next = razzhaitie(ctx, buffer, needed, on_output);
           free(buffer);
           if (next < 0) {
               fprintf(stderr, "Invalid window size\n");
               return 1;
           }
           needed = next;
       }

       fclose(input);
       fclose(output);
       printf("Decompression completed!\n");
       return 0;
   }
   ```

4. I compiled the source against the macOS ARM64 static library and ran the decompressor. I checked that the output file existed and inspected its type.

   ```bash
   clang -O2 -o decompress decompress.c libzhaitie-aarch64-macos.a
   ./decompress
   ls -lh presentation.bin
   file presentation.bin
   ```

5. I searched the recovered file for readable strings containing the flag prefix. `strings` and `grep` located the flag in the decompressed data.

   ```bash
   strings presentation.bin | grep -i 'cctf'
   ```

   ```text
   cctf{NAtur4l_1NtELLig3nCe_oVeR_a1_d0Cs}
   ```

## Подробный разбор

В задании `zhaitie` мне нужно было восстановить сжатую презентацию. Оригинальные исходники алгоритма были повреждены, но организаторы оставили готовые статические библиотеки для разных систем, файл `test.zai` и инструкцию `zhaitie-AGENTS.md`. Я написала собственную обёртку-декомпрессор на C, которая вызывает библиотеку в точности по её контракту.

### 1. Я выбрала библиотеку для своего Mac

```bash
cd ~/Downloads
ls -lh libzhaitie-aarch64-macos.a test.zai
```

Среди вложений были `libzhaitie-aarch64-linux.a`, `libzhaitie-x86_64-linux.a` и `libzhaitie-aarch64-macos.a`. На моём Mac используется Apple Silicon (ARM64), поэтому я выбрала macOS ARM64-версию. Расширение `.a` означает статическую библиотеку: компоновщик подключает нужные части её кода при сборке исполняемого файла.

### 2. Я изучила контракт API в `AGENTS.md`

Документация перечисляла четыре функции:

- `zhaitie_version()` возвращает строку версии;
- `zhaitie_init()` инициализирует библиотеку и должна вызываться ровно один раз до остальных функций;
- `razzhaitie_new()` возвращает контекст декомпрессии;
- `razzhaitie(ctx, data, size, callback)` получает очередной блок сжатых данных и передаёт распакованные байты в callback.

Важная особенность: первый вызов `razzhaitie()` должен получить **ровно 24 байта**. Функция возвращает число байтов, которые нужно прочитать для следующего вызова. Если возвращает `0`, поток закончен. Документация прямо предупреждает, что нельзя передавать слишком короткий блок и нельзя освобождать полученный от библиотеки контекст.

### 3. Я написала декомпрессор с callback-функцией

```bash
nano decompress.c
```

В начале `decompress.c` я объявила непрозрачный тип `razzhaitie_ctx` и сигнатуры библиотечных функций через `extern`. Структуру контекста создавать самой нельзя, поэтому я использовала указатель, возвращаемый `razzhaitie_new()`.

```c
typedef struct razzhaitie_ctx razzhaitie_ctx;
extern void zhaitie_init(void);
extern const char *zhaitie_version(void);
extern razzhaitie_ctx *razzhaitie_new(void);
extern int razzhaitie(razzhaitie_ctx *, const unsigned char *, int,
                     void (*)(const unsigned char *, int));
```

Для получения результата я написала callback, который сразу записывает каждый распакованный блок в выходной файл. Это важно: по документации указатель на память из callback нельзя сохранять после завершения вызова.

```c
static FILE *output;

void on_output(const unsigned char *data, int size) {
    if (size > 0 && fwrite(data, 1, (size_t)size, output)
                    != (size_t)size) {
        perror("Writing output");
        exit(1);
    }
}
```

### 4. Я организовала чтение блоков правильного размера

```c
zhaitie_init();
razzhaitie_ctx *ctx = razzhaitie_new();
int needed = 24;

while (needed > 0) {
    unsigned char *buffer = malloc((size_t)needed);
    if (!buffer) return 1;

    size_t count = fread(buffer, 1, (size_t)needed, input);
    if (count != (size_t)needed) {
        free(buffer);
        return 1;
    }

    int next = razzhaitie(ctx, buffer, needed, on_output);
    free(buffer);
    if (next < 0) return 1;
    needed = next;
}
```

Переменная `needed` изначально равна 24. В цикле программа выделяет буфер подходящего размера, читает из `test.zai` ровно столько байтов, вызывает `razzhaitie()` и получает размер следующего блока. При неполном чтении программа останавливается вместо передачи неправильного буфера, потому что это нарушило бы контракт библиотеки.

Данные callback записывает в `presentation.bin`, открытый в режиме `wb`. Сжатый файл читается в режиме `rb`, то есть как набор байтов, а не как текст.

### 5. Я собрала программу и восстановила файл

```bash
clang -O2 -o decompress decompress.c libzhaitie-aarch64-macos.a
./decompress
ls -lh presentation.bin
file presentation.bin
```

При компиляции я передала `clang` исходник декомпрессора и готовую библиотеку. Команда `./decompress` читает `test.zai` и сохраняет восстановленные байты в `presentation.bin`. Далее я проверила наличие полученного файла и запустила `file`, чтобы определить тип результата по содержимому, не полагаясь на расширение `.bin`.

### 6. Я нашла флаг в восстановленных данных

```bash
strings presentation.bin | grep -i 'cctf'
```

`strings` извлекает последовательности печатных символов из бинарного файла, а `grep -i 'cctf'` оставляет строки с префиксом флага без учёта регистра. Команда вывела:

```text
cctf{NAtur4l_1NtELLig3nCe_oVeR_a1_d0Cs}
```

Таким образом, мне удалось распаковать файл без исходного кода самого алгоритма: достаточно было соблюсти документированный API статической библиотеки и правильно обрабатывать входные окна и выходные callback-блоки. Найденный флаг был засчитан ботом.
