# fortforks

**Category** threadctf

## Solution

1. I connected to the challenge container and inspected the working directory and the secret source.

   ```bash
   ssh a.kaiumova-fortforks@osctf.nnk.moe
   pwd
   ls -la
   ls -l ~/secrets
   file ~/secrets
   ```

   The output confirmed that `/home/ctf/secrets` was a FIFO (named pipe):

   ```text
   /home/ctf
   prw-r--r-- 1 root root 0 Nov 22 16:34 /home/ctf/secrets
   /home/ctf/secrets: fifo (named pipe)
   ```

2. On my Mac, I created `fortforks.c` using `nano` and wrote a C reader. The program opens the FIFO once, reads continuously, and periodically calls `fork()` so the child inherits the open file descriptor. It retains the most recent 511 bytes and searches for a flag beginning with `threadctf{`. I write any recovered flag to `/home/ctf/flag_result.txt`. During debugging, I checked for that output file and for surviving reader processes. At this stage, neither was found.

   ```bash
   nano fortforks.c
   ```

3. I compiled the source on my Mac successfully. I then transferred the source to the Linux container and compiled it there because a macOS executable cannot run directly in the Linux container.

   ```bash
   clang -Wall -Wextra -O2 -o fortforks fortforks.c
   scp fortforks.c a.kaiumova-fortforks@osctf.nnk.moe:/home/ctf/
   ssh a.kaiumova-fortforks@osctf.nnk.moe
   gcc -Wall -Wextra -O2 -o fortforks fortforks.c
   ```

4. I tested the initial version. The SSH connection closed after starting the reader.

   ```bash
   ./fortforks
   ```

   ```text
   Connection to osctf.nnk.moe closed.
   ```

5. After the run, I reconnected to the container and read the saved result. **This time the command succeeded and displayed the flag.** The literal flag value was not pasted into the chat, so it is not invented here.

   ```bash
   ssh a.kaiumova-fortforks@osctf.nnk.moe
   cat /home/ctf/flag_result.txt
   ```

### Final C source

The final source was:

```c
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 4096
#define TAIL_SIZE 512

int main(void) {
    int fd = open("/home/ctf/secrets", O_RDONLY);

    if (fd < 0) {
        perror("open");
        return 1;
    }

    char buf[BUFFER_SIZE];
    char tail[TAIL_SIZE] = {0};

    size_t used = 0;
    time_t start = time(NULL);

    while (1) {
        if (time(NULL) - start >= 3) {
            pid_t pid = fork();

            if (pid < 0) {
                perror("fork");
                close(fd);
                return 1;
            }

            if (pid > 0) {
                int status;
                waitpid(pid, &status, 0);
                _exit(0);
            }

            start = time(NULL);
        }

        ssize_t n = read(fd, buf, sizeof(buf));

        if (n < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("read");
            break;
        }

        if (n == 0) {
            break;
        }

        for (ssize_t i = 0; i < n; i++) {
            if (used < TAIL_SIZE - 1) {
                tail[used++] = buf[i];
            } else {
                memmove(tail, tail + 1, TAIL_SIZE - 2);
                tail[TAIL_SIZE - 2] = buf[i];
                used = TAIL_SIZE - 1;
            }
            tail[used] = '\0';
        }

        char *p = strstr(tail, "threadctf{");

        if (p != NULL) {
            char *end = strchr(p, '}');

            if (end != NULL) {
                end[1] = '\0';

                FILE *out = fopen("/home/ctf/flag_result.txt", "w");
                if (out != NULL) {
                    fprintf(out, "%s\n", p);
                    fclose(out);
                }

                printf("\nFLAG: %s\n", p);
                fflush(stdout);
                break;
            }
        }
    }

    close(fd);
    return 0;
}
```

## Подробный разбор

### 1. Подключение и исследование окружения

```bash
ssh a.kaiumova-fortforks@osctf.nnk.moe
pwd
ls -la
ls -l ~/secrets
file ~/secrets
```

- `ssh` — подключение к удалённому Linux-контейнеру.
- `pwd` — вывод текущей директории (`/home/ctf`).
- `ls -la` — подробный список файлов, включая скрытые (`-l` — подробности, `-a` — все файлы).
- `ls -l ~/secrets` — проверка прав и типа файла по первому символу строки прав. Символ `p` обозначает FIFO.
- `file ~/secrets` — независимая проверка типа: `fifo (named pipe)`.

