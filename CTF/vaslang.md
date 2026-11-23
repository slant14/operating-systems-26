# vaslang

**Category** cctf

## Solution

1. I downloaded `vaslang.c` and `inp.vas` into `~/Downloads`. I compiled the original C interpreter and ran the supplied Vaslang input to reproduce the problem.

   ```bash
   cd ~/Downloads
   ls -lh vaslang.c inp.vas
   clang -o vaslang vaslang.c
   ./vaslang < inp.vas > output.txt
   head -n 15 output.txt
   ```

2. The interpreter printed multiple `ERROR: func ... undefined` messages, for example `hopeful_893`. I inspected the end of the large input file and found a `main!` command. The interpreter resolved function calls too early, before all function definitions had been read.

   ```bash
   tail -n 3 inp.vas
   ```

   ```text
   ERROR: func hopeful_893 undefined
   ERROR: func agitated_114 undefined
   ...
   main!
   ```

3. I backed up the interpreter and prepared a Python patch script to defer name resolution. The patch saved each unresolved call and its name, then resolved the references against the complete function table before executing a command.

   ```bash
   cp vaslang.c vaslang_original.c
   nano fix.py
   python3 fix.py
   ```

4. The patch introduced a list of fixups. For every function call encountered while parsing a definition, it stored the call site and referenced name instead of immediately requiring a matching definition.

   ```c
   struct fixup {
       struct term *call;
       char *name;
   };

   struct fixup fixups[1048576];
   size_t fixup_count = 0;
   ```

5. I added `resolve_calls()` to look up each referenced function in `funcs` and update `call->target`. I also fixed `xdef()` so that `funcp` advances only for a new definition, rather than for a redefinition of an existing function.

   ```c
   /* During definition parsing, for a named call: */
   ftermp->ty = 'C';
   fixups[fixup_count].call = ftermp;
   fixups[fixup_count].name = strdup(term);
   fixup_count++;

   /* When finishing a definition: */
   if (is_new) funcp++;
   termp = ftermp;
   ```

6. I recompiled the patched interpreter and executed the input again. This time it produced the flag, mixed into a long line of `>` prompts. I used `grep -o` to extract just the matching text.

   ```bash
   clang -O2 -o vaslang_fixed vaslang.c
   ./vaslang_fixed < inp.vas > output_fixed.txt
   tail -n 10 output_fixed.txt
   grep -o 'cctf{[^}]*}' output_fixed.txt
   ```

   ```text
   cctf{you_CouLd_HaVE_SO1VEd_tHi5_Th3_fUn_waY}
   ```

## Подробный разбор

В задании `vaslang` мне дали написанный на C интерпретатор нового языка и большой входной файл `inp.vas`. Нужно было исправить ошибки в реализации интерпретатора, чтобы он смог выполнить программу и напечатать флаг. Я работала на macOS, запускала исходный код через `clang` и сохраняла результаты в текстовые файлы.

### 1. Я проверила входные файлы и воспроизвела проблему

```bash
cd ~/Downloads
ls -lh vaslang.c inp.vas
clang -o vaslang vaslang.c
./vaslang < inp.vas > output.txt
head -n 15 output.txt
```

`ls -lh` показала, что `vaslang.c` занимает около 3,4 КБ, а `inp.vas` — около 1,1 МБ. Исходный интерпретатор компилировался без ошибок, но при выполнении входного файла выводил сообщения вроде:

```text
hello
> ERROR: func hopeful_893 undefined
> ERROR: func agitated_114 undefined
> ERROR: func serene_690 undefined
```

Оператор `< inp.vas` передаёт содержимое файла на стандартный ввод программы, а `> output.txt` сохраняет стандартный вывод в файл. Команда `head` позволяет посмотреть первые строки и не выводить весь огромный результат в терминал.

### 2. Я разобралась, как работает интерпретатор

```bash
tail -n 3 inp.vas
```

