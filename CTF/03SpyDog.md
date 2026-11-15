# 03SpyDog

**Category** dogctf

## Solution

1. I read the instructions and suspects list in the `cat_tracker` repository. I found references to a classified `cat_cam/` directory and a leaked archive, while the directory was absent from the working tree.

   ```bash
   cat /home/dog/Doguments/cat_tracker/README.md
   cat /home/dog/Doguments/cat_tracker/suspects.txt
   ```

2. I inspected all referenced history and the tracked paths. I found commits about the leak, history rewriting, and removing the frames.

   ```bash
   tracker_repo=/home/dog/Doguments/cat_tracker
   git --no-pager -c safe.directory="$tracker_repo" -C "$tracker_repo" log --all --oneline --decorate -n 30
   git -c safe.directory="$tracker_repo" -C "$tracker_repo" ls-files
   ```

3. I inspected the relevant commits and frame script, then searched the object database for unreachable objects. I found four unreachable commits and two unreachable blobs.

   ```bash
   git --no-pager -c safe.directory="$tracker_repo" -C "$tracker_repo" show --stat --oneline 44a6a20 f69db06 f0a73ef fef6f93
   cat "$tracker_repo/tools/frame_grab.sh"
   git -c safe.directory="$tracker_repo" -C "$tracker_repo" fsck --full --no-reflogs --unreachable
   ```

4. I inspected the old commits and extracted both blobs to determine their formats. I found a 149,915-byte frame in commit `1b6e649` and a 256-byte frame labeled as corrupted in `cff89c1`. I identified blob `d4a9847` as a 300 × 225 RGBA PNG and blob `5bf31a0` as generic data.

   ```bash
   git --no-pager -c safe.directory="$tracker_repo" -C "$tracker_repo" show --stat --oneline 1b6e649 2d822e8 cff89c1 f6ceb8d
   git -c safe.directory="$tracker_repo" -C "$tracker_repo" cat-file blob 5bf31a049306e8154e4c63a05b55ddeeb1240a47 > /tmp/spy_blob_5bf31a0
   git -c safe.directory="$tracker_repo" -C "$tracker_repo" cat-file blob d4a984796be044e06a5972eca853c09624fc3094 > /tmp/spy_blob_d4a9847
   file -- /tmp/spy_blob_5bf31a0 /tmp/spy_blob_d4a9847
   ```

5. I recovered the image by its original path in the old commit.

   ```bash
   git -c safe.directory="$tracker_repo" -C "$tracker_repo" show 1b6e649:cat_cam/frame_0051.png > /tmp/Spy_dog.png
   file -- /tmp/Spy_dog.png
   ```

6. I downloaded and opened the recovered image on my Mac.

   ```bash
   scp a.kaiumova-dog@osctf.nnk.moe:/tmp/Spy_dog.png ~/Downloads/Spy_dog.png
   open ~/Downloads/Spy_dog.png
   ```

7. I submitted `Spy_dog.png` and received `Flag accepted for '03SpyDog'.`

## Подробный разбор

Мне нужно было найти картинку для `03SpyDog`. Я исследовала Git-репозиторий программы слежения за котами, обнаружила объекты после переписывания истории и восстановила удалённый кадр камеры.

### 1. Я прочитала подсказки в репозитории

```bash
cat /home/dog/Doguments/cat_tracker/README.md
cat /home/dog/Doguments/cat_tracker/suspects.txt
```

В README я прочитала описание WhiskerWatch, программы слежения за котами. Среди компонентов я увидела `cat_cam/`, помеченный как секретный. При предыдущем просмотре папки этого каталога я не обнаружила. В `suspects.txt` я также нашла заметку с просьбой повторно не загружать архив камеры.

По этим подсказкам я решила проверить историю Git, где могли сохраниться следы архива или удалённых кадров.

### 2. Я изучила текущую историю и список файлов

```bash
tracker_repo=/home/dog/Doguments/cat_tracker
git --no-pager -c safe.directory="$tracker_repo" -C "$tracker_repo" log --all --oneline --decorate -n 30
git -c safe.directory="$tracker_repo" -C "$tracker_repo" ls-files
```

В `tracker_repo` я сохранила путь к репозиторию. Параметром `-C` я задавала каталог для команд Git. Поскольку владельцем репозитория был `dog`, а я работала как `ctf`, я использовала `-c safe.directory="$tracker_repo"` для разрешения работы с этим известным учебным репозиторием в конкретном вызове.

Через `--no-pager` я выводила результаты прямо в терминал. В `log` я использовала `--all` для просмотра истории от всех доступных ссылок, `--oneline` для кратких записей и `--decorate` для отображения названий веток.

Я увидела следующие сообщения коммитов.

- `44a6a20` — сообщение об утечке архива камеры.
- `f69db06` — заявление о переписывании истории и удалении кадров из всех коммитов.
- `f0a73ef` — повторная проверка отсутствия кадров.
- `fef6f93` — сообщение с надеждой, что кадры действительно удалены.

