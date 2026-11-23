# princess

**Category** threadctf

## Solution

1. I connected to the challenge container and inspected the process tree. The `STAT` column distinguished sleeping (`S`) princess processes from zombies (`Z`). The `PPID` column identified the cabin responsible for each princess.

   ```bash
   ssh a.kaiumova-princess@osctf.nnk.moe
   pwd
   ps -eo pid,ppid,stat,comm,args | head -n 30
   ps -eo pid,ppid,stat,comm,args | grep -E 'princess|cabin'
   ```

   For example, a zombie princess with PID `16` had PPID `8`, identifying `the_fog_cabin` as its parent. A living princess with PID `18` had PPID `9`, so its parent `the_old_cabin` had to be left alone.

2. I extracted the parent PIDs of zombie princesses, without killing anything yet.

   ```bash
   ps -eo pid,ppid,stat,comm --no-headers | awk '$3 ~ /^Z/ && $4 ~ /^princess_/ {print $2}' | sort -nu
   ```

   At that point the output was:

   ```text
   122
   139
   144
   149
   168
   173
   191
   ```

3. I checked these parent processes before proceeding.

   ```bash
   ps -p 122,139,144,149,168,173,191 -o pid,ppid,stat,comm,args
   ```

   Six of the seven PIDs still existed and had cabin names. PID `122` had already disappeared. Because the process table changes over time, I did not rely on this old PID list for termination.

4. I created a Bash script to identify zombie princesses dynamically, verify their cabin parents, and send `SIGTERM` only to matching parents. The script rechecked each zombie's PID, PPID, state, and name before signaling its parent.

   ```bash
   nano solve.sh
   ```

   I entered the following script:

   ```bash
   #!/bin/bash

   count=0

   while [ "$count" -lt 50 ]; do
       found=0

       while read -r pid ppid stat comm; do
           [[ "$stat" == Z* ]] || continue
           [[ "$comm" == princess_* ]] || continue

           parent_comm=$(ps -p "$ppid" -o comm= 2>/dev/null)

           [[ "$parent_comm" == *cabin* ]] || continue

           # Recheck that this zombie still belongs to this cabin
           if ps -p "$pid" -o ppid=,stat=,comm= 2>/dev/null |
               awk -v p="$ppid" '
                   $1 == p && $2 ~ /^Z/ && $3 ~ /^princess_/ {
                       found = 1
                   }
                   END { exit !found }
               '
           then
               if kill -TERM "$ppid" 2>/dev/null; then
                   count=$((count + 1))
                   echo "Removed cabin PID=$ppid ($count/50)"
                   found=1
                   break
               fi
           fi
       done < <(ps -eo pid=,ppid=,stat=,comm=)

       if [ "$found" -eq 0 ]; then
           sleep 0.2
       fi
   done

   echo "Finished: $count cabins"
   ```

5. I made the script executable and ran it.

   ```bash
   chmod +x solve.sh
   ./solve.sh
   ```

   The challenge confirmed completion:

   ```text
   === The Forest ===
   The forest falls silent. 50 cursed cabins lie in ruin, and
   the story is finally over. Your reward waits in your home
   directory.
   ```

6. I listed the home directory to find the reward and read the flag.

   ```bash
   ls -la
   cat /home/ctf/flag.txt
   ```

   The directory contained `flag.txt`.

## Подробный разбор

### 1. Подключение и исследование процессов

```bash
ssh a.kaiumova-princess@osctf.nnk.moe
pwd
ps -eo pid,ppid,stat,comm,args | head -n 30
ps -eo pid,ppid,stat,comm,args | grep -E 'princess|cabin'
```

- `ssh` подключает к удалённому CTF-контейнеру.
- `pwd` показывает текущую директорию; результат — `/home/ctf`.
- `ps` показывает процессы. Опция `-e` выбирает все процессы, а `-o` задаёт столбцы: `pid` — ID процесса, `ppid` — ID родителя, `stat` — состояние, `comm` — короткое имя, `args` — команда запуска.
- `| head -n 30` ограничивает вывод первыми 30 строками.
- `| grep -E 'princess|cabin'` оставляет строки, где встречается `princess` **или** `cabin`; `-E` включает расширенные регулярные выражения.

**Ключевая идея:** zombie-процесс уже завершился, но его родитель ещё не забрал статус завершения через `wait()`/`waitpid()`. Состояние `Z` означает zombie, а `S` — sleeping. Уничтожать самого zombie бесполезно: завершать нужно соответствующий родительский `cabin`, причём только если его принцесса — zombie.

