# 03InsecureDog

**Category** dogctf

## Solution

1. I inspected `secret_stuff` and read `remember.txt`. I found a hint that the image in `super_secret` had a four-letter filename made from `D`, `O`, and `G`, with one letter repeated.

   ```bash
   ls -la /home/ctf/go_to_opt/Doguments/secret_stuff
   cat /home/ctf/go_to_opt/Doguments/secret_stuff/remember.txt
   ```

   I observed the directory permissions `d-wx--x--x` on `super_secret`, allowing traversal while restricting directory listing.

2. I generated all 81 four-letter combinations and tested each exact path for readability.

   ```bash
   cd /home/ctf/go_to_opt/Doguments/secret_stuff
   for dog_name in {D,O,G}{D,O,G}{D,O,G}{D,O,G}; do
       if test -r "super_secret/$dog_name.png"; then
           printf '%s\n' "$PWD/super_secret/$dog_name.png"
       fi
   done
   ```

3. I found the readable file `OGOD.png`, which matched the hint.

   ```text
   /home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/OGOD.png
   ```

4. I downloaded and opened the image on my Mac.

   ```bash
   scp a.kaiumova-dog@osctf.nnk.moe:/home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/OGOD.png ~/Downloads/OGOD.png
   open ~/Downloads/OGOD.png
   ```

5. I submitted `OGOD.png` and received `Flag accepted for '03InsecureDog'.`

## Подробный разбор

Мне нужно было найти картинку в каталоге с ограниченными правами. Я прочитала подсказку об имени, перебрала короткий набор вариантов и получила доступ к файлу по точному пути.

### 1. Я прочитала подсказку и разобралась в правах каталога

```bash
ls -la /home/ctf/go_to_opt/Doguments/secret_stuff
cat /home/ctf/go_to_opt/Doguments/secret_stuff/remember.txt
```

В `remember.txt` я прочитала, что картинка находится в папке `super_secret`. Имя состояло из четырёх букв, использовало `D`, `O`, `G`, и одна из букв встречалась дважды. В примерах я увидела расширение `.png`.

У каталога `super_secret` я увидела права `d-wx--x--x`. Я была владельцем `ctf`, поэтому для меня действовала группа прав `-wx`. Я могла проходить через каталог благодаря `x`, но не могла читать список имён без права `r`. Для чтения файла внутри также требовались права на сам файл.

По этим условиям я решила проверить известные варианты полного имени. Такой способ использовал доступ по точному пути и не требовал перечисления содержимого `super_secret`.

### 2. Я перебрала короткие имена

Внутри SSH-сессии я выполнила цикл.

```bash
cd /home/ctf/go_to_opt/Doguments/secret_stuff
for dog_name in {D,O,G}{D,O,G}{D,O,G}{D,O,G}; do
    if test -r "super_secret/$dog_name.png"; then
        printf '%s\n' "$PWD/super_secret/$dog_name.png"
    fi
done
```

Через раскрытие фигурных скобок Bash я сгенерировала варианты четырёх букв. Для каждой позиции было три возможных значения, поэтому получилось `3 × 3 × 3 × 3 = 81` имя.

Я выбрала полный небольшой набор из 81 комбинации. Он включал варианты с повторениями, выходящими за ограничения подсказки, и гарантированно охватывал все подходящие имена. Элементы этого набора оболочка создала из явно заданных букв.

Переменная `dog_name` последовательно принимала каждое имя. Через `test -r` я проверяла, доступен ли файл `super_secret/$dog_name.png` для чтения. При успешной проверке я выводила абсолютный путь с помощью `printf`. В `$PWD` находился текущий каталог.

### 3. Я получила точный путь к картинке

Цикл вывел одну строку.

```text
/home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/OGOD.png
```

Я проверила соответствие подсказке. В `OGOD` четыре буквы, присутствуют `O`, `G`, `D`, а `O` встречается дважды. Так я нашла подходящее имя без чтения списка каталога.

### 4. Я скачала и открыла файл

Я открыла отдельную вкладку терминала на Mac и выполнила следующие команды.

```bash
scp a.kaiumova-dog@osctf.nnk.moe:/home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/OGOD.png ~/Downloads/OGOD.png
open ~/Downloads/OGOD.png
```

Первым аргументом `scp` я указала точный источник в контейнере, вторым — путь назначения на Mac. Затем я открыла локальную копию командой `open`.

### 5. Я подтвердила решение

Я отправила `OGOD.png` из папки `Downloads` боту и получила `Flag accepted for '03InsecureDog'.` По этому сообщению я подтвердила, что нашла картинку для нужного задания.
