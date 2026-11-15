# 02SpitoonDog

**Category** dogctf

## Solution

1. I ran `spitoon` and captured its output streams. I found 99 bytes of ASCII text on stdout and an empty stderr.

   ```bash
   /home/ctf/go_to_opt/Programs/spitoon > /tmp/spitoon.stdout 2> /tmp/spitoon.stderr
   wc -c /tmp/spitoon.stdout /tmp/spitoon.stderr
   file -- /tmp/spitoon.stdout /tmp/spitoon.stderr
   cat /tmp/spitoon.stdout
   ```

   I received the greeting and the hint `come back when you are more of a man`.

2. I read the installed manual, which described the required responses and SIGINT handling.

   ```bash
   man -P cat spitoon
   ```

3. I started the program interactively.

   ```bash
   /home/ctf/go_to_opt/Programs/spitoon
   ```

   I entered the following response and pressed Enter.

   ```text
   How tough am I? How tough am I?! I ran 109 of linux programs this lab
   ```

4. After `Yea, so?`, I typed the following response and pressed Ctrl+C before pressing Enter.

   ```text
   Without 109 errors
   ```

   I received the actual image path.

   ```text
   Right this way sir: /regular_paw_storage/6c455644a15d4b7f/Spitoon_dog.png, sorry to keep you waiting
   ```

5. I downloaded and opened the image on my Mac, using the challenge SSH destination in place of `USER@HOST`.

   ```bash
   scp USER@HOST:/regular_paw_storage/6c455644a15d4b7f/Spitoon_dog.png ~/Downloads/Spitoon_dog.png
   open ~/Downloads/Spitoon_dog.png
   ```

6. I submitted `Spitoon_dog.png` and received `Flag accepted for '02SpitoonDog'.`

## Подробный разбор

Мне нужно было пройти проверку программы `spitoon` и получить путь к картинке. Я нашла инструкцию через `man`, ввела два точных ответа и отправила сигнал SIGINT сочетанием Ctrl+C в указанный момент.

### 1. Я проверила вывод программы

```bash
/home/ctf/go_to_opt/Programs/spitoon > /tmp/spitoon.stdout 2> /tmp/spitoon.stderr
wc -c /tmp/spitoon.stdout /tmp/spitoon.stderr
file -- /tmp/spitoon.stdout /tmp/spitoon.stderr
cat /tmp/spitoon.stdout
```

Через `>` я сохранила стандартный вывод, а через `2>` — поток ошибок. С помощью `wc -c` я получила 99 байт в первом файле и 0 во втором. Команда `file` определила первый результат как ASCII-текст, поэтому я прочитала его через `cat`.

Я получила приветствие `Welcome to the salty spitoon, how tough are ya?` и фразу `Listen, kid: come back when you are more of a man`. Я прочитала слово `man` как подсказку к команде просмотра руководств.

### 2. Я прочитала руководство

```bash
man -P cat spitoon
```

Через `man` я открыла установленную страницу руководства программы. Параметром `-P cat` я вывела её сразу в терминал, что позволило прочитать весь текст без постраничного просмотрщика.

В руководстве я нашла два точных ответа. Для первого требовалось нажать Enter. Для второго требовалось напечатать текст и сразу нажать Ctrl+C, оставив Enter ненажатым. Я также прочитала, что программа перехватывает SIGINT как часть проверки. Путь к картинке в разделе примеров я воспринимала как образец и решила получить фактический адрес через успешный диалог.

### 3. Я начала интерактивный диалог

В SSH-сессии я снова запустила программу.

```bash
/home/ctf/go_to_opt/Programs/spitoon
```

После вопроса `how tough are ya?` я ввела ответ из руководства одной строкой и нажала Enter.

```text
How tough am I? How tough am I?! I ran 109 of linux programs this lab
```

Я сохранила регистр, знаки препинания и число `109` в ответе. Затем я дождалась следующей реплики `Yea, so?`.

### 4. Я отправила второй ответ вместе с сигналом

Я напечатала второй ответ.

```text
Without 109 errors
```

Сразу после набора текста я нажала Ctrl+C, как требовало руководство. В терминале это сочетание отправляет SIGINT процессам активной группы переднего плана. По описанию программы я знала, что у неё есть обработчик этого сигнала, который участвует в проверке ответа.

После выполнения последовательности я получила сообщение.

```text
Right this way sir: /regular_paw_storage/6c455644a15d4b7f/Spitoon_dog.png, sorry to keep you waiting
```

Я выделила из него путь `/regular_paw_storage/6c455644a15d4b7f/Spitoon_dog.png`. Запятая и фраза после `.png` относились к сообщению программы.

### 5. Я скачала и открыла картинку

Я открыла отдельную вкладку терминала на Mac и подставила вместо `USER@HOST` адрес подключения из задания.

```bash
scp USER@HOST:/regular_paw_storage/6c455644a15d4b7f/Spitoon_dog.png ~/Downloads/Spitoon_dog.png
open ~/Downloads/Spitoon_dog.png
```

Первым аргументом `scp` я указала исходный файл в контейнере, вторым — место сохранения на Mac. Затем я открыла локальную картинку через `open`.

### 6. Я подтвердила решение

Я отправила `Spitoon_dog.png` из папки `Downloads` боту и получила `Flag accepted for '02SpitoonDog'.` По этому ответу я подтвердила прохождение проверки программы.
