# 03UsedDog

**Category** dogctf

## Solution

1. I searched filenames related to the remaining challenges and found `/home/dog/Used_dog.png`.

   ```bash
   find / -xdev \( -iname '*chain*' -o -iname '*spy*' -o -iname '*used*dog*' \) \
       -print 2>/dev/null | head -n 80
   ```

2. I checked the directory permissions, file permissions, and image format. I found a readable 123,852-byte PNG, identified as a 300 × 200 RGBA image.

   ```bash
   ls -ld /home/dog
   ls -l /home/dog/Used_dog.png
   file -- /home/dog/Used_dog.png
   ```

3. I downloaded and opened the image on my Mac.

   ```bash
   scp a.kaiumova-dog@osctf.nnk.moe:/home/dog/Used_dog.png ~/Downloads/Used_dog.png
   open ~/Downloads/Used_dog.png
   ```

4. I submitted `Used_dog.png` and received `Flag accepted for '03UsedDog'.`

## Подробный разбор

Мне нужно было получить картинку для `03UsedDog`. При поиске файлов для оставшихся заданий я нашла PNG в домашней папке пользователя `dog`. Я проверила права доступа, скачала картинку и подтвердила результат через бота.

### 1. Я нашла файл по имени

Я искала зацепки для оставшихся заданий по именам файлов и каталогов.

```bash
find / -xdev \( -iname '*chain*' -o -iname '*spy*' -o -iname '*used*dog*' \) \
    -print 2>/dev/null | head -n 80
```

Через `find /` я начала поиск от корня файловой системы контейнера. Параметром `-xdev` я ограничила обход одной файловой системой. Через `-iname` я задала шаблоны без учёта регистра, а через `-o` объединила их условием «или». Скобками я сгруппировала условия и экранировала скобки для оболочки.

Перенаправлением `2>/dev/null` я скрыла сообщения о недоступных каталогах. Через `head -n 80` я ограничила число строк вывода. Среди найденных объектов я увидела `/home/dog/Used_dog.png` и выбрала его для проверки.

### 2. Я проверила доступ и формат картинки

```bash
ls -ld /home/dog
ls -l /home/dog/Used_dog.png
file -- /home/dog/Used_dog.png
```

У каталога `/home/dog` я увидела владельца `dog` и права `drwxr-xr-x`. Для доступа других пользователей были доступны чтение списка и проход через каталог.

У файла я увидела владельца `dog`, права `-rw-r--r--` и размер 123 852 байта. Я работала как `ctf` и могла читать файл благодаря праву `r` для остальных пользователей.

Командой `file` я получила `PNG image data, 300 x 200, 8-bit/color RGBA, non-interlaced`. Так я подтвердила наличие PNG по содержимому файла.

### 3. Я скачала и открыла изображение

Я открыла отдельную вкладку терминала на Mac и выполнила следующие команды.

```bash
scp a.kaiumova-dog@osctf.nnk.moe:/home/dog/Used_dog.png ~/Downloads/Used_dog.png
open ~/Downloads/Used_dog.png
```

Первым аргументом `scp` я указала источник в контейнере, вторым — путь назначения на Mac. Затем я открыла локальную картинку командой `open`.

### 4. Я подтвердила решение

Я отправила `Used_dog.png` из папки `Downloads` боту и получила `Flag accepted for '03UsedDog'.` По этому ответу я подтвердила, что найденная картинка относится к этому заданию.
