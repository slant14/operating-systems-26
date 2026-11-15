# 02PetDog

**Category** dogctf

## Solution

1. I inspected the AutoPetter repository and read its build instructions.

   ```bash
   cd /home/ctf/go_to_opt/Doguments/auto_petter
   cat README.md
   cat autopet.sh
   cat tools/assemble_mascot.sh
   ls -lh frames
   git status --short --branch
   git log --all --graph --oneline --decorate -n 30
   ```

   I found an interrupted rebase of `dev` onto `master` at `b442a21`. I also found that the mascot assembler concatenated `part2.bin`, `part1.bin`, and `part3.bin` in that order.

2. I inspected the rebase state, the conflicting configuration, and the frame history.

   ```bash
   git status
   cat config/petting.cfg
   cat .git/rebase-merge/git-rebase-todo
   file frames/part1.bin frames/part2.bin frames/part3.bin
   git --no-pager show master:config/petting.cfg
   git --no-pager show 45800cb -- config/petting.cfg
   git --no-pager show 6652519 -- frames/part1.bin
   git show 6652519^:frames/part1.bin | wc -c
   ```

   I identified the final `part2` already on `master`, the final `part3` in `9eb78a0`, and the valid 38,152-byte `part1` before quarantine commit `6652519`.

3. I resolved `config/petting.cfg`, keeping the intensity limits from `master`, adding the turbo settings, and preserving the 45-minute session setting. I edited the remaining rebase plan to drop `fb29708` and `03845ff`, the two experimental frame changes. I kept the final `part3` and adaptive cadence commits.

   ```bash
   nano config/petting.cfg
   nano .git/rebase-merge/git-rebase-todo
   git add config/petting.cfg
   GIT_EDITOR=true git rebase --continue
   ```

4. I resolved the binary conflict in `part3.bin` by selecting the final design encode, then continued the rebase.

   ```bash
   git restore --source=9eb78a0 --staged --worktree -- frames/part3.bin
   GIT_EDITOR=true git rebase --continue
   ```

5. I resolved `src/petting_core.sh` by combining the adaptive stroke count and pause with the tail safety check. I checked shell syntax and completed the rebase.

   ```bash
   nano src/petting_core.sh
   sh -n src/petting_core.sh &&
   git add src/petting_core.sh &&
   GIT_EDITOR=true git rebase --continue
   git status
   ```

6. I restored the valid `part1` from before quarantine and committed the repair.

   ```bash
   git restore --source=6652519^ -- frames/part1.bin
   wc -c frames/part1.bin
   git add frames/part1.bin
   git commit -m "mascot: restore final part1 encode"
   ```

7. I advanced `master` to the rebased `dev` history with a fast-forward and rebuilt the mascot. I obtained a 114,456-byte PNG, identified as a 300 × 200 RGBA image.

   ```bash
   git switch master &&
   git merge --ff-only dev &&
   ./tools/assemble_mascot.sh
   file mascot.png
   git status
   ```

8. I downloaded and opened `mascot.png` on my Mac, using the challenge SSH destination in place of `USER@HOST`. I submitted the image and received `Flag accepted for '02PetDog'.`

   ```bash
   scp USER@HOST:/home/ctf/go_to_opt/Doguments/auto_petter/mascot.png ~/Downloads/mascot.png
   open ~/Downloads/mascot.png
   ```

## Подробный разбор

Мне нужно было собрать картинку для `02PetDog` из файлов Git-репозитория. Я обнаружила незавершённый rebase, разрешила три конфликта, выбрала подходящие версии частей картинки и перенесла готовую историю в `master`.

### 1. Я прочитала инструкцию и выяснила способ сборки

В `Doguments/auto_petter` я нашла `.git`, `README.md`, `autopet.sh` и несколько каталогов. Я перешла в репозиторий и прочитала инструкцию и скрипты.

```bash
cd /home/ctf/go_to_opt/Doguments/auto_petter
cat README.md
cat autopet.sh
cat tools/assemble_mascot.sh
ls -lh frames
git status --short --branch
git log --all --graph --oneline --decorate -n 30
```

В README я прочитала, что изменения из рабочих веток должны переноситься на `master` через rebase, а завершение работы проверяется сборкой `mascot.png`. Я также нашла указание использовать финальные версии частей и смотреть историю Git.

