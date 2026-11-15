# 02ShyDog

**Category** dogctf

## Solution

1. I inspected the first 30 lines of the `Shy dog` directory listing and found many files with image-like names and several extensions.

   ```bash
   ls -la -- '/home/ctf/go_to_opt/Videos/Shy dog' | head -n 30
   ```

2. I checked the file contents and filtered for recognized PNG images. I found one matching result, `Winter_Maybe_Dog_470ee04f.png`, identified as a 150 × 200 PNG with an 8-bit colormap.

   ```bash
   cd '/home/ctf/go_to_opt/Videos/Shy dog'
   find . -maxdepth 1 -type f -exec file -- {} + | grep -F 'PNG image data'
   ```

3. I downloaded and opened the image on my Mac. I used the challenge SSH destination in place of `USER@HOST` and quoted the remote argument containing the directory name with a space.

   ```bash
   scp 'USER@HOST:/home/ctf/go_to_opt/Videos/Shy dog/Winter_Maybe_Dog_470ee04f.png' ~/Downloads/
   open ~/Downloads/Winter_Maybe_Dog_470ee04f.png
   ```

4. I submitted `Winter_Maybe_Dog_470ee04f.png` and received `Flag accepted for '02ShyDog'.`

## Подробный разбор

Мне нужно было найти картинку для `02ShyDog` среди множества файлов в папке `Shy dog`. Я проверила их форматы по содержимому и нашла один файл, распознанный как PNG.

### 1. Я осмотрела часть списка файлов

```bash
ls -la -- '/home/ctf/go_to_opt/Videos/Shy dog' | head -n 30
```

Через `ls -la` я получила подробный список с учётом скрытых имён, а через `head -n 30` ограничила вывод первыми 30 строками. Я увидела множество похожих имён, например `Backup_Maybe_Dog_…`, и расширения `.png`, `.jpg`, `.jpeg`, `.gif`, `.bmp`, `.img` и `.data`.

Я решила проверять формат по данным внутри файлов. Для этого я использовала команду `file`, которая распознаёт форматы по сигнатурам и структуре содержимого.

### 2. Я отобрала файлы с распознанным заголовком PNG

```bash
cd '/home/ctf/go_to_opt/Videos/Shy dog'
find . -maxdepth 1 -type f -exec file -- {} + | grep -F 'PNG image data'
```

Я заключила путь с пробелом в кавычки, чтобы передать его команде `cd` одним аргументом. С помощью `find .` я перебрала содержимое текущей папки, через `-maxdepth 1` ограничила глубину, а через `-type f` выбрала обычные файлы. Такой обход включал и скрытые имена.

Действием `-exec file -- {} +` я передала найденные пути команде `file` группами. На место `{}` подставлялись имена файлов, а `+` позволял проверять несколько файлов за один запуск. Через `--` я обозначила конец параметров `file`.

Оператором `|` я передала результаты проверки в `grep`. Параметром `-F` я задала поиск буквальной строки `PNG image data`. В результате я получила одну строку.

```text
./Winter_Maybe_Dog_470ee04f.png: PNG image data, 150 x 200, 8-bit colormap, non-interlaced
```

Так я определила нужный файл-кандидат и размеры картинки. Затем я проверила результат через открытие и отправку боту.

### 3. Я скачала и открыла картинку

В отдельной вкладке терминала на Mac я выполнила следующие команды, подставив вместо `USER@HOST` адрес подключения из задания.

```bash
scp 'USER@HOST:/home/ctf/go_to_opt/Videos/Shy dog/Winter_Maybe_Dog_470ee04f.png' ~/Downloads/
open ~/Downloads/Winter_Maybe_Dog_470ee04f.png
```

Я заключила удалённый аргумент в кавычки, поскольку в пути содержался пробел. В качестве назначения я указала папку `Downloads`, поэтому файл сохранился под исходным именем. Затем я открыла его через `open`.

### 4. Я подтвердила флаг

Я отправила `Winter_Maybe_Dog_470ee04f.png` боту и получила `Flag accepted for '02ShyDog'.` По этому ответу я подтвердила, что среди файлов нашла нужную картинку.
