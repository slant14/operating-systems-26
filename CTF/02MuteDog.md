# 02MuteDog

**Category** dogctf

## Solution

1. I ran `mute_dog` inside the container and redirected its standard output and standard error to separate files.

   ```bash
   /home/ctf/go_to_opt/Programs/mute_dog > /tmp/mute_dog.stdout 2> /tmp/mute_dog.stderr
   ```

2. I checked both outputs. I found 29 bytes in standard output and 21 bytes in standard error. I identified both as ASCII text with no line terminators.

   ```bash
   wc -c /tmp/mute_dog.stdout /tmp/mute_dog.stderr
   file -- /tmp/mute_dog.stdout /tmp/mute_dog.stderr
   ```

3. I concatenated the outputs in stdout-then-stderr order and obtained the full image path.

   ```bash
   cat /tmp/mute_dog.stdout /tmp/mute_dog.stderr
   ```

   ```text
   /regular_paw_storage/596c834bcf5318d2/Mute_dog.png
   ```

4. I downloaded and opened the image on my Mac, using the challenge SSH destination in place of `USER@HOST`.

   ```bash
   scp USER@HOST:/regular_paw_storage/596c834bcf5318d2/Mute_dog.png ~/Downloads/Mute_dog.png
   open ~/Downloads/Mute_dog.png
   ```

5. I submitted `Mute_dog.png` and received `Flag accepted for '02MuteDog'.`

## Подробный разбор

Мне нужно было найти картинку для `02MuteDog`. Я запустила программу, сохранила два потока её вывода отдельно и обнаружила, что они содержат части одного пути к картинке.

### 1. Я запустила программу с перенаправлением двух потоков

При осмотре `Programs` я нашла исполняемый файл `mute_dog`. По правам `--wx--x--x` я увидела, что могу запустить его. Внутри SSH-сессии я выполнила команду.

```bash
/home/ctf/go_to_opt/Programs/mute_dog > /tmp/mute_dog.stdout 2> /tmp/mute_dog.stderr
```

Я сохранила стандартный вывод программы, `stdout`, через `>` в `/tmp/mute_dog.stdout`. Стандартный поток ошибок, `stderr`, я сохранила через `2>` в `/tmp/mute_dog.stderr`. Цифра `2` указывает номер файлового дескриптора стандартного потока ошибок. Для стандартного вывода номером является `1`, поэтому `>` эквивалентен `1>`.

Так я получила возможность отдельно проверить оба потока. Я учитывала, что программа может записывать в `stderr` любые данные, хотя обычно его используют для ошибок и диагностических сообщений.

### 2. Я проверила размер и формат результатов

```bash
wc -c /tmp/mute_dog.stdout /tmp/mute_dog.stderr
file -- /tmp/mute_dog.stdout /tmp/mute_dog.stderr
```

Через `wc -c` я посчитала байты. Я получила 29 байт для `/tmp/mute_dog.stdout`, 21 байт для `/tmp/mute_dog.stderr` и 50 байт суммарно. С помощью `file` я определила оба результата как `ASCII text, with no line terminators`.

По результатам я поняла, что в обоих файлах находится текст без завершающего перевода строки. Я решила проверить, соединяются ли эти тексты в один путь.

### 3. Я соединила части пути

```bash
cat /tmp/mute_dog.stdout /tmp/mute_dog.stderr
```

Командой `cat` я вывела сначала содержимое файла со стандартным выводом, затем содержимое файла с потоком ошибок. Между ними команда не добавляла символов, поэтому я получила цельный путь.

```text
/regular_paw_storage/596c834bcf5318d2/Mute_dog.png
```

После `.png` я увидела приглашение `ctf@…$` в той же строке. Я отделила его от пути, поскольку файлы не содержали перевода строки в конце. Так я установила, что программа разделила адрес картинки между двумя потоками.

### 4. Я скачала и открыла картинку

Я открыла отдельную вкладку терминала на Mac и подставила вместо `USER@HOST` адрес подключения к контейнеру.

```bash
scp USER@HOST:/regular_paw_storage/596c834bcf5318d2/Mute_dog.png ~/Downloads/Mute_dog.png
open ~/Downloads/Mute_dog.png
```

Первым аргументом `scp` я указала файл в контейнере, вторым — место сохранения на Mac. Затем я открыла локальную копию командой `open`.

### 5. Я подтвердила решение

Я отправила `Mute_dog.png` из папки `Downloads` боту. В ответ я получила `Flag accepted for '02MuteDog'.` По этому сообщению я подтвердила, что правильно соединила вывод программы и нашла нужную картинку.
