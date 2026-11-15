# 03RestoreDog

**Category** dogctf

## Solution

1. I read `crashed.txt`, checked its swap file, and confirmed that Vim was available.

   ```bash
   cat /home/ctf/go_to_opt/Doguments/crashed.txt
   file -- /home/ctf/go_to_opt/Doguments/.crashed.txt.swp
   command -v vim
   ```

   I found a note about an editor crash and the recovery key `71ffe37a75610d1f`. I got `data` for the swap file and `/usr/bin/vim` for the editor.

2. I opened Vim in recovery mode and entered the key when prompted.

   ```bash
   vim -r /home/ctf/go_to_opt/Doguments/crashed.txt
   ```

   On a repeated recovery attempt, I saw both `.crashed.txt.swo` from my newer editor session and the original `.crashed.txt.swp` from November 8. I selected option `2`, the original swap file. I recovered this path.

   ```text
   /regular_paw_storage/lxtp68u/Restore_dog.png
   ```

3. I checked the ambiguous first character of the directory name using exact candidate paths over SSH.

   ```bash
   ssh a.kaiumova-dog@osctf.nnk.moe 'for name in lxtp68u 1xtp68u Ixtp68u; do if test -r "/regular_paw_storage/$name/Restore_dog.png"; then echo "/regular_paw_storage/$name/Restore_dog.png"; fi; done'
   ```

   I confirmed the spelling `lxtp68u`, beginning with a lowercase `l`. My earlier `scp` attempt with `?xtp68u` had failed because wildcard expansion required reading the restricted parent directory.

4. I checked for remaining editor processes from my Mac. I found a recovery session with PID `937007` and sent it SIGTERM.

   ```bash
   ssh a.kaiumova-dog@osctf.nnk.moe 'ps -u ctf -o pid,stat,args'
   ssh a.kaiumova-dog@osctf.nnk.moe 'kill -TERM 937007'
   ```

   I used the PID observed in that process listing. I received no error from the signal command.

5. I downloaded and opened the image on my Mac, submitted `Restore_dog.png`, and received `Flag accepted for '03RestoreDog'.`

   ```bash
   scp a.kaiumova-dog@osctf.nnk.moe:/regular_paw_storage/lxtp68u/Restore_dog.png ~/Downloads/Restore_dog.png
   open ~/Downloads/Restore_dog.png
   ```

## Подробный разбор

Мне нужно было восстановить заметку после сбоя редактора и получить адрес картинки. Я использовала сохранённый swap-файл Vim и ключ из текстовой подсказки. Дополнительно я разобралась с режимами Vim, оставшимся процессом редактора и ограничениями доступа к каталогу с картинкой.

### 1. Я прочитала подсказку и проверила инструменты

```bash
cat /home/ctf/go_to_opt/Doguments/crashed.txt
file -- /home/ctf/go_to_opt/Doguments/.crashed.txt.swp
command -v vim
```

В `crashed.txt` я прочитала, что сессия редактора аварийно завершилась во время записи адреса любимой собаки. В заметке я также нашла указание на swap-файлы Vim и ключ восстановления `71ffe37a75610d1f`.

Командой `file` я получила для `.crashed.txt.swp` общий результат `data`. По расширению и явной подсказке я решила использовать встроенное восстановление Vim. Через `command -v vim` я убедилась, что редактор доступен по пути `/usr/bin/vim`.

### 2. Я восстановила зашифрованную заметку

```bash
vim -r /home/ctf/go_to_opt/Doguments/crashed.txt
```

Параметром `-r` я запустила восстановление по swap-файлу. При запросе ключа я использовала `71ffe37a75610d1f`. После восстановления я увидела в буфере редактора путь к картинке.

```text
/regular_paw_storage/lxtp68u/Restore_dog.png
```

При повторном открытии восстановления я получила список из двух swap-файлов. Под номером `1` находился более новый `.crashed.txt.swo` от 15 ноября, связанный с моей сессией редактора. Под номером `2` находился исходный `.crashed.txt.swp` от 8 ноября с заметкой из задания. Я выбрала `2` и нажала Enter.

