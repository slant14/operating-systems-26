# `findingdory`: recovered images

This report records findings in the shared container `o.shefner-dog@osctf.nnk.moe`. The images are saved alongside the report. Bot confirmations change as files are submitted; this document records how the PNGs were obtained.

| Challenge | Commands and source | File |
|---|---|---|
| `01CatDog` | Two halves in `/home/ctf/go_to_opt/Pictures/Cat_dog_top` and `Cat_dog_bottom`; concatenated in that order (`cat Cat_dog_top Cat_dog_bottom > Cat_dog.png`) | [`Cat_dog.png`](Cat_dog.png) |
| `01CompileDog` | `gcc /home/ctf/go_to_opt/Programs/compile_dog.c -o /tmp/compile_dog && /tmp/compile_dog`; the program printed the path `/regular_paw_storage/7881aeec9f35f5a0/Compile_dog.png` | [`Compile_dog.png`](Compile_dog.png) |
| `01EditDog` | In `/home/ctf/go_to_opt/Doguments/EDITME`, five lines of the ASCII dog were extended by 9 columns, without changing any text outside the boundary lines. When the file was saved with LF line endings, the container reported `Woof! You have solved Edit_dog!` and the path `/regular_paw_storage/6394577d2b2356c5/Edit_dog.png` | [`Edit_dog.png`](Edit_dog.png) |
| `01HelloDog` | The starting directory `/home/ctf/go_to_opt` contained `hoi_:3.png`; the shell printed `Hiiii :3` on login | [`hoi_3.png`](hoi_3.png) |
| `01HistoryDog` | A command containing `/regular_paw_storage/155b69acc8fb0712/grandpaw.png` was found in `/home/ctf/.bash_history` | [`grandpaw.png`](grandpaw.png) |
| `01PoweredDog` | `/opt/README.md` provided the path `/regular_paw_storage/a6a929afb8ed8396/powered_dog.png` | [`powered_dog.png`](powered_dog.png) |
| `01TailheadedDog` | The beginning of `/home/ctf/go_to_opt/Downloads/dog.log` contained `/regular_paw_storage/de0f94a4`, and the end contained `b59d6d0b/tailheaded.png`; the path fragments were joined | [`tailheaded.png`](tailheaded.png) |
| `02MuteDog` | Running `/home/ctf/go_to_opt/Programs/mute_dog` with stdout and stderr redirected separately revealed the path `/regular_paw_storage/a9b79bbb777e2336/Mute_dog.png` | [`Mute_dog.png`](Mute_dog.png) |
| `02PetDog` | In `/home/ctf/go_to_opt/Doguments/auto_petter`, the `./tools/assemble_mascot.sh` script assembled `mascot.png` from the parts `frames/part1.bin`, `part2.bin`, and `part3.bin` | [`mascot.png`](mascot.png) |
| `02RemDog` | `/home/ctf/go_to_opt/Doguments/unsorted/README` instructed us to delete the evil dogs. The 32 ASCII files fell into two groups of identical images. After making a backup, we deleted the 13 files of 1937 bytes and kept the 19 files of 1678 bytes. `/usr/local/bin/checker_dog check` printed `/regular_paw_storage/e1bb8928678ba2fe/Rem_dog.png` | [`Rem_dog.png`](Rem_dog.png) |
| `03InsecureDog` | `/home/ctf/go_to_opt/Doguments/secret_stuff/remember.txt` described a four-letter name made from `D`, `O`, and `G`, with one letter repeated. Trying candidate names with `stat` in the `super_secret` directory, which could not be listed, located `OGOD.png` | [`OGOD.png`](OGOD.png) |
| `02ScatteredDog` | `Videos/Scattered dog` contained parts `-50` … `49`; `for n in $(seq -50 49); do cat "$n"; done > Scattered_dog.png` | [`Scattered_dog.png`](Scattered_dog.png) |
| `03UsedDog` | A PNG was located at `/home/dog/Used_dog.png` | [`Used_dog.png`](Used_dog.png) |
| `03NetDog` | `/proc/net/tcp` showed a listener on port 4644; `nc 127.0.0.1 4644 > Net_dog.png` saved the PNG | [`Net_dog.png`](Net_dog.png) |
| `03SpyDog` | In `/home/dog/Doguments/cat_tracker`, `git fsck --no-reflogs --unreachable` found the deleted commit `4936471`; `git show 4936471:cat_cam/frame_0044.png > Spy_dog.png` extracted the frame | [`Spy_dog.png`](Spy_dog.png) |
| `03ChainedDog` | `/home/dog/Downloads/start` contained the first `Next:` link. Each subsequent file contained `Next:` and binary `Content:`. We followed the links through 272 files to `end` and concatenated the `Content:` fields into a PNG | [`Chained_dog.png`](Chained_dog.png) |
| `02ShyDog` | We checked the PNG signature among 1000 files in `/home/ctf/go_to_opt/Videos/Shy dog`; the real file was `IMG_Maybe_Dog_2a705290.png` | [`Shy_dog.png`](Shy_dog.png) |
| `02SpitoonDog` | `zcat /usr/share/man/man1/spitoon.1.gz` revealed two lines of dialogue. In `/home/ctf/go_to_opt/Programs/spitoon`, we entered `How tough am I? How tough am I?! I ran 227 of linux programs this lab`, then `Without 227 errors`, followed by `Ctrl+C` without pressing Enter. The program printed `/regular_paw_storage/e230d0d6477947df/Spitoon_dog.png` | [`Spitoon_dog.png`](Spitoon_dog.png) |
| `03RestoreDog` | `/home/ctf/go_to_opt/Doguments/crashed.txt` contained a key for a Vim swap file. `vim -r crashed.txt` recovered the encrypted record; opening it in Vim with the key `b92aa0f97ebaed0d` revealed `/regular_paw_storage/yjxgcaf/Restore_dog.png` | [`Restore_dog.png`](Restore_dog.png) |
| `03AqueousDog` | Base64 text was found in `/home/ctf/.config/fish/config.fish`; `base64 -d` revealed `/regular_paw_storage/e523c9b09c90e881/Aqueous_dog.png` | [`Aqueous_dog.png`](Aqueous_dog.png) |
| `03ProDog` | After logging into the container, the terminal displayed `Woof woof! You caught the ProDog! Your bone is buried at: /regular_paw_storage/aac1e8451a14e8af/Pro_dog.png`. The file was downloaded via SFTP using the exact path | [`Pro_dog.png`](Pro_dog.png) |

Files from `/regular_paw_storage` were downloaded via SFTP using their exact paths. Part of the directory cannot be listed, so `find` does not show all the images there. To submit each PNG to the bot, send it **as a file** after opening the corresponding challenge with `/challenge <name>`.
