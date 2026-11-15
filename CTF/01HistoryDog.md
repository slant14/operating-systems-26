# 01HistoryDog

**Category** dogctf

## Solution

1. I inspected the first 40 entries in the container shell history.

   ```bash
   history | head -n 40
   ```

2. I found this copy command at entry 13 and identified its destination as the image location.

   ```bash
   cp /super_secret_doggo_vault/grandpaw.png /regular_paw_storage/1e1ab4e34f5f48a4/grandpaw.png
   ```

3. I downloaded `grandpaw.png` to my Mac and opened it.

   ```bash
   scp a.kaiumova-dog@osctf.nnk.moe:/regular_paw_storage/1e1ab4e34f5f48a4/grandpaw.png ~/Downloads/grandpaw.png
   open ~/Downloads/grandpaw.png
   ```

4. I submitted `grandpaw.png` and received `Flag accepted for '01HistoryDog'.`

## Подробный разбор

Мне нужно было найти картинку для `01HistoryDog`. Я проверила историю команд оболочки и обнаружила в ней путь, по которому ранее скопировали нужный файл.

### 1. Я посмотрела начало истории команд

Внутри SSH-сессии контейнера я выполнила команду.

```bash
history | head -n 40
```

Через `history` я вывела историю команд текущей оболочки. Оператором `|` я передала её вывод команде `head -n 40`, чтобы посмотреть первые 40 строк. Я выбрала начало истории, где могли находиться заранее оставленные подсказки.

### 2. Я нашла команду с адресом картинки

Среди команд создания файлов и вывода текста я заметила запись под номером 13.

```bash
cp /super_secret_doggo_vault/grandpaw.png /regular_paw_storage/1e1ab4e34f5f48a4/grandpaw.png
```

Я прочитала её как команду копирования. Первый путь обозначал исходный файл, а второй — файл назначения. По второму аргументу я получила конкретный адрес `/regular_paw_storage/1e1ab4e34f5f48a4/grandpaw.png`.

Я использовала запись истории как подсказку. Наличие команды в истории само по себе ещё не доказывало, что файл существует в текущий момент. Успешное скачивание по найденному пути подтвердило, что файл доступен.

### 3. Я скачала и открыла картинку

Я открыла отдельную вкладку терминала на Mac и выполнила следующие команды.

```bash
scp a.kaiumova-dog@osctf.nnk.moe:/regular_paw_storage/1e1ab4e34f5f48a4/grandpaw.png ~/Downloads/grandpaw.png
open ~/Downloads/grandpaw.png
```

В первом аргументе `scp` я указала SSH-адрес и найденный путь внутри контейнера. Во втором я указала место сохранения на Mac. Поскольку команду я запускала локально, `~` обозначал мою домашнюю папку на Mac. Через `open` я открыла скачанную картинку.

### 4. Я проверила результат сдачи

Я отправила `grandpaw.png` из папки `Downloads` боту. В ответ я получила `Flag accepted for '01HistoryDog'.` По этому сообщению я подтвердила, что путь из истории привёл к нужному флагу.
