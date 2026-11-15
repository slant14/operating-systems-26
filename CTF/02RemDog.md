# 02RemDog

**Category** dogctf

## Solution

1. I inspected `Doguments/unsorted` and read its README, which asked me to remove all angry dogs from that directory.

   ```bash
   ls -la /home/ctf/go_to_opt/Doguments/unsorted
   cat /home/ctf/go_to_opt/Doguments/unsorted/README
   ```

2. I compared the text drawings in two files. I identified the dog with bared teeth in `2d7554af` as the angry dog.

   ```bash
   cat /home/ctf/go_to_opt/Doguments/unsorted/06999112
   cat /home/ctf/go_to_opt/Doguments/unsorted/2d7554af
   ```

3. I copied the angry dog outside the target directory and searched for exact byte-for-byte matches. I found nine matching files.

   ```bash
   cd /home/ctf/go_to_opt/Doguments/unsorted
   cp 2d7554af /tmp/angry_dog.reference
   find . -maxdepth 1 -type f -exec cmp -s -- /tmp/angry_dog.reference {} \; -print
   ```

4. I removed the matching files and received the image path from the checker.

   ```bash
   find . -maxdepth 1 -type f -exec cmp -s -- /tmp/angry_dog.reference {} \; -exec rm -- {} \; -print
   ls -la
   ```

   ```text
   Woof! You have solved Rem_dog!
   Here's your dog pic: /regular_paw_storage/93a22bbfe927db66/Rem_dog.png
   ```

5. I downloaded and opened the image on my Mac.

   ```bash
   scp a.kaiumova-dog@osctf.nnk.moe:/regular_paw_storage/93a22bbfe927db66/Rem_dog.png ~/Downloads/Rem_dog.png
   open ~/Downloads/Rem_dog.png
   ```

6. I submitted `Rem_dog.png` and received `Flag accepted for '02RemDog'.`

## Подробный разбор

Мне нужно было удалить файлы с изображением злой собаки из папки `unsorted`. Я сравнила рисунки, выбрала образец, нашла девять точных копий и удалила их. После этого я получила путь к картинке для сдачи.

### 1. Я прочитала инструкцию в папке

```bash
ls -la /home/ctf/go_to_opt/Doguments/unsorted
cat /home/ctf/go_to_opt/Doguments/unsorted/README
```

В `unsorted` я увидела файлы с короткими шестнадцатеричными именами. У большинства был размер 1678 байт, у остальных рисунков — 1937 байт. В README я прочитала `I need to remove all the angry dogs from this folder some day...`. По этой инструкции я определила, какие файлы нужно искать.

### 2. Я сравнила рисунки

Я прочитала по одному файлу каждого из двух размеров.

```bash
cat /home/ctf/go_to_opt/Doguments/unsorted/06999112
cat /home/ctf/go_to_opt/Doguments/unsorted/2d7554af
```

В `06999112` я увидела спокойную собаку, а в `2d7554af` — собаку с оскаленными зубами. Я выбрала `2d7554af` как образец злой собаки. Для отбора остальных файлов я решила сравнивать их полное содержимое.

### 3. Я нашла девять точных копий образца

```bash
cd /home/ctf/go_to_opt/Doguments/unsorted
cp 2d7554af /tmp/angry_dog.reference
find . -maxdepth 1 -type f -exec cmp -s -- /tmp/angry_dog.reference {} \; -print
```

Я сохранила образец в `/tmp`, за пределами папки удаления. Благодаря этому он оставался доступен для сравнения даже после удаления исходного `2d7554af` из `unsorted`.

Командой `find .` я начала обход текущей папки. Через `-maxdepth 1` я ограничила его непосредственным содержимым, через `-type f` выбрала обычные файлы. Для каждого из них я запустила `cmp -s`, подставив его путь на место `{}`.

Я использовала код завершения `cmp` как условие отбора. При точном совпадении байтов он равен нулю, поэтому `find` выполнял следующее действие `-print`. Параметр `-s` отключал обычный вывод сравнения. Запись `\;` завершала аргументы `-exec`, а `--` в вызове `cmp` отделяло параметры от имён файлов.

Я получила девять совпадений.

```text
./d182e089
./cdfe126b
./707fa021
./b095b1f3
./5d97f9a2
./fc9203a1
./7adf4343
./2d7554af
./9226f7b1
```

### 4. Я удалила совпадения и получила картинку

```bash
find . -maxdepth 1 -type f -exec cmp -s -- /tmp/angry_dog.reference {} \; -exec rm -- {} \; -print
ls -la
```

К уже проверенному условию я добавила действие `-exec rm -- {} \;`. Для каждого совпавшего файла я выполняла удаление через `rm`, затем выводила имя через `-print` при успешном завершении удаления.

После удаления я получила от проверяющей программы сообщение `Woof! You have solved Rem_dog!` и путь к картинке.

```text
/regular_paw_storage/93a22bbfe927db66/Rem_dog.png
```

### 5. Я скачала и открыла результат

Я открыла отдельную вкладку терминала на Mac и выполнила следующие команды.

```bash
scp a.kaiumova-dog@osctf.nnk.moe:/regular_paw_storage/93a22bbfe927db66/Rem_dog.png ~/Downloads/Rem_dog.png
open ~/Downloads/Rem_dog.png
```

Первым аргументом `scp` я указала путь в контейнере, вторым — место сохранения на Mac. Через `open` я открыла скачанную картинку.

### 6. Я подтвердила сдачу

Я отправила `Rem_dog.png` из папки `Downloads` боту. В ответ я получила `Flag accepted for '02RemDog'.` По этому сообщению я подтвердила решение задания.