В списке `ls-files` я нашла скрипт `tools/frame_grab.sh`, но кадров `cat_cam/` среди текущих отслеживаемых файлов не было.

### 3. Я нашла недостижимые объекты Git

```bash
git --no-pager -c safe.directory="$tracker_repo" -C "$tracker_repo" show --stat --oneline 44a6a20 f69db06 f0a73ef fef6f93
cat "$tracker_repo/tools/frame_grab.sh"
git -c safe.directory="$tracker_repo" -C "$tracker_repo" fsck --full --no-reflogs --unreachable
```

Через `show --stat` я посмотрела состав изменений. В скрипте захвата я увидела, что кадры сохраняются в `cat_cam/`, а номер в имени файла форматируется четырьмя цифрами через `printf '%04d'`.

Затем я проверила базу объектов командой `fsck`. Параметром `--full` я включила полную проверку объектов, через `--unreachable` запросила недостижимые объекты, а через `--no-reflogs` исключила журналы перемещений ссылок из точек отсчёта достижимости.

Я получила четыре недостижимых коммита.

```text
1b6e649bda907eb2b6aa3ba626d47db28cb10599
2d822e81e9650e206dd8efb47814e01030dc4a84
cff89c1d7e35f3a8f6ff39e2fa1d349700e2e577
f6ceb8df095432347c8c78c9bd9aa6915a15325c
```

Я также получила два недостижимых blob-объекта.

```text
5bf31a049306e8154e4c63a05b55ddeeb1240a47
d4a984796be044e06a5972eca853c09624fc3094
```

Я разобралась, что коммит ссылается на дерево каталогов, а дерево связывает имена файлов с объектами содержимого — blob. После переписывания истории старые коммиты и файлы могли остаться в базе объектов до её очистки. Обычный просмотр истории от текущих ссылок уже не показывал эти версии, но `fsck` позволил мне обнаружить их по идентификаторам.

### 4. Я отличила подходящий кадр от повреждённого

```bash
git --no-pager -c safe.directory="$tracker_repo" -C "$tracker_repo" show --stat --oneline 1b6e649 2d822e8 cff89c1 f6ceb8d
git -c safe.directory="$tracker_repo" -C "$tracker_repo" cat-file blob 5bf31a049306e8154e4c63a05b55ddeeb1240a47 > /tmp/spy_blob_5bf31a0
git -c safe.directory="$tracker_repo" -C "$tracker_repo" cat-file blob d4a984796be044e06a5972eca853c09624fc3094 > /tmp/spy_blob_d4a9847
file -- /tmp/spy_blob_5bf31a0 /tmp/spy_blob_d4a9847
```

В коммите `1b6e649` я нашла добавление `cat_cam/frame_0051.png` размером 149 915 байт. В `cff89c1` я увидела `cat_cam/frame_0050.png` размером 256 байт и сообщение о повреждённом кадре.

Через `cat-file blob` я извлекла исходные байты двух объектов. Перенаправлением `>` я сохранила их в отдельные файлы в `/tmp`. Затем я проверила формат с помощью `file`.

Для `spy_blob_5bf31a0` я получила `data`, а для `spy_blob_d4a9847` — `PNG image data, 300 x 225, 8-bit/color RGBA, non-interlaced`. По совокупности размера, сообщения коммита и распознанного формата я выбрала кадр `0051` для восстановления и проверки у бота.

### 5. Я восстановила картинку из конкретного коммита

```bash
git -c safe.directory="$tracker_repo" -C "$tracker_repo" show 1b6e649:cat_cam/frame_0051.png > /tmp/Spy_dog.png
file -- /tmp/Spy_dog.png
```

В выражении `1b6e649:cat_cam/frame_0051.png` я указала две части. Слева от двоеточия находился идентификатор старого коммита `1b6e649`, справа — путь `cat_cam/frame_0051.png` в дереве этого коммита. Командой `git show` я прочитала содержимое файла из этой сохранённой версии.

Перенаправлением `>` я записала полученные байты в `/tmp/Spy_dog.png`. Так я восстановила изображение из объекта старой истории, хотя в текущей рабочей папке каталога `cat_cam/` уже не было.

### 6. Я скачала и открыла результат

В отдельной вкладке терминала на Mac я выполнила следующие команды.

```bash
scp a.kaiumova-dog@osctf.nnk.moe:/tmp/Spy_dog.png ~/Downloads/Spy_dog.png
open ~/Downloads/Spy_dog.png
```

Первым аргументом `scp` я указала восстановленный файл внутри контейнера, вторым — путь назначения на Mac. Затем я открыла локальную картинку через `open`.

### 7. Я подтвердила решение

Я отправила `Spy_dog.png` из папки `Downloads` боту и получила `Flag accepted for '03SpyDog'.` По этому сообщению я подтвердила, что восстановила нужный кадр. Локальная копия имела размер 149 915 байт и формат PNG размером 300 × 225 с цветом RGBA.
