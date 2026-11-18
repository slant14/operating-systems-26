# legacy

**Category** cctf

## Solution

1. I read the challenge description and the supplied `legacy.c`. The original developer's diagnostics reported an 18-bit `unsigned` type and a check value of `237614`, whereas modern macOS uses a 32-bit `unsigned`.

   ```text
   unsigned has 18 bits
   check value: 237614
   ```

2. I inspected `position()` and `add_data()`. The rolling hash computed by `position()` was used directly as an index into `document`, which only has 69 elements. The size of `unsigned` affects the hash overflow behavior and can therefore produce an out-of-bounds access.

   ```c
   unsigned position(const char *s)
   {
       unsigned h = 0;
       while (*s) {
           h = h * 67 + *s++;
       }
       return h;
   }
   ```

3. I worked locally on my Mac rather than in the `cavern` SSH container. I checked that `legacy.c` was available in `Downloads`, opened it in `nano`, and replaced the hash update with an 18-bit mask.

   ```bash
   cd ~/Downloads
   ls -l legacy.c
   nano legacy.c
   ```

   ```c
   h = (h * 67 + *s++) & 0x3FFFF;
   ```

4. I compiled the corrected source and ran it.

   ```bash
   clang -o legacy_fixed legacy.c
   ./legacy_fixed
   ```

5. The check value matched the author's diagnostics even though `unsigned` remained 32 bits on my Mac. The decrypted document contained the flag.

   ```text
   == Diagnostics: ==
    unsigned has 32 bits
    check value: 237614
   == End of Diagnostics ==

   Attempting to decrypt the document...
   == Decrypted Document: ==
   cctf{thE_G0oD_oLd_Days_wiTh_90Od_old_c}
   == End of Decrypted Document ==
   ```

## Подробный разбор

В задании `legacy` мне дали исходный файл `legacy.c` и диагностику от разработчика, который запускал программу на старой машине. Нужно было исправить программу так, чтобы она правильно работала на современном компьютере, и извлечь флаг из расшифрованного документа. Я выполняла это задание локально через Terminal на macOS.

### 1. Я прочитала условие и проверила исходный файл

```bash
cd ~/Downloads
ls -l legacy.c
```

В сообщении задания разработчик указал `unsigned has 18 bits` и `check value: 237614`. На моей машине тип `unsigned` использует 32 бита. Поскольку в C арифметика беззнаковых целых выполняется по модулю `2^N`, где `N` — разрядность типа, одно и то же выражение может дать разный результат на двух машинах с различным размером `unsigned`.

Сначала я случайно открыла пустой `legacy.c` в контейнере от предыдущего задания `cavern`. После проверки `pwd`, `ls -la` и `find /home /tmp -name 'legacy*'` выяснилось, что это другой контейнер. Поэтому я вернулась в Terminal на Mac и работала с файлом, скачанным из Telegram.

### 2. Я нашла зависимость от разрядности `unsigned`

```c
unsigned position(const char *s)
{
    unsigned h = 0;
    while (*s) {
        h = h * 67 + *s++;
    }
    return h;
}
```

Функция `position()` последовательно обрабатывает символы строки и вычисляет хеш. В `add_data()` результат используется как индекс:

```c
unsigned pos = position(data);
document[pos] = *data;
```

При этом внутри `decrypt_document()` выделяется всего 69 байтов: `char document[69] = {0};`. Если хеш не соответствует индексам, рассчитанным для старой 18-битной архитектуры, запись может выйти за границы массива. Это объясняет падение программы с `segmentation fault`.

### 3. Я имитировала 18-битную арифметику

```bash
nano legacy.c
```

Я заменила исходную строку вычисления хеша на:

```c
h = (h * 67 + *s++) & 0x3FFFF;
```

Маска `0x3FFFF` равна `2^18 - 1`, то есть в двоичном виде содержит 18 единиц. Операция `&` оставляет только младшие 18 бит результата. Таким образом, промежуточный хеш ведёт себя как 18-битное беззнаковое число, хотя тип `unsigned` в самой программе по-прежнему 32-битный.

Я сохранила изменения в `nano` сочетанием `Ctrl + O`, `Enter`, затем вышла через `Ctrl + X`.

### 4. Я собрала и запустила программу

```bash
clang -o legacy_fixed legacy.c
./legacy_fixed
```

`clang` — компилятор C, доступный на macOS. Аргумент `-o legacy_fixed` задаёт имя созданного исполняемого файла. После запуска я получила полную диагностику и восстановленный документ:

```text
== Diagnostics: ==
 unsigned has 32 bits
 check value: 237614
== End of Diagnostics ==

Attempting to decrypt the document...
== Decrypted Document: ==
cctf{thE_G0oD_oLd_Days_wiTh_90Od_old_c}
== End of Decrypted Document ==
```

### 5. Я сверила результат с условием и получила флаг

Главная проверка — число `237614`: оно точно совпало с диагностикой старого разработчика. Строка `unsigned has 32 bits` при этом осталась прежней, потому что я не меняла архитектуру или тип, а только исправила вычисление. Флаг появился в выводе `decrypt_document()`, после чего я отправила его в CTF-бот и задание было учтено в общем результате.
