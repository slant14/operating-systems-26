# 01CompileDog

**Category** dogctf

## Solution

1. I found `compile_dog.c` in `/home/ctf/go_to_opt/Programs` and inspected its beginning and end.

   ```bash
   head -n 80 /home/ctf/go_to_opt/Programs/compile_dog.c
   tail -n 100 /home/ctf/go_to_opt/Programs/compile_dog.c
   ```

   I found the byte array `woofscript` and its interpreter, `prance()`. I saw that `main()` repeatedly called `prance()`, which could output bytes through `putchar()` and terminate through `exit(0)`.

2. I compiled the source and ran the resulting executable inside the container, saving its standard output.

   ```bash
   gcc /home/ctf/go_to_opt/Programs/compile_dog.c -o /tmp/compile_dog &&
   /tmp/compile_dog > /tmp/compile_dog_output
   ```

3. I read the saved output and obtained the image path.

   ```bash
   cat /tmp/compile_dog_output
   ```

   ```text
   /regular_paw_storage/28bed636c1e6a87c/Compile_dog.png
   ```

4. I downloaded the image to my Mac.

   ```bash
   scp a.kaiumova-dog@osctf.nnk.moe:/regular_paw_storage/28bed636c1e6a87c/Compile_dog.png ~/Downloads/Compile_dog.png
   open ~/Downloads/Compile_dog.png
   ```

5. I submitted `Compile_dog.png` and received `Flag accepted for '01CompileDog'.`

## Подробный разбор

Мне нужно было получить картинку для `01CompileDog`. Я нашла исходник программы на C, собрала его компилятором и запустила. Из вывода программы я узнала путь к нужной картинке.

### 1. Я осмотрела исходник

При просмотре папки `Programs` я нашла `compile_dog.c` размером 8043 байта. Я прочитала начало и конец файла.

```bash
head -n 80 /home/ctf/go_to_opt/Programs/compile_dog.c
tail -n 100 /home/ctf/go_to_opt/Programs/compile_dog.c
```

С помощью `head -n 80` я вывела первые 80 строк, а с помощью `tail -n 100` — последние 100 строк. В начале я увидела подключения стандартных заголовков C, массив `barkbank`, переменные `pawpa`, `pawpb`, `tailchaser`, определения числовых кодов через `#define` и массив `woofscript`.

По концу файла я разобралась, что `woofscript` содержит команды для небольшого интерпретатора. В функции `prance()` я увидела чтение очередного байта через `woofscript[tailchaser++]`. По операциям внутри `switch` я определила, что программа умеет менять значения и память, выполнять XOR и выводить отдельные байты через `putchar()`.

В `main()` я увидела цикл `for (;;) prance();`. По нему я поняла, что интерпретатор будет исполнять команды последовательно. В ветке `case 0xf` я нашла `exit(0)`, а последней командой массива был `BARK_OFF`, определённый как `0xff`. Так я нашла условие завершения программы.

### 2. Я скомпилировала и запустила программу

В SSH-сессии я выполнила команды внутри контейнера.

```bash
gcc /home/ctf/go_to_opt/Programs/compile_dog.c -o /tmp/compile_dog &&
/tmp/compile_dog > /tmp/compile_dog_output
```

Через `gcc` я собрала из исходника исполняемый файл. Параметром `-o /tmp/compile_dog` я задала имя и расположение результата. Затем я запустила его по абсолютному пути `/tmp/compile_dog`.

Оператором `&&` я связала два действия так, чтобы запуск выполнялся после успешной компиляции. Перенаправлением `>` я сохранила стандартный вывод программы в `/tmp/compile_dog_output`. Благодаря этому я могла отдельно прочитать результат.

Я различала три файла по их роли. В `compile_dog.c` находился исходный код, в `/tmp/compile_dog` — исполняемая программа, а в `/tmp/compile_dog_output` — её вывод после запуска.

### 3. Я прочитала результат выполнения

```bash
cat /tmp/compile_dog_output
```

Я получила путь `/regular_paw_storage/28bed636c1e6a87c/Compile_dog.png`. Так я узнала адрес картинки, который программа сформировала при исполнении команд из массива.

### 4. Я скачала картинку

Я открыла отдельную вкладку терминала на Mac и выполнила следующие команды.

```bash
scp a.kaiumova-dog@osctf.nnk.moe:/regular_paw_storage/28bed636c1e6a87c/Compile_dog.png ~/Downloads/Compile_dog.png
open ~/Downloads/Compile_dog.png
```

В первом аргументе я указала источник внутри контейнера, во втором — место сохранения на Mac. Затем я открыла скачанный файл командой `open`.

### 5. Я отправила файл и получила подтверждение

Я отправила `Compile_dog.png` из папки `Downloads` боту. В ответ я получила `Flag accepted for '01CompileDog'.` Так я подтвердила, что результат запуска программы привёл к нужной картинке.
