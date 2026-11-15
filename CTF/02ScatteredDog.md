# 02ScatteredDog

**Category** dogctf

## Solution

1. I inspected the `Scattered dog` directory and found 100 files named from `-50` to `49`.

   ```bash
   ls -la -- '/home/ctf/go_to_opt/Videos/Scattered dog'
   ```

2. I checked three pieces. I identified `-50` as the piece containing the PNG header, with dimensions 300 × 195. I got `data` for `0` and `49`.

   ```bash
   cd '/home/ctf/go_to_opt/Videos/Scattered dog'
   file -- ./-50 ./0 ./49
   ```

3. I concatenated all pieces in ascending numeric order, using Bash brace expansion to generate the filenames. I identified the result as a 300 × 195 RGBA PNG.

   ```bash
   cat -- ./{-50..49} > /tmp/Scattered_dog.png
   file -- /tmp/Scattered_dog.png
   ```

4. I downloaded and opened the image on my Mac, using the challenge SSH destination in place of `USER@HOST`.

   ```bash
   scp USER@HOST:/tmp/Scattered_dog.png ~/Downloads/Scattered_dog.png
   open ~/Downloads/Scattered_dog.png
   ```

5. I submitted `Scattered_dog.png` and received `Flag accepted for '02ScatteredDog'.`

## Подробный разбор

Мне нужно было восстановить картинку из частей, разбросанных по отдельным файлам. Я определила порядок по числовым именам и соединила все 100 частей в один PNG.

### 1. Я нашла файлы с числовыми именами

```bash
ls -la -- '/home/ctf/go_to_opt/Videos/Scattered dog'
```

Я увидела имена от `-50` до `49`, всего 100 файлов. Размер каждой части составлял 442 или 443 байта. Я заключила путь в одинарные кавычки, чтобы оболочка передала имя папки с пробелом одним аргументом.

По выводу я заметила текстовый порядок имён, например `-1`, `-10`, `-11` и позднее `-2`. Я предположила, что для восстановления потребуется порядок чисел от −50 до 49.

### 2. Я проверила несколько частей

```bash
cd '/home/ctf/go_to_opt/Videos/Scattered dog'
file -- ./-50 ./0 ./49
```

Для `./-50` я получила `PNG image data, 300 x 195, 8-bit/color RGBA, non-interlaced`. Для `./0` и `./49` я получила `data`. По этим результатам я определила, что в части `-50` находится заголовок PNG, а выбранные более поздние части содержат продолжение файла.

### 3. Я соединила части в числовом порядке

```bash
cat -- ./{-50..49} > /tmp/Scattered_dog.png
file -- /tmp/Scattered_dog.png
```

Я использовала раскрытие фигурных скобок в Bash. До запуска `cat` оболочка развернула `./{-50..49}` в аргументы `./-50`, `./-49`, далее все промежуточные числа, `./0` и до `./49` включительно. Так я задала порядок байтов в собранном файле.

Префиксом `./` я указала пути относительно текущего каталога. Благодаря этому имена с начальным минусом передавались как пути. Через `--` я также явно обозначила конец параметров команды.

С помощью `cat` я прочитала части последовательно, а перенаправлением `>` записала их содержимое в `/tmp/Scattered_dog.png`. После этого командой `file` я получила PNG размером 300 × 195 с цветом RGBA. Я учла, что распознавание заголовка само по себе ещё не гарантирует целостность всего изображения, и продолжила проверку через открытие и сдачу файла.

### 4. Я скачала и открыла картинку

Я открыла отдельную вкладку терминала на Mac и подставила вместо `USER@HOST` адрес подключения из задания.

```bash
scp USER@HOST:/tmp/Scattered_dog.png ~/Downloads/Scattered_dog.png
open ~/Downloads/Scattered_dog.png
```

Первым аргументом `scp` я указала собранный файл внутри контейнера, вторым — путь назначения на Mac. Затем я открыла локальную картинку через `open`.

### 5. Я подтвердила правильность сборки

Я отправила `Scattered_dog.png` из папки `Downloads` боту и получила `Flag accepted for '02ScatteredDog'.` По этому ответу я подтвердила, что выбрала правильный порядок всех частей.