**Почему это важно:** FIFO — потоковый канал, а не обычный файл. Прочитанные данные нельзя вернуть с помощью `lseek()`. При отсутствии данных `read()` может блокироваться.

### 2. Создание исходника на Mac

```bash
nano fortforks.c
```

`nano` — терминальный текстовый редактор. В нём был создан C-файл с логикой чтения FIFO и смены процессов. Для сохранения в `nano` используются `Ctrl+O`, `Enter`, для выхода — `Ctrl+X`.

### 3. Компиляция и перенос исходника

```bash
clang -Wall -Wextra -O2 -o fortforks fortforks.c
scp fortforks.c a.kaiumova-fortforks@osctf.nnk.moe:/home/ctf/
ssh a.kaiumova-fortforks@osctf.nnk.moe
gcc -Wall -Wextra -O2 -o fortforks fortforks.c
```

- `clang` — компилятор C на Mac; `gcc` — компилятор в Linux-контейнере.
- `-Wall` и `-Wextra` включают предупреждения; `-O2` — оптимизация; `-o fortforks` задаёт имя исполняемого файла.
- `scp` передаёт **исходный код** с Mac в `/home/ctf/` на сервере. Если `fortforks.c` там уже существует, он заменяется. Отдельный исполняемый `fortforks` обновляется только после новой компиляции.
- Компилировать в Linux необходимо, поскольку бинарный файл macOS имеет другой формат.

### 4. Запуск

```bash
nano fortforks.c
scp fortforks.c a.kaiumova-fortforks@osctf.nnk.moe:/home/ctf/
ssh a.kaiumova-fortforks@osctf.nnk.moe
gcc -Wall -Wextra -O2 -o fortforks fortforks.c
./fortforks
```

Родитель после `fork()` вызывает `waitpid(pid, &status, 0)` и ждёт дочерний процесс. Это не гарантирует обход любого серверного лимита, но предотвращает немедленное завершение исходного родителя. После повторного запуска пользователь успешно прочитал сохранённый флаг командой `cat /home/ctf/flag_result.txt`.

### 5. Получение флага из файла

```bash
ssh a.kaiumova-fortforks@osctf.nnk.moe
cat /home/ctf/flag_result.txt
```

- `ssh` — повторное подключение после завершения предыдущей сессии.
- `cat /home/ctf/flag_result.txt` — вывод содержимого файла, в который C-программа сохранила найденный флаг. **На финальной попытке команда сработала и показала флаг.**

### 6. Как работает программа

1. `open("/home/ctf/secrets", O_RDONLY)` открывает FIFO на чтение. Дескриптор `fd` сохраняется при `fork()`.
2. `read(fd, buf, sizeof(buf))` получает очередную порцию потока; `EINTR` обрабатывается повторной попыткой.
3. `time(NULL)` измеряет время; примерно каждые три секунды вызывается `fork()`. `pid < 0` означает ошибку, `pid > 0` — родитель, `pid == 0` — ребёнок.
4. Родитель вызывает `waitpid()`; ребёнок продолжает цикл с унаследованным `fd` и обновлённым временем старта.
5. `tail` хранит последние 511 байт плюс завершающий `\0`. `memmove()` сдвигает буфер, когда он заполнен.
6. `strstr(tail, "threadctf{")` ищет начало флага, `strchr(p, '}')` — закрывающую скобку. Это позволяет находить флаг даже на границе двух чтений, если он помещается в буфер.
7. `fopen(..., "w")`, `fprintf()` и `fclose()` записывают найденный флаг в файл. `printf()` и `fflush(stdout)` также выводят его в терминал.
8. `close(fd)` закрывает дескриптор перед завершением программы.

**Ограничения:** проверка трёх секунд выполняется только между вызовами `read()`, поэтому блокирующее чтение может задержать `fork()`. Кроме того, родитель и ребёнок наследуют состояние программы на момент `fork()`, но имеют раздельную память после создания процесса. В отчёте не утверждается, что эта схема универсально обходит ограничения времени.

**Результат:** после повторного запуска команда `cat /home/ctf/flag_result.txt` успешно показала флаг.
