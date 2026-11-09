# 01BackgroundDog

**Category** Bash CTF (`dogctf`)

**Topic** Shell job control

## Solution

The challenge asks for the original dog image file from the container. When I connected, the shell reported that job `[1]`, `/usr/local/bin/background_dog /tmp/bgdog`, was stopped.

1. I ran `jobs -l` to confirm that the program was an existing shell job and had stopped on a signal. `ls -ld -- /tmp/bgdog` showed that the path passed as an argument did not exist.
2. I ran `bg %1` to resume job `[1]` in the background. The program repeatedly printed the path to the image.

   ```text
   /regular_paw_storage/2e0ef8d776993747/Background_dog.png
   ```

3. I ran `kill -STOP %1` to pause the job and stop its repeated output. I then checked the file at the path printed by the program.

   ```bash
   ls -lh -- /regular_paw_storage/2e0ef8d776993747/Background_dog.png
   file -- /regular_paw_storage/2e0ef8d776993747/Background_dog.png
   ```

   `ls -lh` showed a readable file with a displayed size of `121K`. `file` identified it as a 300 × 150 PNG with 8 bits per channel, RGBA color, and no interlacing.

4. From a local terminal, I used `scp` to copy the file out of the container. `USER@HOST` stands for the SSH destination provided with the challenge.

   ```bash
   scp USER@HOST:/regular_paw_storage/2e0ef8d776993747/Background_dog.png ~/Downloads/Background_dog.png
   file ~/Downloads/Background_dog.png
   ```

   The local copy was also identified as a 300 × 150 PNG. I submitted `Background_dog.png` on the challenge page.

## Подробный разбор

### Как прочитать сообщение оболочки

`[1]` — номер задания внутри текущей оболочки. В выводе `jobs -l` рядом с ним был `136096` — PID процесса в нашем контейнере. Поэтому в командах управления заданием мы использовали `%1`. Состояние `Stopped (signal)` означало, что процесс был приостановлен сигналом и его можно было возобновить.

### Почему сработал `bg %1`

В названии `BackgroundDog` есть подсказка про фоновый режим. `bg %1` возобновил существующее задание в фоне и сразу вернул приглашение оболочки. Процесс сохранил стандартный вывод, связанный с терминалом, поэтому мы увидели напечатанный путь, а новые строки продолжали появляться рядом с приглашением.

### Как мы нашли нужный файл

Мы сначала проверили `/tmp/bgdog`, потому что увидели его в аргументах программы. `ls` сообщил, что такого пути нет. Затем программа сама вывела другой путь — `/regular_paw_storage/2e0ef8d776993747/Background_dog.png`. Это дало нам конкретный файл для проверки.

Команда `kill -STOP %1` отправила заданию сигнал `SIGSTOP`. Повторяющиеся строки прекратились, а оболочка снова показала `Stopped`. После этого `ls -lh` подтвердил наличие файла и показал его права и размер. Команда `file` распознала PNG по содержимому. Для сдачи нужен именно этот файл.

### Как мы скачали картинку

Мы открыли отдельную вкладку терминала на Mac и запустили `scp` оттуда. У `scp` первым аргументом был источник `USER@HOST:/путь/к/файлу` внутри контейнера, а вторым — путь назначения в локальной папке `Downloads`. В `USER@HOST` подставили адрес подключения из команды `ssh`. Затем запустили `file` на Mac, чтобы убедиться, что локальная копия тоже распознаётся как PNG размером 300 × 150, и отправили этот файл в задании.