Я узнала, что Vim может использовать расширение `.swo`, когда имя `.swp` уже занято. Рядом с новой сессией я увидела `STILL RUNNING` и PID `936475`. Так я поняла, что редактор обнаружил существующий процесс, связанный с этим временным файлом.

Во время работы я пыталась сохранить восстановленную строку отдельно через команду Vim `:call writefile(getline(1, '$'), '/tmp/restore_dog_path.txt')`. Команда попала в текст буфера, и успешного создания этого файла я не подтвердила. Для получения точного адреса я затем использовала проверку известных вариантов пути через SSH.

Я также разобралась, что в Vim действия зависят от режима. Для ввода команды выхода я должна была сначала нажать Esc, затем последовательно напечатать `:q!` и нажать Enter. Команда с двоеточием должна была появиться внизу экрана. В моём случае выход вызвал затруднения, и позже я завершила оставшийся процесс через отдельное SSH-соединение.

### 3. Я уточнила точное имя каталога

На скриншоте первый символ имени каталога был похож на строчную `l`, цифру `1` или заглавную `I`. Я сначала попробовала скачать файл с маской `?xtp68u` и получила ошибку.

```text
scp: remote readdir("/regular_paw_storage/"): Permission denied
```

По ошибке я поняла, что для раскрытия маски клиент попытался прочитать список имён в `/regular_paw_storage`, а права каталога этого не позволяли. Я перешла к проверке конкретных известных вариантов имени.

В терминале на Mac я выполнила следующую команду.

```bash
ssh a.kaiumova-dog@osctf.nnk.moe 'for name in lxtp68u 1xtp68u Ixtp68u; do if test -r "/regular_paw_storage/$name/Restore_dog.png"; then echo "/regular_paw_storage/$name/Restore_dog.png"; fi; done'
```

После входа по SSH удалённая оболочка перебрала три точных имени. Через `test -r` я проверила доступность каждого файла для чтения и вывела совпавший путь. Я получила `/regular_paw_storage/lxtp68u/Restore_dog.png` и определила первый символ как строчную латинскую `l`.

Так я на практике увидела разницу между просмотром списка каталога и обращением к файлу по известному имени. Я смогла прочитать картинку по точному пути при доступных правах прохода по каталогам и чтения самого файла.

### 4. Я проверила и завершила оставшуюся сессию Vim

В терминале на Mac я посмотрела процессы пользователя `ctf` в контейнере.

```bash
ssh a.kaiumova-dog@osctf.nnk.moe 'ps -u ctf -o pid,stat,args'
```

В выводе я увидела процесс `937007` с командой `vim -r /home/ctf/go_to_opt/Doguments/crashed.txt`. Я также увидела процессы SSH-соединений `dropbear`, оболочки `bash` и несколько `background_dog` в состоянии `T`, то есть приостановленных.

Я различала временные файлы `.swp` и `.swo` на диске и работающий процесс редактора с PID. Чтобы завершить найденную сессию Vim, я отправила ей SIGTERM.

```bash
ssh a.kaiumova-dog@osctf.nnk.moe 'kill -TERM 937007'
```

Я получила завершение команды без сообщения об ошибке. Отдельную повторную проверку процесса после этого я не выполняла. PID `937007` относился к этой конкретной сессии. При повторении я сначала определяю текущий PID по команде процесса и затем использую его для отправки сигнала.

### 5. Я скачала картинку и подтвердила решение

В терминале на Mac я скачала файл по точному адресу и открыла его.

```bash
scp a.kaiumova-dog@osctf.nnk.moe:/regular_paw_storage/lxtp68u/Restore_dog.png ~/Downloads/Restore_dog.png
open ~/Downloads/Restore_dog.png
```

Я указала источник в контейнере первым аргументом `scp`, а путь назначения на Mac — вторым. Затем я отправила `Restore_dog.png` из папки `Downloads` боту и получила `Flag accepted for '03RestoreDog'.` По этому ответу я подтвердила, что восстановленная заметка привела к нужной картинке.