Например, в первоначальном выводе `princess_cage` с PID `16` имела `PPID=8` и `STAT=Z`; процесс `the_fog_cabin` имел PID `8`. Напротив, `princess_razor` с PID `18` была в состоянии `S`, поэтому её родителя с PID `9` трогать нельзя.

### 2. Поиск родительских PID zombie-принцесс

```bash
ps -eo pid,ppid,stat,comm --no-headers | awk '$3 ~ /^Z/ && $4 ~ /^princess_/ {print $2}' | sort -nu
```

- `--no-headers` убирает строку заголовков.
- `awk` обрабатывает столбцы: `$3 ~ /^Z/` означает, что состояние начинается с `Z`, а `$4 ~ /^princess_/` — что имя начинается с `princess_`.
- `{print $2}` выводит `PPID`, то есть PID родительского домика.
- `sort -n` сортирует численно; `-u` убирает повторы.

Команда вывела `122`, `139`, `144`, `149`, `168`, `173`, `191`. Это был **снимок состояния**, а не постоянный список: процессы могут исчезать или появляться.

### 3. Проверка родителей

```bash
ps -p 122,139,144,149,168,173,191 -o pid,ppid,stat,comm,args
```

- `-p` выбирает конкретные PID, перечисленные через запятую.
- `-o` задаёт поля вывода.

В результате осталось шесть процессов с именами вроде `the_moss_cabin` и `the_far_cabin`; PID `122` уже отсутствовал. Поэтому решение должно проверять актуальное состояние непосредственно перед отправкой сигнала.

### 4. Создание скрипта

```bash
nano solve.sh
```

`nano` — текстовый редактор в терминале. Я создала `solve.sh` с Bash-скриптом, приведённым в английском разделе `Solution`.

Разбор его ключевых частей:

- `#!/bin/bash` — запуск через Bash.
- `count=0` — счётчик успешных отправок сигнала.
- `while [ "$count" -lt 50 ]; do` — повторять, пока счётчик меньше 50.
- `ps -eo pid=,ppid=,stat=,comm=` — получить актуальные процессы без заголовков; знак `=` после названия поля отключает заголовок этого поля.
- `while read -r pid ppid stat comm` — читать строки списка процессов в четыре переменные. `-r` не интерпретирует обратные слеши.
- `[[ "$stat" == Z* ]]` — выбрать zombie; `[[ "$comm" == princess_* ]]` — выбрать принцесс.
- `parent_comm=$(ps -p "$ppid" -o comm= 2>/dev/null)` — узнать имя родителя; `$(...)` подставляет вывод команды, `2>/dev/null` скрывает сообщения об ошибках для исчезнувших процессов.
- `[[ "$parent_comm" == *cabin* ]]` — убедиться, что родитель похож на домик.
- Повторный `ps -p "$pid" ... | awk -v p="$ppid" ...` проверяет, что выбранная принцесса **всё ещё** zombie и её родитель не изменился. В `awk` `-v p=...` задаёт переменную `p`, а `END { exit !found }` возвращает код успеха только при совпадении.
- `kill -TERM "$ppid"` отправляет **SIGTERM родительскому домику**, а не zombie-принцессе. Это запрос на завершение процесса.
- `count=$((count + 1))` увеличивает счётчик после успешного вызова `kill`; это не самостоятельная проверка фактического завершения процесса.
- `break` прекращает текущий проход по списку, чтобы получить новый снимок процессов.
- `sleep 0.2` делает небольшую паузу, если подходящий процесс не найден, вместо постоянной нагрузки на CPU.

Поскольку между проверкой и `kill` процессы теоретически могут измениться, здесь есть небольшое окно гонки (TOCTOU). Для данного запуска сервер подтвердил достижение цели.

### 5. Запуск и подтверждение результата

```bash
chmod +x solve.sh
./solve.sh
```

- `chmod +x` добавляет файлу право на выполнение.
- `./solve.sh` запускает скрипт из текущей директории.

После выполнения сервер сообщил:

```text
=== The Forest ===
The forest falls silent. 50 cursed cabins lie in ruin, and
the story is finally over. Your reward waits in your home
directory.
```

Это подтверждает выполнение условия задачи: 50 проклятых домиков уничтожены.

### 6. Получение флага

```bash
ls -la
cat /home/ctf/flag.txt
```

- `ls -la` показывает содержимое домашней директории, включая скрытые файлы; в списке появился `flag.txt`.
- `cat /home/ctf/flag.txt` печатает содержимое файла с наградой.
