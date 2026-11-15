# 03AqueousDog

**Category** dogctf

## Solution

1. I inspected the hidden configuration directory and found `fish/config.fish`.

   ```bash
   ls -laR /home/ctf/.config
   ```

2. I read the Fish configuration and found a greeting followed by a Base64-encoded path piped to `base64 -d`.

   ```bash
   cat /home/ctf/.config/fish/config.fish
   ```

3. I decoded the string and obtained the image path.

   ```bash
   echo 'L3JlZ3VsYXJfcGF3X3N0b3JhZ2UvYzA5NTM3MDM2M2YzY2ExYy9BcXVlb3VzX2RvZy5wbmc=' | base64 -d
   ```

   ```text
   /regular_paw_storage/c095370363f3ca1c/Aqueous_dog.png
   ```

4. I downloaded and opened the image on my Mac.

   ```bash
   scp a.kaiumova-dog@osctf.nnk.moe:/regular_paw_storage/c095370363f3ca1c/Aqueous_dog.png ~/Downloads/Aqueous_dog.png
   open ~/Downloads/Aqueous_dog.png
   ```

5. I submitted `Aqueous_dog.png` and received `Flag accepted for '03AqueousDog'.`

## Подробный разбор

Мне нужно было найти картинку для `03AqueousDog`. Я обнаружила подсказку в конфигурации оболочки Fish и восстановила адрес картинки из строки Base64.

### 1. Я осмотрела скрытую папку настроек

При просмотре `/home/ctf` я увидела каталог `.config`. Я проверила его содержимое вместе с вложенными папками.

```bash
ls -laR /home/ctf/.config
```

С помощью `-a` я включила скрытые имена, через `-l` получила подробные сведения, а через `-R` — рекурсивный просмотр. Я нашла папку `fish` и файл `config.fish` размером 287 байт.

### 2. Я прочитала конфигурацию Fish

```bash
cat /home/ctf/.config/fish/config.fish
```

Я увидела приветствие `Welcome to the Fish Shell!` и текст `A submerged woof echoes from the depths... the path is clear:`. Следующая команда передавала длинную строку в `base64 -d`. По сочетанию Fish и упоминания глубин я связала подсказку с названием `AqueousDog`.

Я также прочитала примечание о том, что некоторые задания предназначены для Bash. Для получения пути я выполнила отдельную команду декодирования в текущей оболочке.

### 3. Я декодировала адрес картинки

```bash
echo 'L3JlZ3VsYXJfcGF3X3N0b3JhZ2UvYzA5NTM3MDM2M2YzY2ExYy9BcXVlb3VzX2RvZy5wbmc=' | base64 -d
```

Через `echo` я вывела закодированную строку, а оператором `|` передала её в стандартный ввод `base64`. Параметром `-d` я выбрала декодирование. Base64 представляет байты с помощью текстового алфавита, поэтому для восстановления исходной строки мне было достаточно выполнить эту операцию.

Я получила следующий путь.

```text
/regular_paw_storage/c095370363f3ca1c/Aqueous_dog.png
```

Приглашение `ctf@…$` появилось сразу после `.png`, поскольку декодированная строка закончилась без перевода строки. Я выделила из вывода только путь к файлу.

### 4. Я скачала и открыла картинку

В отдельной вкладке терминала на Mac я выполнила следующие команды.

```bash
scp a.kaiumova-dog@osctf.nnk.moe:/regular_paw_storage/c095370363f3ca1c/Aqueous_dog.png ~/Downloads/Aqueous_dog.png
open ~/Downloads/Aqueous_dog.png
```

Первым аргументом `scp` я указала источник в контейнере, вторым — путь назначения в локальной папке `Downloads`. Затем я открыла картинку через `open`.

### 5. Я подтвердила соответствие заданию

Я отправила `Aqueous_dog.png` боту и получила `Flag accepted for '03AqueousDog'.` По этому сообщению я подтвердила, что правильно определила задание и восстановила нужный адрес.
