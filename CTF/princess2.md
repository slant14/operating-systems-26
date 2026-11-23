# princess2

**Category** threadctf

## Solution

1. I connected to the challenge and found that the usual shell commands were disabled. The restricted environment allowed `gcc`, `vim`/`nano`, `rm`, and locally compiled executables (`./...`). Unlike the first `princess` challenge, I could not use `ps`, `awk`, or the `kill` command directly.

   ```bash
   ssh a.kaiumova-princess2@osctf.nnk.moe
   ```

   The environment displayed:

   ```text
   The forest kept your shell. It won't give it back.
   All you have left is gcc and whatever you compile.

   Allowed: gcc, vim/nano, rm and ./binaries
   Forbidden: Everything else
   ```

2. I wrote a small C program to inspect processes through Linux's `/proc` filesystem. For each numeric directory `/proc/<pid>`, it read `/proc/<pid>/status` and extracted the process name (`Name`), state (`State`), and parent PID (`PPid`). It printed only processes whose names started with `princess_`.

   ```bash
   nano inspect.c
   ```

   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>
   #include <dirent.h>
   #include <ctype.h>

   int main(void) {
       DIR *dir = opendir("/proc");

       if (dir == NULL) {
           perror("opendir");
           return 1;
       }

       struct dirent *entry;

       while ((entry = readdir(dir)) != NULL) {
           if (!isdigit((unsigned char)entry->d_name[0]))
               continue;

           char path[512];
           snprintf(path, sizeof(path),
                    "/proc/%s/status", entry->d_name);

           FILE *file = fopen(path, "r");
           if (file == NULL)
               continue;

           char line[256];
           char name[128] = "";
           char state = '?';
           int ppid = -1;

           while (fgets(line, sizeof(line), file)) {
               if (strncmp(line, "Name:", 5) == 0)
                   sscanf(line, "Name:\t%127s", name);

               if (strncmp(line, "State:", 6) == 0)
                   sscanf(line, "State:\t%c", &state);

               if (strncmp(line, "PPid:", 5) == 0)
                   sscanf(line, "PPid:\t%d", &ppid);
           }

           fclose(file);

           if (strncmp(name, "princess_", 9) == 0) {
               printf("PID=%s PPID=%d STATE=%c NAME=%s\n",
                      entry->d_name, ppid, state, name);
           }
       }

       closedir(dir);
       return 0;
   }
   ```

3. I compiled the inspection program. Initially, GCC warned that the path buffer could theoretically be too short for a directory entry name. I edited `inspect.c`, increasing `path` from 256 to 512 bytes, and compiled again.

   ```bash
   gcc -Wall -Wextra -o inspect inspect.c
   nano inspect.c
   gcc -Wall -Wextra -o inspect inspect.c
   ./inspect
   ```

   The output confirmed that I could distinguish living (`S`) from zombie (`Z`) princesses and identify each princess's parent using `PPID`:

   ```text
   PID=75 PPID=45 STATE=S NAME=princess_ember
   PID=91 PPID=67 STATE=S NAME=princess_glass
   PID=111 PPID=110 STATE=S NAME=princess_chains
   PID=116 PPID=115 STATE=S NAME=princess_grey
   PID=121 PPID=120 STATE=Z NAME=princess_heart
   PID=126 PPID=125 STATE=Z NAME=princess_wraith
   PID=130 PPID=129 STATE=Z NAME=princess_ash
   PID=134 PPID=133 STATE=Z NAME=princess_thorn
   PID=138 PPID=137 STATE=Z NAME=princess_burned
   PID=142 PPID=141 STATE=Z NAME=princess_chains
   PID=146 PPID=145 STATE=S NAME=princess_cage
   PID=150 PPID=149 STATE=Z NAME=princess_hollow
   PID=154 PPID=153 STATE=S NAME=princess_iron
   PID=159 PPID=158 STATE=Z NAME=princess_thorn
   PID=164 PPID=163 STATE=S NAME=princess_cage
   PID=168 PPID=167 STATE=Z NAME=princess_briar
   PID=178 PPID=177 STATE=Z NAME=princess_razor
   ```

4. I implemented the actual solution in C. The program repeatedly scanned `/proc`, selected only zombie princesses, looked up their parent processes, verified that the parent name contained `cabin`, rechecked the princess's state and parent, and sent `SIGTERM` to the cabin's PID. The loop targeted 100 successful signal sends.

   ```bash
   nano solve.c
   ```

   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>
   #include <dirent.h>
   #include <ctype.h>
   #include <signal.h>
   #include <unistd.h>

   #define TARGET 100

   typedef struct {
       int ppid;
       char state;
       char name[128];
   } ProcessInfo;

   /* Read process information from /proc/<pid>/status */
   int read_process(int pid, ProcessInfo *info) {
       char path[512];

       snprintf(path, sizeof(path), "/proc/%d/status", pid);

       FILE *file = fopen(path, "r");

       if (file == NULL)
           return 0;

       info->ppid = -1;
       info->state = '?';
       info->name[0] = '\0';

       char line[256];

       while (fgets(line, sizeof(line), file)) {
           if (strncmp(line, "Name:", 5) == 0)
               sscanf(line, "Name:%127s", info->name);

           if (strncmp(line, "State:", 6) == 0)
               sscanf(line, "State: %c", &info->state);

           if (strncmp(line, "PPid:", 5) == 0)
               sscanf(line, "PPid: %d", &info->ppid);
       }

       fclose(file);

       return 1;
   }

   int main(void) {
       int count = 0;

       while (count < TARGET) {
           DIR *dir = opendir("/proc");

           if (dir == NULL) {
               perror("opendir");
               return 1;
           }

           struct dirent *entry;
           int found = 0;

           while ((entry = readdir(dir)) != NULL) {
               if (!isdigit((unsigned char)entry->d_name[0]))
                   continue;

               int pid = atoi(entry->d_name);

               ProcessInfo princess;

               if (!read_process(pid, &princess))
                   continue;

               /* Only zombie princesses */
               if (princess.state != 'Z')
                   continue;

               if (strncmp(princess.name, "princess_", 9) != 0)
                   continue;

               int cabin_pid = princess.ppid;

               if (cabin_pid <= 1)
                   continue;

               ProcessInfo cabin;

               if (!read_process(cabin_pid, &cabin))
                   continue;

               /* Verify that the parent is a cabin */
               if (strstr(cabin.name, "cabin") == NULL)
                   continue;

               /* Recheck the princess before terminating its parent */
               ProcessInfo check;

               if (!read_process(pid, &check))
                   continue;

               if (check.state != 'Z' || check.ppid != cabin_pid)
                   continue;

               if (kill(cabin_pid, SIGTERM) == 0) {
                   count++;
                   found = 1;

                   printf("Removed cabin PID=%d (%d/%d)\n",
                          cabin_pid, count, TARGET);

                   fflush(stdout);

                   break;
               }
           }

           closedir(dir);

           if (!found)
               usleep(200000);
       }

       printf("Finished: %d cabins\n", count);

       return 0;
   }
   ```

