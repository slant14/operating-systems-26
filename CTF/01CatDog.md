# 01CatDog

**Category** dogctf

## Solution

1. I inspected the challenge directories and found `Cat_dog_top` and `Cat_dog_bottom` in `Pictures`. Each file was 26,543 bytes.

   ```bash
   cd /home/ctf/go_to_opt
   ls -la -- Doguments Downloads Pictures Programs Videos
   ```

2. I checked both files with `file`. I got `PNG image data, 223 x 300, 8-bit/color RGBA, non-interlaced` for `Cat_dog_top` and `data` for `Cat_dog_bottom`.

   ```bash
   file -- Pictures/Cat_dog_top Pictures/Cat_dog_bottom
   ```

3. I concatenated the files in top-to-bottom order and saved the result. I checked the result with `file` and got a 223 × 300 RGBA PNG.

   ```bash
   cat Pictures/Cat_dog_top Pictures/Cat_dog_bottom > /tmp/Cat_dog.png
   file -- /tmp/Cat_dog.png
   ```

4. I downloaded the result from a terminal on my Mac and opened it. I used the SSH destination provided with the challenge in place of `USER@HOST`.

   ```bash
   scp USER@HOST:/tmp/Cat_dog.png ~/Downloads/Cat_dog.png
   open ~/Downloads/Cat_dog.png
   ```

5. I submitted `Cat_dog.png` and received `Flag accepted for '01CatDog'.`

## Подробный разбор

Мне нужно было найти картинку с собакой и получить файл для отправки в задании. Я нашла две части файла, соединила их и отправила собранную картинку боту.

### 1. Я нашла два подходящих файла

В SSH-сессии я перешла в исходную папку и посмотрела содержимое пяти каталогов.

```bash
cd /home/ctf/go_to_opt
ls -la -- Doguments Downloads Pictures Programs Videos
```

В `Pictures` я увидела `Cat_dog_top` и `Cat_dog_bottom`. У каждого файла был размер 26 543 байта. По словам `top` и `bottom` я предположила, что нашла две последовательные части картинки. В выводе `ls` я также увидела права `-rw-r--r--`, по которым могла определить, что оба файла доступны мне для чтения.

### 2. Я проверила содержимое частей

```bash
file -- Pictures/Cat_dog_top Pictures/Cat_dog_bottom
```

Для `Cat_dog_top` я получила `PNG image data, 223 x 300, 8-bit/color RGBA, non-interlaced`. Для `Cat_dog_bottom` я получила `data`. Командой `file` я проверила формат по содержимому, хотя в именах исходных файлов отсутствовало расширение `.png`.

По результатам я поняла, что первая часть содержит заголовок PNG с данными о формате и размерах изображения. Во второй части `file` не распознал самостоятельный формат. Это согласовывалось с предположением, что она содержит продолжение того же файла. Я учла, что распознавание заголовка первой части само по себе ещё не подтверждает целостность картинки.

### 3. Я соединила части в один файл

```bash
cat Pictures/Cat_dog_top Pictures/Cat_dog_bottom > /tmp/Cat_dog.png
file -- /tmp/Cat_dog.png
```

С помощью `cat` я последовательно прочитала байты двух файлов. Первым аргументом я указала `Cat_dog_top`, вторым — `Cat_dog_bottom`. Такой порядок сохранил заголовок PNG в начале результата.

Перенаправлением `>` я записала вывод команды в `/tmp/Cat_dog.png`. Я использовала новый путь для результата, поэтому обе исходные части сохранились. Я также учла, что `>` создаёт файл при его отсутствии и обнуляет существующий файл назначения перед записью.

У меня получилась последовательность байтов первой части, за которой сразу следует вторая часть. Затем я проверила собранный файл командой `file` и получила PNG размером 223 × 300. Окончательное подтверждение правильной сборки я получила при отправке боту.

### 4. Я скачала и открыла картинку

Я открыла отдельную вкладку терминала на Mac и выполнила команды, подставив вместо `USER@HOST` адрес из команды подключения к контейнеру.

```bash
scp USER@HOST:/tmp/Cat_dog.png ~/Downloads/Cat_dog.png
open ~/Downloads/Cat_dog.png
```

Первым аргументом `scp` я задала SSH-адрес и путь к собранному файлу в контейнере. Вторым я указала путь сохранения на Mac. Поскольку я запускала команду на Mac, `~` обозначал мою локальную домашнюю папку. Командой `open` я открыла скачанную картинку в приложении просмотра.

### 5. Я проверила принятие флага

Я отправила `Cat_dog.png` из папки `Downloads` боту и получила `Flag accepted for '01CatDog'.` По этому ответу я подтвердила, что правильно соединила части и решила именно `01CatDog`.
