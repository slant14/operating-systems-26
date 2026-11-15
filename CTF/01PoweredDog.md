# 01PoweredDog

**Category** dogctf

## Solution

1. I connected to the challenge container by SSH and inspected my location and shell jobs.

   ```bash
   pwd
   ls -la
   jobs -l
   ```

   I found myself in `/home/ctf/go_to_opt`. I saw the directories `Doguments`, `Downloads`, `Pictures`, `Programs`, and `Videos`, and the file `hoi_:3.png`. I also found the stopped `background_dog` job from the previous challenge.

2. I followed the `go_to_opt` directory name as a hint. Inside the container, I ran the following commands and found a 115-byte `README.md`.

   ```bash
   cd /opt
   ls -la
   ```

3. After an SSH disconnection, I reconnected and read the file using its absolute path.

   ```bash
   cat /opt/README.md
   ```

   I found the image path in the output.

   ```text
   Congrats on discovering your address!
   Here is your dog image: /regular_paw_storage/efd72645699ee0f2/powered_dog.png
   ```

4. I copied the image to my Mac with `scp`. I used the SSH destination provided with the challenge in place of `USER@HOST`.

   ```bash
   scp USER@HOST:/regular_paw_storage/efd72645699ee0f2/powered_dog.png ~/Downloads/powered_dog.png
   ```

   I saw the transfer reach `100%`, with a displayed size of `132KB`. I confirmed the local file was a 300 × 213 RGBA PNG.

   ```bash
   file ~/Downloads/powered_dog.png
   ```

5. I submitted `powered_dog.png` and received `Flag accepted for '01PoweredDog'.`

## Подробный разбор

Мне нужно было найти картинку с собакой в контейнере и скачать её для отправки в задании.

### 1. Я подключилась и осмотрела окружение

Я вошла по SSH с адресом и паролем из задания. Затем я проверила текущую папку, её содержимое и задания оболочки.

```bash
pwd
ls -la
jobs -l
```

Командой `pwd` я получила `/home/ctf/go_to_opt`. Имя последней папки я прочитала как подсказку «перейди в /opt». Командой `ls -la` я вывела содержимое папки вместе со скрытыми элементами, правами доступа и размерами. Я увидела папки `Doguments`, `Downloads`, `Pictures`, `Programs`, `Videos` и файл `hoi_:3.png`.

В выводе `jobs -l` я увидела приостановленное задание `[1]` с PID `914881` и командой `/usr/local/bin/background_dog /tmp/bgdog`. Я узнала программу из предыдущего задания и продолжила разбирать подсказку `go_to_opt`.

### 2. Я перешла в `/opt` внутри контейнера

```bash
cd /opt
ls -la
```

Я использовала абсолютный путь `/opt`, который начинается от корня файловой системы. С помощью `cd` я сменила текущую папку, а с помощью `ls -la` увидела в ней файл `README.md` размером 115 байт. Я выбрала этот файл как следующую подсказку.

При первой попытке я выполнила эти команды на Mac и увидела папку `homebrew`. По приглашению `anastasiakaumova@MacBook-Air…` я поняла, где выполнялась команда, подключилась по SSH и повторила её уже при приглашении `ctf@…$`. Так я разобралась, что одинаковый абсолютный путь `/opt` может содержать разные файлы на Mac и внутри контейнера.

### 3. Я прочитала подсказку через `cat`

SSH-соединение оборвалось с сообщениями `Connection to osctf.nnk.moe closed by remote host` и `Broken pipe`. После этого терминал вернулся к оболочке Mac, где попытка прочитать `/opt/README.md` закончилась ошибкой `No such file or directory`. Я снова подключилась по SSH и выполнила команду внутри контейнера.

```bash
cat /opt/README.md
```

Командой `cat` я вывела содержимое текстового файла в терминал. Благодаря абсолютному пути я смогла прочитать его сразу после входа, из любой текущей папки. Я получила поздравление и путь к картинке.

```text
/regular_paw_storage/efd72645699ee0f2/powered_dog.png
```

Я заметила, что приглашение `ctf@…$` появилось сразу после `.png` в той же строке. Я отделила его от имени файла, поскольку текст в `README.md` закончился без перевода строки.

### 4. Я скачала картинку на Mac

Я открыла отдельную вкладку терминала на Mac и выполнила `scp`, подставив вместо `USER@HOST` адрес подключения из задания.

```bash
scp USER@HOST:/regular_paw_storage/efd72645699ee0f2/powered_dog.png ~/Downloads/powered_dog.png
```

В первом аргументе я указала SSH-адрес, двоеточие и абсолютный путь к исходному файлу в контейнере. Во втором я указала путь назначения на Mac. В этом терминале `~` обозначает мою домашнюю папку на Mac, поэтому картинка сохранилась в `Downloads`.

Я ввела пароль из задания и увидела строку передачи `powered_dog.png 100% 132KB`. По ней я убедилась, что копирование завершилось. При проверке локального файла командой `file` я получила PNG размером 300 × 213 с цветом RGBA.

```bash
file ~/Downloads/powered_dog.png
```

### 5. Я отправила картинку и проверила результат

Я отправила `powered_dog.png` из папки `Downloads` в ответ на задание. Бот ответил `Flag accepted for '01PoweredDog'.` По этому сообщению я подтвердила, к какому заданию относится найденная картинка.