5. I compiled and ran the solution.

   ```bash
   gcc -Wall -Wextra -o solve solve.c
   ./solve
   ```

   The challenge confirmed success:

   ```text
   === The Forest ===
   The forest falls silent. 100 cursed cabins lie in ruin, and
   the story is finally over. Your reward waits in your home
   directory.
   ```

6. Because `cat` was forbidden in the restricted shell, I created a separate C program to read the reward file using standard C file I/O.

   ```bash
   nano flag.c
   ```

   ```c
   #include <stdio.h>

   int main(void) {
       FILE *file = fopen("/home/ctf/flag.txt", "r");

       if (file == NULL) {
           perror("fopen");
           return 1;
       }

       int ch;

       while ((ch = fgetc(file)) != EOF) {
           putchar(ch);
       }

       fclose(file);
       return 0;
   }
   ```

   ```bash
   gcc -Wall -Wextra -o flag flag.c
   ./flag
   ```

   The program successfully printed the reward. The flag value is intentionally omitted from this report.

## Подробный разбор

### 1. Почему понадобился C

В этой версии задания вместо обычного Bash доступна ограниченная оболочка `story-shell`. Она разрешает редактировать исходники (`nano`, `vim`), компилировать их (`gcc`) и запускать созданные исполняемые файлы (`./имя`). Поэтому команды `ps`, `awk`, `kill` и `cat` из первой версии задания недоступны. Их функциональность пришлось реализовать через системные интерфейсы и стандартную библиотеку C.

### 2. Как работает `/proc`

`/proc` — виртуальная файловая система Linux, предоставляющая сведения о процессах и ядре. Для процесса с PID `121` файл `/proc/121/status` содержит текстовые поля, в том числе `Name`, `State` и `PPid`. Важно, что файлы `/proc` отражают **текущее** состояние системы: процессы могут завершаться и появляться между чтениями.