В `assemble_mascot.sh` я увидела порядок сборки.

```bash
cat frames/part2.bin frames/part1.bin frames/part3.bin > mascot.png
```

Я определила, что части нужно соединять именно в порядке `part2`, `part1`, `part3`. Скрипт затем проверял первые четыре байта на значение `89504e47` — начало сигнатуры PNG. Я учла, что такая проверка подтверждает только начало файла и может пропустить повреждения в следующих частях.

### 2. Я разобралась в состоянии Git и версиях частей

```bash
git status
cat config/petting.cfg
cat .git/rebase-merge/git-rebase-todo
file frames/part1.bin frames/part2.bin frames/part3.bin
git --no-pager show master:config/petting.cfg
git --no-pager show 45800cb -- config/petting.cfg
git --no-pager show 6652519 -- frames/part1.bin
git show 6652519^:frames/part1.bin | wc -c
```

Из `git status` я узнала, что уже выполнялся интерактивный rebase ветки `dev` на коммит `b442a21`, где находился `master`. Операция остановилась на конфликте конфигурации при применении коммита `45800cb` с turbo-режимом. Метка `UU` показывала неразрешённый конфликт. Состояние detached HEAD было частью процесса переноса коммитов.

В `git-rebase-todo` я увидела оставшиеся действия. В истории и сообщениях коммитов я нашла следующие зацепки.

- В `master` уже находилась повторно экспортированная `part2` из коммита `e4793e6`.
- В коммите `9eb78a0` находилась финальная версия `part3`. В `master` эту часть ранее заменили сжатой версией.
- В `fb29708` была экспериментальная обработка `part2`, описанная как портящая результат.
- В `03845ff` была экспериментальная увеличенная `part1` с дефектами.
- Коммит `6652519` заменил `part1` при карантине. До него эта часть имела размер 38 152 байта, а в текущем состоянии — 88 байт.

Через `git show КОММИТ:путь` я читала сохранённый в Git файл. Запись `6652519^` обозначала родителя коммита карантина, поэтому я смогла проверить версию непосредственно перед заменой. Вариант `--no-pager` позволил мне получить вывод прямо в терминале без постраничного просмотрщика.

### 3. Я разрешила конфликт конфигурации и убрала экспериментальные коммиты

Я открыла конфигурацию в Nano.

```bash
nano config/petting.cfg
```

Я сохранила ограничения интенсивности из `master`, добавила секцию turbo и сохранила уже применённое увеличение длительности сеанса. Я удалила маркеры конфликта и дублирующиеся значения. Итоговое содержимое конфигурации соответствовало следующему.

```ini
# petting.cfg - AutoPetter 9000 configuration
[intensity]
default = 2
max_safe = 5
hard_clamp = true

[turbo]
enabled = true
wind_up_sec = 2

[tail_sensor]
poll_interval_ms = 50
debounce = false

[session]
max_duration_min = 45
cooldown_min = 5
```

Я сохраняла файл в Nano через Ctrl+O и Enter, затем выходила через Ctrl+X. Следующим действием я открыла план оставшейся части rebase.

```bash
nano .git/rebase-merge/git-rebase-todo
```

В двух экспериментальных коммитах я заменила действие `pick` на `drop`.

```text
drop fb29708033f9060b201ae70152f230682404c1cf wip: new dithering filter on part2 (mangles the output)
drop 03845ffb8687b8cba9f8ecd8b2a581ba27ef3b7f wip: upscale part1 for the retina mockup (ringing everywhere, parking this)
```

Так я исключила эти изменения из переносимой истории. Для финальной `part3` и адаптивного ритма я сохранила `pick`, а последней командой оставила запуск сборщика. Затем я отметила конфигурацию как разрешённую и продолжила операцию.

```bash
git add config/petting.cfg
GIT_EDITOR=true git rebase --continue
```

Через `git add` я записала выбранное содержимое в индекс и отметила разрешение конфликта. Переменной `GIT_EDITOR=true` я позволила Git использовать готовое сообщение коммита без открытия редактора. Я получила новый коммит `91cbe5a` с turbo-режимом.

### 4. Я разрешила конфликт бинарной части `part3`

