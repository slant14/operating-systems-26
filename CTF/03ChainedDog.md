# 03ChainedDog

**Category** dogctf

## Solution

1. I inspected `/home/dog/Downloads` and found many `file_….txt` files plus the anchors `start` and `end`.

   ```bash
   ls -la /home/dog/Downloads | head -n 30
   find /home/dog/Downloads -maxdepth 1 -type f ! -name 'file_*.txt' -printf '%f\n'
   ```

2. I inspected the first line and bytes of `start`. I found `Next: file_6oxl5bwrt4.txt`, a newline, the nine-byte prefix `Content: `, and the PNG signature. I also observed `Next: end` in `end`.

   ```bash
   head -n 1 /home/dog/Downloads/start
   file -- /home/dog/Downloads/start /home/dog/Downloads/end
   od -An -tx1 -N 48 -- /home/dog/Downloads/start
   ```

3. I followed the `Next` references, removed each text header and content prefix, and concatenated the binary payloads. I included the payload of `end` once and stopped there.

   ```bash
   bash <<'BASH'
   set -euo pipefail
   cd /home/dog/Downloads

   assembled_file=$(mktemp /tmp/chained-dog.XXXXXX)
   trap 'rm -f -- "$assembled_file"' EXIT

   declare -A seen=()
   current=start
   parts=0

   while true; do
       if [[ "$current" == */* || ! -f "$current" ]]; then
           printf 'Invalid filename: %s\n' "$current" >&2
           exit 1
       fi

       if [[ -n "${seen[$current]:-}" ]]; then
           printf 'Unexpected cycle at: %s\n' "$current" >&2
           exit 1
       fi
       seen[$current]=1

       {
           IFS= read -r header
           IFS= read -r -N 9 prefix

           if [[ "$header" != 'Next: '* || "$prefix" != 'Content: ' ]]; then
               printf 'Unexpected format: %s\n' "$current" >&2
               exit 1
           fi

           cat >> "$assembled_file"
       } < "$current"

       parts=$((parts + 1))
       if [[ "$current" == end ]]; then
           break
       fi
       current="${header#Next: }"
   done

   mv -- "$assembled_file" /tmp/Chained_dog.png
   printf 'Assembled %s parts\n' "$parts"
   file -- /tmp/Chained_dog.png
   BASH
   ```

4. I obtained `Assembled 222 parts` and identified the result as a 225 × 300 RGB PNG.

5. I downloaded and opened the image on my Mac.

   ```bash
   scp a.kaiumova-dog@osctf.nnk.moe:/tmp/Chained_dog.png ~/Downloads/Chained_dog.png
   open ~/Downloads/Chained_dog.png
   ```

6. I submitted `Chained_dog.png` and received `Flag accepted for '03ChainedDog'.`

## Подробный разбор

Мне нужно было восстановить картинку из цепочки файлов. В каждом звене я нашла имя следующего файла и бинарную часть изображения. Я прошла от `start` до `end`, убрала служебные заголовки и соединила 222 части.

### 1. Я нашла начало и конец цепочки

Сначала по названию задания я предположила, что речь идёт о символических ссылках. Поиск ссылок в знакомых каталогах и затем в файловой системе не дал подходящей зацепки. Позже я обнаружила домашнюю папку пользователя `dog` и осмотрела её `Downloads`.

```bash
ls -la /home/dog/Downloads | head -n 30
find /home/dog/Downloads -maxdepth 1 -type f ! -name 'file_*.txt' -printf '%f\n'
```

Я увидела множество текстовых имён вида `file_04mistcmk5.txt`, а среди имён вне этого шаблона — `start` и `end`. Это дало мне кандидатов на начало и завершение цепочки.

При пробном чтении `end` через `cat` я получила строку `Next: end`, за которой пошли бинарные данные. Так я выяснила, что расширение `.txt` и текстовый заголовок соседствуют с бинарным содержимым. После этого я использовала ограниченное чтение заголовка и шестнадцатеричный просмотр.

### 2. Я определила точный формат звена

```bash
head -n 1 /home/dog/Downloads/start
file -- /home/dog/Downloads/start /home/dog/Downloads/end
od -An -tx1 -N 48 -- /home/dog/Downloads/start
```

Через `head -n 1` я получила `Next: file_6oxl5bwrt4.txt`. С помощью `file` я определила оба файла как `data`. В выводе `od` я разобрала начало по байтам.

```text
Next: file_6oxl5bwrt4.txt\n
Content: <binary payload>
```

Байт `0a` обозначал перевод строки после имени следующего файла. Затем я увидела байты `43 6f 6e 74 65 6e 74 3a 20`, которые соответствуют девяти ASCII-символам `Content: `. Сразу за ними находилась сигнатура PNG `89 50 4e 47 0d 0a 1a 0a`.