`opendir("/proc")` открывает каталог, `readdir()` последовательно возвращает его записи, а `closedir()` освобождает ресурс. В каталоге встречаются не только числовые имена, поэтому `isdigit()` помогает пропускать служебные записи. `atoi()` преобразует имя каталога в числовой PID. `snprintf()` безопаснее обычного `sprintf()`, поскольку учитывает размер буфера пути.

`fopen()` открывает `/proc/<pid>/status`; `fgets()` читает файл построчно; `strncmp()` распознаёт нужные поля; `sscanf()` извлекает их значения; `fclose()` закрывает файл. Если процесс исчез между обнаружением PID и открытием файла, `fopen()` может вернуть `NULL` — такой PID пропускается.

### 3. Что означают PID, PPID, S и Z

- **PID (Process ID)** — идентификатор процесса.
- **PPID (Parent Process ID)** — идентификатор родительского процесса.
- **S (interruptible sleep)** — процесс ожидает событие; в этом задании такие принцессы считались живыми.
- **Z (zombie)** — процесс уже завершился, но его родитель ещё не получил информацию о завершении через `wait()` или родственный вызов.

Например, строка `PID=121 PPID=120 STATE=Z NAME=princess_heart` показывает, что завершившаяся принцесса имеет PID `121`, а её родитель — PID `120`. Нельзя автоматически предполагать, что любой родитель является домиком: программа отдельно проверяет его имя.

### 4. Почему программа не трогает живых принцесс

В `solve.c` сначала проверяется `princess.state == 'Z'`, затем префикс имени `princess_`. Для выбранной принцессы программа получает `princess.ppid`, считывает данные родителя и проверяет наличие `cabin` в его имени через `strstr()`. Только после повторной проверки состояния и PPID принцессы выполняется `kill(cabin_pid, SIGTERM)`.

`kill()` — **системный вызов для отправки сигнала**, а не обязательно принудительное уничтожение. `SIGTERM` просит процесс завершиться; в отличие от `SIGKILL`, этот сигнал можно обработать. Возвращаемое значение `0` означает успешную отправку сигнала, но само по себе не доказывает, что процесс уже завершился. Фактическое выполнение цели подтверждено сообщением сервера о 100 уничтоженных домиках.

Повторная проверка уменьшает вероятность ошибки при изменении списка процессов, но не устраняет гонки полностью: между проверкой и `kill()` процесс теоретически может завершиться, а PID — переиспользоваться. В данном учебном окружении решение сработало.

### 5. Как работает цикл до 100

`#define TARGET 100` задаёт цель. Внешний цикл `while (count < TARGET)` повторяет сканирование `/proc`. Внутренний цикл перебирает процессы и ищет подходящую пару «zombie-принцесса → родитель-домик». После успешного `kill()` увеличивается `count`, печатается прогресс и выполняется `break`, чтобы начать новое сканирование со свежим списком процессов.

Если подходящий процесс не найден, `usleep(200000)` делает паузу на 200 000 микросекунд (0,2 секунды), уменьшая нагрузку на CPU. `fflush(stdout)` обеспечивает немедленный вывод прогресса. Переменная `found` показывает, была ли успешная отправка сигнала в текущем проходе.

### 6. Компиляция и получение результата

Команда `gcc -Wall -Wextra -o solve solve.c` компилирует исходный файл `solve.c` в исполняемый файл `solve`. `-Wall` и `-Wextra` включают предупреждения компилятора, а `-o` задаёт имя результата. `./solve` запускает программу из текущего каталога.

Отдельная программа `flag.c` заменяет запрещённую команду `cat`: `fopen()` открывает `/home/ctf/flag.txt`, `fgetc()` читает файл посимвольно до `EOF`, `putchar()` печатает символы, а `fclose()` закрывает файл. Команды `gcc -Wall -Wextra -o flag flag.c` и `./flag` были выполнены после подтверждения успешного прохождения задания. Само значение флага в отчёте не приводится.

### Итог

Задание решено с помощью языка C без обычных команд оболочки. Программа читала сведения о процессах из `/proc`, определяла zombie-принцесс по состоянию `Z`, находила их родительские домики через `PPID` и отправляла `SIGTERM` только проверенным родителям. Сервер подтвердил выполнение условия: уничтожено 100 проклятых домиков.