При применении `9eb78a0` я получила конфликт в `frames/part3.bin`. Для этой части я выбрала финальный encode из указанного коммита.

```bash
git restore --source=9eb78a0 --staged --worktree -- frames/part3.bin
GIT_EDITOR=true git rebase --continue
```

С помощью `--source` я явно указала версию. Параметрами `--staged --worktree` я восстановила её одновременно в индексе и рабочем файле, тем самым разрешив конфликт. Я получила новый коммит `9de3345` с финальной частью.

### 5. Я объединила адаптивный ритм с проверкой хвоста

Следующий конфликт возник в `src/petting_core.sh` при применении `06c5a9d`. Я прочитала конфликтующий файл, версию `master` и изменения коммита.

```bash
cat src/petting_core.sh
git --no-pager show master:src/petting_core.sh
git --no-pager show 06c5a9d -- src/petting_core.sh
nano src/petting_core.sh
```

В `master` я нашла проверку хвоста перед каждым поглаживанием. В `dev` я нашла вычисление числа поглаживаний через интенсивность и паузу `sleep 0.2`. Я объединила эти изменения. Тело функции `pet_loop()` получилось следующим.

```sh
pet_loop() {
    target="$1"
    intensity="$2"
    strokes=0
    max_strokes=$((intensity * 40))
    while [ "$strokes" -lt "$max_strokes" ]; do
        check_tail "$target" || break
        pet_once "$target" "$intensity"
        strokes=$((strokes + 1))
        sleep 0.2
    done
}
```

Я сохранила функции `pet_once`, `check_tail` и `read_tail`, включая два чтения датчика в `check_tail`. После редактирования я проверила синтаксис и продолжила rebase.

```bash
sh -n src/petting_core.sh &&
git add src/petting_core.sh &&
GIT_EDITOR=true git rebase --continue
git status
```

Командой `sh -n` я проверила синтаксис без выполнения скрипта. Через `&&` я связала действия так, чтобы переходить к следующему после успешного завершения предыдущего. Я получила коммит `b66a345`, выполнение завершающей команды сборки и сообщение `Successfully rebased and updated refs/heads/dev.`

Первая сборка содержала 76 392 байта. Сборщик сообщил `OK`, но я знала, что `part1` всё ещё состоит из 88 байт после карантина. Поэтому я продолжила восстановление частей.

### 6. Я восстановила полноценную `part1`

```bash
git restore --source=6652519^ -- frames/part1.bin
wc -c frames/part1.bin
git add frames/part1.bin
git commit -m "mascot: restore final part1 encode"
```

Я получила версию до карантина, проверила размер 38 152 байта и сохранила восстановление отдельным коммитом `31b6cd3`. Так я сохранила исправленную `part2` из `master`, финальную `part3` и полноценную исходную `part1`.

### 7. Я перенесла готовую историю в `master` и пересобрала картинку

```bash
git switch master &&
git merge --ff-only dev &&
./tools/assemble_mascot.sh
file mascot.png
git status
```

После rebase история `dev` продолжала историю `master`. Через `git merge --ff-only dev` я передвинула указатель `master` с `b442a21` на `31b6cd3`, получив fast-forward без merge-коммита. Это соответствовало описанному в README порядку переноса изменений.

Сборщик соединил три части и вывел `mascot.png assembled OK (114456 bytes)`. Через `file` я получила `PNG image data, 300 x 200, 8-bit/color RGBA, non-interlaced`.

В `git status` я увидела ветку `master` и новый неотслеживаемый файл `mascot.png`. Я различала сохранённые в коммитах исходные части и созданный из них результат сборки. Для отправки в задании я использовала полученный файл.

### 8. Я скачала картинку и подтвердила флаг

В отдельной вкладке терминала на Mac я выполнила следующие команды, подставив вместо `USER@HOST` SSH-адрес из задания.

```bash
scp USER@HOST:/home/ctf/go_to_opt/Doguments/auto_petter/mascot.png ~/Downloads/mascot.png
open ~/Downloads/mascot.png
```

Я скачала картинку в `Downloads`, открыла её и отправила боту. В ответ я получила `Flag accepted for '02PetDog'.` По этому сообщению я подтвердила правильность собранного изображения.