Последней строкой была команда `main!`. В `vaslang.c` я увидела, что строки вида `name: body` определяют функцию, а `name!` запускает её. Для вычислений интерпретатор использует стек и простые инструкции: `+` складывает числа по модулю 251, `*` умножает их по модулю 251, `:` дублирует значение, а `.` удаляет верхний элемент стека.

При чтении определения функция `xdef()` сразу пыталась найти каждую упоминаемую функцию в уже зарегистрированном списке. Но `inp.vas` содержит обращения к функциям, которые определены ниже. Поэтому появлялась ошибка `func ... undefined`, хотя проблема состояла не обязательно в отсутствии функции, а в порядке обработки определений.

### 3. Я сохранила исходник и подготовила исправление

```bash
cp vaslang.c vaslang_original.c
nano fix.py
python3 fix.py
```

Командой `cp` я создала резервную копию перед изменениями. В `fix.py` я использовала Python для изменения конкретных фрагментов исходного C-кода, чтобы не редактировать вручную несколько мест. Скрипт добавил структуру `fixup` и массив записей о вызовах функций:

```c
struct fixup {
    struct term *call;
    char *name;
};

struct fixup fixups[1048576];
size_t fixup_count = 0;
```

Каждая запись хранит указатель на терм вызова и строку с названием функции, которая должна быть найдена позже. Так можно сначала разобрать все определения, а затем установить адреса их тел.

### 4. Я перенесла разрешение имён на момент выполнения

При разборе вызова по имени новый код сохранял ссылку и имя:

```c
ftermp->ty = 'C';
fixups[fixup_count].call = ftermp;
fixups[fixup_count].name = strdup(term);
fixup_count++;
```

Функция `resolve_calls()` проходила по этим записям, сравнивала имена с `funcs[i].name` через `strcmp()`, находила тело нужной функции и записывала его адрес в `call->target`. Перед запуском команды `xcmd()` вызывала `resolve_calls()`, чтобы исполнения не происходило с неразрешёнными ссылками.

По смыслу это похоже на позднее связывание символов: сначала собираются определения, затем ссылки сопоставляются с адресами. Мы не меняли сам набор инструкций языка, а исправили момент, когда интерпретатор ищет целевые функции.

### 5. Я исправила счётчик определений при переопределении функции

В исходной версии `xdef()` всегда делала `funcp++`, даже если находила функцию с таким именем и перезаписывала её запись. Я добавила признак `is_new`: он равен 1 для нового имени и становится 0, если имя уже существует.

```c
/* At the start of xdef(): */
struct func *func = funcp;
int is_new = 1;

/* When an existing name is found: */
if (strcmp(funcs[i].name, name) == 0) {
    func = funcs + i;
    is_new = 0;
}

/* At the end: */
if (is_new) funcp++;
termp = ftermp;
```

Теперь повторное определение не увеличивает число зарегистрированных имён и не создаёт лишнюю запись в таблице. Вместе с отложенным разрешением ссылок это позволило запустить входной файл.

### 6. Я повторно скомпилировала программу и нашла флаг

```bash
clang -O2 -o vaslang_fixed vaslang.c
./vaslang_fixed < inp.vas > output_fixed.txt
tail -n 10 output_fixed.txt
grep -o 'cctf{[^}]*}' output_fixed.txt
```

Опция `-O2` включает оптимизацию компилятора. В результате выполнения интерпретатор напечатал много символов `>` — это приглашения ввода внутри цикла `main()`. Поэтому вместо просмотра всей строки я использовала `grep -o`: флаг `-o` выводит только совпавший с шаблоном фрагмент. Выражение `cctf{[^}]*}` ищет строку, начинающуюся с `cctf{` и заканчивающуюся первой `}`.

```text
cctf{you_CouLd_HaVE_SO1VEd_tHi5_Th3_fUn_waY}
```

Флаг был получен из вывода исправленного интерпретатора и принят в рамках CTF.
