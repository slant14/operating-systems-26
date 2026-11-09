# 01BackgroundDog

**Category** DOCCTF

## Solution

1. I ran `jobs -l` and found `/usr/local/bin/background_dog /tmp/bgdog` stopped as job `[1]`. I checked `/tmp/bgdog` with `ls -ld -- /tmp/bgdog` and got `No such file or directory`.

2. I resumed job `[1]` in the background with `bg %1` and saw the program print this path repeatedly.

   ```text
   /regular_paw_storage/2e0ef8d776993747/Background_dog.png
   ```

3. I ran `kill -STOP %1` to pause the job and stop the repeated output. I checked `/tmp/bgdog` again with `ls -lh` and `file` and got `No such file or directory`.

4. I checked the path printed by the program. `ls -lh` showed a readable file with a displayed size of `121K`, and `file` identified a 300 × 150 RGBA PNG.

   ```bash
   ls -lh -- /regular_paw_storage/2e0ef8d776993747/Background_dog.png
   file -- /regular_paw_storage/2e0ef8d776993747/Background_dog.png
   ```

5. I copied the image to my Mac with `scp`, using the SSH destination from the challenge in place of `USER@HOST`. I verified the local PNG with `file` and submitted it.

   ```bash
   scp USER@HOST:/regular_paw_storage/2e0ef8d776993747/Background_dog.png ~/Downloads/Background_dog.png
   file ~/Downloads/Background_dog.png
   ```

## Подробный разбор

Мне нужно было найти в контейнере картинку с собакой, скачать её и отправить в ответ на задание.

### 1. Я проверила состояние программы и её аргумент

После подключения по SSH я увидела, что оболочка уже запустила `/usr/local/bin/background_dog /tmp/bgdog` и показала его в состоянии `Stopped`. Я проверила задание и путь из аргумента программы.

```bash
jobs -l
ls -ld -- /tmp/bgdog
```

В выводе `jobs -l` я увидела `[1]`, PID `136096` и состояние `Stopped (signal)`. `[1]` — номер задания в моей оболочке, а `136096` — идентификатор процесса в системе. Поэтому для управления заданием из этой же SSH-сессии я могла использовать `%1`. Состояние `Stopped (signal)` означало, что процесс был приостановлен сигналом и его можно было возобновить. По выводу `ls` я узнала, что пути `/tmp/bgdog` в этот момент не было.

### 2. Я возобновила программу в фоне

Название `BackgroundDog` подсказало мне попробовать фоновый режим.

```bash
bg %1
```

`bg` возобновил задание `[1]` в фоне, и оболочка сразу вернула мне приглашение. Стандартный вывод процесса остался связан с терминалом, поэтому я увидела, как программа снова и снова печатает один и тот же путь.

```text
/regular_paw_storage/2e0ef8d776993747/Background_dog.png
```

### 3. Я приостановила процесс и перепроверила `/tmp/bgdog`

Я всё ещё могла вводить команды в той же оболочке, даже когда строки программы перекрывали приглашение. Я отправила заданию сигнал `SIGSTOP`, чтобы остановить поток вывода. Затем я проверила, появился ли `/tmp/bgdog` после запуска в фоне.

```bash
kill -STOP %1
ls -lh -- /tmp/bgdog
file -- /tmp/bgdog
```

Оболочка снова показала `Stopped`, а `ls` и `file` сообщили, что пути `/tmp/bgdog` по-прежнему нет. После этого я перешла к пути, который вывела программа.

### 4. Я проверила файл по выведенному пути

```bash
ls -lh -- /regular_paw_storage/2e0ef8d776993747/Background_dog.png
file -- /regular_paw_storage/2e0ef8d776993747/Background_dog.png
```

В выводе `ls -lh` я увидела размер `121K` и права `-rw-r--r--`. По правам было видно, что файл доступен для чтения. Затем я проверила формат по содержимому командой `file` и получила `PNG image data, 300 x 150, 8-bit/color RGBA, non-interlaced`. Так я подтвердила, что по выведенному пути действительно лежит картинка.

### 5. Я скачала и отправила картинку

Я открыла отдельную вкладку терминала на Mac и выполнила `scp` в ней. Вместо `USER@HOST` я подставила адрес подключения из команды `ssh`.

```bash
scp USER@HOST:/regular_paw_storage/2e0ef8d776993747/Background_dog.png ~/Downloads/Background_dog.png
file ~/Downloads/Background_dog.png
```

В первом аргументе `scp` я указала SSH-адрес и абсолютный путь к файлу внутри контейнера. Во втором указала, куда сохранить файл на Mac. После скачивания я запустила `file` уже на локальной копии и снова увидела PNG размером 300 × 150. Затем я отправила `Background_dog.png` из папки `Downloads` в ответ на задание.
