# 01TailheadedDog

**Category** dogctf

## Solution

1. I inspected the beginning and end of `dog.log`.

   ```bash
   head -n 10 /home/ctf/go_to_opt/Downloads/dog.log
   tail -n 10 /home/ctf/go_to_opt/Downloads/dog.log
   ```

   At the beginning, I found `/regular_paw_storage/2098b88f` and a hint to inspect the end. I saw only a text drawing in the last ten lines.

2. I expanded the end range to 60 lines and found the remaining path fragment, `e052669a/tailheaded.png`.

   ```bash
   tail -n 60 /home/ctf/go_to_opt/Downloads/dog.log
   ```

3. I joined the two fragments directly to obtain the full image path.

   ```text
   /regular_paw_storage/2098b88fe052669a/tailheaded.png
   ```

4. I downloaded the image and opened it on my Mac.

   ```bash
   scp a.kaiumova-dog@osctf.nnk.moe:/regular_paw_storage/2098b88fe052669a/tailheaded.png ~/Downloads/tailheaded.png
   open ~/Downloads/tailheaded.png
   ```

5. I submitted `tailheaded.png` and received `Flag accepted for '01TailheadedDog'.`

## Подробный разбор

Мне нужно было найти картинку для `01TailheadedDog`. Я обнаружила две части пути в начале и ближе к концу большого файла `dog.log`, соединила их и скачала картинку.

### 1. Я посмотрела начало и конец журнала

При осмотре папки `Downloads` я увидела файл `dog.log` размером 1 454 784 байта. По названию задания я предположила, что для поиска пригодятся команды `head` и `tail`.

```bash
head -n 10 /home/ctf/go_to_opt/Downloads/dog.log
tail -n 10 /home/ctf/go_to_opt/Downloads/dog.log
```

Командой `head -n 10` я вывела первые десять строк. В них я нашла текст `Look, a dog image path:`, фрагмент `/regular_paw_storage/2098b88f` и подсказку `I wonder if there is more at the end of it..`.

Затем командой `tail -n 10` я вывела последние десять строк. В этом диапазоне я увидела только рисунок из символов. По первой подсказке я решила посмотреть больший участок конца файла.

### 2. Я увеличила диапазон поиска

```bash
tail -n 60 /home/ctf/go_to_opt/Downloads/dog.log
```

В последних 60 строках я нашла `e052669a/tailheaded.png` и текст `I wonder if there is more at the start of it..`. Так я получила вторую половину пути и указание связать её с началом файла. Я поняла, что десяти строк было недостаточно из-за рисунка после подсказки.

### 3. Я составила полный путь

Первым фрагментом у меня был `/regular_paw_storage/2098b88f`, вторым — `e052669a/tailheaded.png`. Я приписала второй фрагмент сразу к первому и получила полный путь.

```text
/regular_paw_storage/2098b88fe052669a/tailheaded.png
```

Я объединила `2098b88f` и `e052669a` в одно имя каталога `2098b88fe052669a`. Разделитель перед именем файла уже содержался во втором фрагменте.

### 4. Я скачала и открыла картинку

Я открыла отдельную вкладку терминала на Mac и выполнила следующие команды.

```bash
scp a.kaiumova-dog@osctf.nnk.moe:/regular_paw_storage/2098b88fe052669a/tailheaded.png ~/Downloads/tailheaded.png
open ~/Downloads/tailheaded.png
```

В первом аргументе `scp` я указала источник в контейнере, во втором — путь назначения на Mac. Затем я открыла скачанную картинку командой `open`.

### 5. Я получила подтверждение флага

Я отправила `tailheaded.png` из папки `Downloads` боту. В ответ я получила `Flag accepted for '01TailheadedDog'.` По этому сообщению я подтвердила, что правильно соединила части пути и нашла нужный файл.
