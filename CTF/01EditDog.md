# 01EditDog

**Category** dogctf

## Solution

1. I read the instructions and ASCII drawing in `EDITME`.

   ```bash
   cat /home/ctf/go_to_opt/Doguments/EDITME
   ```

   I found instructions to make the dog four characters longer, preserve its integrity, and keep all edits inside the safety lines.

2. I backed up the file and opened it in Nano.

   ```bash
   cp /home/ctf/go_to_opt/Doguments/EDITME /tmp/EDITME.original
   nano /home/ctf/go_to_opt/Doguments/EDITME
   ```

3. I inserted four spaces before the head on the first drawing line. I inserted four underscores into each of the two body lines, and four spaces between the left and right leg groups on each of the two bottom lines. I saved with Ctrl+O and Enter, then exited with Ctrl+X.

4. I compared the result with my backup. I saw the five changed drawing lines in the diff and received the image path from the challenge checker.

   ```bash
   diff -u /tmp/EDITME.original /home/ctf/go_to_opt/Doguments/EDITME
   ```

   ```text
   Here's your dog pic: /regular_paw_storage/4cf12d4a5a8f6c3a/Edit_dog.png
   ```

5. I downloaded and opened the image on my Mac, using the challenge SSH destination in place of `USER@HOST`.

   ```bash
   scp USER@HOST:/regular_paw_storage/4cf12d4a5a8f6c3a/Edit_dog.png ~/Downloads/Edit_dog.png
   open ~/Downloads/Edit_dog.png
   ```

6. I submitted `Edit_dog.png` and received `Flag accepted for '01EditDog'.`

## Подробный разбор

Мне нужно было удлинить собаку в текстовом рисунке на четыре символа, сохранив её форму. Я отредактировала пять строк внутри ограничителей и получила картинку после проверки файла.

### 1. Я прочитала условие в файле

```bash
cat /home/ctf/go_to_opt/Doguments/EDITME
```

Командой `cat` я вывела файл в терминал. Я прочитала требование `Make the dog 4 characters longer. Preserve the integrity of the dog.` и увидела рисунок между двумя защитными линиями. По условию я определила область допустимого редактирования.

### 2. Я сохранила исходную версию и открыла редактор

```bash
cp /home/ctf/go_to_opt/Doguments/EDITME /tmp/EDITME.original
nano /home/ctf/go_to_opt/Doguments/EDITME
```

С помощью `cp` я сохранила резервную копию для сравнения. В Nano я открыла исходный файл, который проверяло задание.

### 3. Я удлинила туловище и сохранила выравнивание

Я рассматривала рисунок как сетку символов. Чтобы удлинить туловище на четыре позиции, я сдвинула голову и правую пару лап на четыре столбца вправо. Для этого я внесла следующие изменения.

1. В верхней строке с `.-.` я вставила четыре пробела перед головой.
2. В строке спины с `()6` я вставила четыре символа `_` в длинную последовательность подчёркиваний.
3. В следующей строке туловища я также вставила четыре символа `_` в последовательность подчёркиваний.
4. В строке с лапами я вставила четыре пробела между левой и правой группами `//\\`.
5. В строке с `jgs` и ступнями я вставила четыре пробела между левой и правой группами ступней.

Я использовала пробелы для точного сдвига на четыре позиции. После этих изменений я сохранила взаимное расположение головы, туловища и лап. В Nano я нажала Ctrl+O, подтвердила имя файла клавишей Enter и вышла через Ctrl+X.

### 4. Я проверила изменения и получила путь к картинке

```bash
diff -u /tmp/EDITME.original /home/ctf/go_to_opt/Doguments/EDITME
```

Через `diff -u` я сравнила исходную и отредактированную версии. В строках с начальным `-` я увидела прежний рисунок, а в строках с начальным `+` — удлинённый. Я проверила, что изменились пять строк рисунка внутри защитных линий.

После сохранения я также увидела сообщение проверяющей программы с путём `/regular_paw_storage/4cf12d4a5a8f6c3a/Edit_dog.png`. По нему я поняла, что правка удовлетворила условию, и получила адрес картинки для скачивания.

### 5. Я скачала картинку на Mac

Я открыла отдельную вкладку терминала на Mac и подставила вместо `USER@HOST` адрес подключения из задания.

```bash
scp USER@HOST:/regular_paw_storage/4cf12d4a5a8f6c3a/Edit_dog.png ~/Downloads/Edit_dog.png
open ~/Downloads/Edit_dog.png
```

Первым аргументом `scp` я указала файл в контейнере, вторым — путь назначения на Mac. Затем я открыла локальную копию командой `open`.

### 6. Я получила подтверждение сдачи

Я отправила `Edit_dog.png` из папки `Downloads` боту. В ответ я получила `Flag accepted for '01EditDog'.` По этому сообщению я подтвердила решение задания.
