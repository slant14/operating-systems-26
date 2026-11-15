# 01HelloDog

**Category** dogctf

## Solution

1. I listed the starting directory and found `hoi_:3.png`.

   ```bash
   cd /home/ctf/go_to_opt
   ls -la
   ```

2. I checked the file format and found a 300 × 204 RGBA PNG.

   ```bash
   file -- 'hoi_:3.png'
   ```

3. I downloaded the image and opened it on my Mac. I used the challenge SSH destination in place of `USER@HOST`.

   ```bash
   scp 'USER@HOST:/home/ctf/go_to_opt/hoi_:3.png' ~/Downloads/
   open ~/Downloads/'hoi_:3.png'
   ```

4. I submitted `hoi_:3.png` and received `Flag accepted for '01HelloDog'.`

## Подробный разбор

Мне нужно было найти картинку для `01HelloDog`. Я обнаружила готовый PNG в исходной папке контейнера, скачала его и проверила соответствие заданию через ответ бота.

### 1. Я нашла картинку в исходной папке

```bash
cd /home/ctf/go_to_opt
ls -la
```

Командой `cd` я перешла в исходную папку, а командой `ls -la` вывела её содержимое. Среди каталогов я увидела файл `hoi_:3.png` размером 60 446 байт. По имени я предположила, что эта картинка может относиться к приветственному заданию `01HelloDog`.

### 2. Я проверила формат файла

```bash
file -- 'hoi_:3.png'
```

Я получила `PNG image data, 300 x 204, 8-bit/color RGBA, non-interlaced`. С помощью `file` я определила формат по содержимому файла. По результату я узнала, что это PNG размером 300 × 204 с цветом RGBA.

### 3. Я скачала и открыла картинку

Я открыла отдельную вкладку терминала на Mac и выполнила следующие команды. Вместо `USER@HOST` я подставила SSH-адрес из задания.

```bash
scp 'USER@HOST:/home/ctf/go_to_opt/hoi_:3.png' ~/Downloads/
open ~/Downloads/'hoi_:3.png'
```

В первом аргументе `scp` я указала источник в контейнере. Во втором я указала локальную папку `Downloads`, поэтому файл сохранился в ней под исходным именем `hoi_:3.png`. Через `open` я открыла скачанную картинку.

Я заключила имя в одинарные кавычки, чтобы передать его оболочке буквально. В аргументе `scp` двоеточие после `USER@HOST` отделяло SSH-адрес от пути, а двоеточие в `hoi_:3.png` было частью имени файла.

### 4. Я подтвердила, к какому заданию относится картинка

Я отправила `hoi_:3.png` боту и получила `Flag accepted for '01HelloDog'.` По этому ответу я подтвердила своё предположение о соответствии картинки заданию.
