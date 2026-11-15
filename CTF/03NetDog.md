# 03NetDog

**Category** dogctf

## Solution

1. I found `/usr/local/bin/netdog` and checked for usage information. I got `No manual entry for netdog`, then obtained `Using port: 2757` from a time-limited launch.

   ```bash
   man -P cat netdog
   timeout 3s /usr/local/bin/netdog --help
   ```

2. I tried a background launch with logging. I observed exit status `1` and found that `ss` was unavailable.

   ```bash
   /usr/local/bin/netdog > /tmp/netdog.log 2>&1 &
   netdog_pid=$!
   sleep 1
   cat /tmp/netdog.log
   ss -lntup
   ```

3. I checked available tools, existing processes, and the kernel socket tables. I found an existing root-owned `netdog` process with PID `215` and a TCP listener on port `2757`, represented as hexadecimal `0AC5`.

   ```bash
   command -v nc netcat ncat netstat python3
   ps -ef | grep '[n]etdog'
   grep -Hi ':0ac5' /proc/net/tcp /proc/net/tcp6 /proc/net/udp /proc/net/udp6
   ```

4. I connected to the existing listener and saved its response. I identified the response as a 300 × 222 RGBA PNG.

   ```bash
   nc -w 3 127.0.0.1 2757 < /dev/null > /tmp/netdog.response
   file -- /tmp/netdog.response
   ```

5. I downloaded the response to my Mac as `Net_dog.png` and opened it.

   ```bash
   scp a.kaiumova-dog@osctf.nnk.moe:/tmp/netdog.response ~/Downloads/Net_dog.png
   open ~/Downloads/Net_dog.png
   ```

6. I submitted `Net_dog.png` and received `Flag accepted for '03NetDog'.`

## Подробный разбор

Мне нужно было получить картинку от программы `netdog`. Я выяснила, что она уже работает как TCP-сервер в контейнере, нашла порт и сохранила полученные по сети байты в файл.

### 1. Я нашла программу и узнала порт

При осмотре `/usr/local/bin` я увидела исполняемый файл `netdog`. Сначала я попробовала открыть руководство, затем запросить справку у программы.

```bash
man -P cat netdog
timeout 3s /usr/local/bin/netdog --help
```

Для `man` я получила `No manual entry for netdog`. При запуске программы я увидела `Using port: 2757`. Через `timeout 3s` я ограничила время выполнения этой попытки. Вывод раскрыл номер порта, но не объяснил протокол взаимодействия и причину завершения программы.

### 2. Я проверила фоновый запуск

Я попробовала запустить программу в фоне, чтобы одновременно осмотреть её сетевой сокет.

```bash
/usr/local/bin/netdog > /tmp/netdog.log 2>&1 &
netdog_pid=$!
sleep 1
cat /tmp/netdog.log
ss -lntup
```

Через `>` я направила стандартный вывод в журнал, а через `2>&1` направила туда же поток ошибок. С помощью `&` я запустила процесс в фоне, а из `$!` сохранила его PID в `netdog_pid`. После короткого ожидания я прочитала журнал.

Я увидела PID `959033`, затем сообщение о завершении с кодом `1`. В журнале находилось `Using port: 2757`. При попытке осмотреть сокеты через `ss` я получила `command not found`. Тогда я решила проверить уже существующие процессы и доступные альтернативы.

### 3. Я обнаружила работающий сервер и его TCP-сокет

```bash
command -v nc netcat ncat netstat python3
ps -ef | grep '[n]etdog'
grep -Hi ':0ac5' /proc/net/tcp /proc/net/tcp6 /proc/net/udp /proc/net/udp6
```

Через `command -v` я нашла `nc`, `netcat`, `netstat` и `python3`. По выводу `ps -ef` я обнаружила `/usr/local/bin/netdog`, уже запущенный от `root` с PID `215`. Шаблон `[n]etdog` в `grep` позволил мне найти имя программы и исключить обычную строку самого поиска `grep [n]etdog`.

В `/proc/net/tcp` я нашла строку с локальным адресом `00000000:0AC5` и состоянием `0A`. В этой таблице я прочитала порт в шестнадцатеричной записи. Значение `0AC5` соответствует десятичному `2757`, а состояние `0A` означает `LISTEN`. Нулевой локальный адрес обозначал прослушивание всех IPv4-интерфейсов, поэтому я могла обратиться к серверу через локальный адрес контейнера `127.0.0.1`.

Я сопоставила существующий процесс и слушающий порт. Занятый порт был вероятной причиной неудачи моего повторного запуска. Отдельного сообщения об ошибке привязки сокета я не получила, поэтому рассматривала эту причину как вывод из наблюдений.

### 4. Я получила картинку по TCP

```bash
nc -w 3 127.0.0.1 2757 < /dev/null > /tmp/netdog.response
file -- /tmp/netdog.response
```

Через `nc` я установила TCP-соединение с `127.0.0.1` на порт `2757`. Параметром `-w 3` я задала тайм-аут ожидания. Через `< /dev/null` я предоставила пустой стандартный ввод, а через `>` сохранила полученные от сервера данные в `/tmp/netdog.response`.

После проверки командой `file` я получила `PNG image data, 300 x 222, 8-bit/color RGBA, non-interlaced`. Так я определила, что сервер передал саму картинку. Я сохранила бинарные данные напрямую в файл и использовала его для скачивания.

### 5. Я скачала и открыла изображение

В отдельной вкладке терминала на Mac я выполнила следующие команды.

```bash
scp a.kaiumova-dog@osctf.nnk.moe:/tmp/netdog.response ~/Downloads/Net_dog.png
open ~/Downloads/Net_dog.png
```

Первым аргументом `scp` я указала сохранённый ответ внутри контейнера. Во втором я задала локальное имя `Net_dog.png` в папке `Downloads`. Расширением `.png` я обозначила уже определённый по содержимому формат. Затем я открыла файл через `open`.

### 6. Я подтвердила решение

Я отправила `Net_dog.png` боту и получила `Flag accepted for '03NetDog'.` По этому ответу я подтвердила, что сохранила правильную картинку из TCP-соединения.