Я определила два элемента служебного оформления, которые нужно убрать из каждого звена. Первым была вся строка `Next: …` вместе с переводом строки. Вторым был префикс `Content: ` ровно из девяти байт. В оставшихся данных могли встречаться произвольные байты, включая переводы строк, поэтому я решила переносить остаток файла напрямую через `cat`.

Порядок файлов я получала из значений `Next`. Первым звеном был `start`, следующим — `file_6oxl5bwrt4.txt`, а завершением — `end`. У `end` указатель вёл на него самого, поэтому мне требовалось добавить его данные один раз и завершить обход.

### 3. Я собрала изображение скриптом Bash

Я выполнила следующий блок в контейнере.

```bash
bash <<'BASH'
set -euo pipefail
cd /home/dog/Downloads

assembled_file=$(mktemp /tmp/chained-dog.XXXXXX)
trap 'rm -f -- "$assembled_file"' EXIT

declare -A seen=()
current=start
parts=0

while true; do
    if [[ "$current" == */* || ! -f "$current" ]]; then
        printf 'Invalid filename: %s\n' "$current" >&2
        exit 1
    fi

    if [[ -n "${seen[$current]:-}" ]]; then
        printf 'Unexpected cycle at: %s\n' "$current" >&2
        exit 1
    fi
    seen[$current]=1

    {
        IFS= read -r header
        IFS= read -r -N 9 prefix

        if [[ "$header" != 'Next: '* || "$prefix" != 'Content: ' ]]; then
            printf 'Unexpected format: %s\n' "$current" >&2
            exit 1
        fi

        cat >> "$assembled_file"
    } < "$current"

    parts=$((parts + 1))
    if [[ "$current" == end ]]; then
        break
    fi
    current="${header#Next: }"
done

mv -- "$assembled_file" /tmp/Chained_dog.png
printf 'Assembled %s parts\n' "$parts"
file -- /tmp/Chained_dog.png
BASH
```

Через `bash <<'BASH'` я передала весь блок отдельному Bash. Кавычки вокруг маркера позволили передать переменные и команды в теле блока буквально для выполнения этой оболочкой.

В `current` я хранила имя текущего звена, в `parts` — число добавленных частей. Через `declare -A seen=()` я создала ассоциативный массив посещённых имён, чтобы обнаружить неожиданный цикл. Я также проверяла, что имя не содержит `/` и соответствует существующему обычному файлу в текущей папке.

Внутри блока `{ … } < "$current"` я один раз открывала текущее звено на стандартном вводе. Первой командой `read` я читала строку `Next`, второй — ровно девять символов префикса. Я использовала `IFS=`, чтобы сохранить пробелы, и `-r`, чтобы читать обратные слеши буквально. На этом участке префикс состоял из ASCII-символов, поэтому девять символов соответствовали девяти байтам.

После этих чтений стандартный ввод находился прямо перед бинарным содержимым. Командой `cat >> "$assembled_file"` я дописывала весь остаток в результат. Бинарные части проходили непосредственно через `cat` в файл, а в переменных Bash я хранила текстовые заголовки и имена.

После добавления части я увеличивала счётчик. Если текущим именем было `end`, я выходила из цикла. Иначе через `${header#Next: }` я удаляла текстовый префикс из заголовка и получала имя следующего файла.

Я собирала результат во временном файле от `mktemp`. После успешного завершения обхода я переименовывала его в `/tmp/Chained_dog.png`. Через `trap` я настроила удаление временного пути при выходе, а через `set -euo pipefail` — завершение скрипта при ошибках команд, обращении к неопределённой переменной и ошибках конвейеров.

### 4. Я проверила результат сборки

Я получила следующий результат.

```text
Assembled 222 parts
/tmp/Chained_dog.png: PNG image data, 225 x 300, 8-bit/color RGB, non-interlaced
```

Так я подтвердила количество обработанных звеньев, включая `start` и `end`, и распознала формат собранного файла. Затем я продолжила проверку через открытие картинки и отправку боту.

### 5. Я скачала и открыла картинку

В отдельной вкладке терминала на Mac я выполнила следующие команды.

```bash
scp a.kaiumova-dog@osctf.nnk.moe:/tmp/Chained_dog.png ~/Downloads/Chained_dog.png
open ~/Downloads/Chained_dog.png
```

Первым аргументом `scp` я указала результат сборки в контейнере, вторым — локальный путь в `Downloads`. Затем я открыла картинку через `open`.

### 6. Я подтвердила решение

Я отправила `Chained_dog.png` боту и получила `Flag accepted for '03ChainedDog'.` По этому сообщению я подтвердила правильный порядок звеньев и корректное выделение бинарных частей.
