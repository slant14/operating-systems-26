# dogctf - 02ShyDog

**Flag:** picture file (`IMG_Maybe_Dog_aacf8580.png`).

## Task

`~/go_to_opt/Videos/Shy dog/` contains ~1000 photo-like files with random
extensions (`png/jpeg/bmp/gif/img/tiff/data`) and random names
(`*_archive_*`, `*_Maybe_Dog_*`). The real dog picture hides among them.



`file *` classifies every file by actual content: 946 are just `data`, others
are random-magic decoys (`OpenPGP Secret Key`, `DOS executable`, ...) - and
**exactly one** file is a real `PNG image data`:

```sh
cd ~/go_to_opt/Videos/'Shy dog'
file * | grep 'PNG image data' | cut -d: -f1
# IMG_Maybe_Dog_aacf8580.png
```
