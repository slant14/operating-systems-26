# dog

Instance: dogctf. Result: 13 dog challenges solved (flags accepted by the bot).
Host: osctf.nnk.moe, container: ssh m.torebaev-dog@osctf.nnk.moe (password from the bot).
Paths under /regular_paw_storage/<hex>/ are random per student; the ones below are from
my session. Each task hides one dog picture behind a basic Linux skill.

First look (shows almost every puzzle):
  ls -la ~ ; cat ~/.bash_profile ; ls -la / ; ps aux

| Challenge | Key command | Location of picture / clue |
|-----------|-------------|----------------------------|
| 01HelloDog | `file hoi_:3.png` | already there: ~/go_to_opt/hoi_:3.png |
| 01PoweredDog | `cat /opt/README.md` | clue in /opt/README.md -> /regular_paw_storage/.../powered_dog.png |
| 01HistoryDog | `cat ~/.bash_history` | history shows the cp from /super_secret_doggo_vault/grandpaw.png |
| 03AqueousDog | `fish` then `cat ~/.config/fish/config.fish` | path in the fish shell config |
| 01TailheadedDog | `head -n5 dog.log ; tail -n15 dog.log` | ~/go_to_opt/Downloads/dog.log (path at head+tail) |
| 01CatDog | `cat Cat_dog_top Cat_dog_bottom > /tmp/cat_dog.png` | two halves in ~/go_to_opt/Pictures |
| 02ShyDog | `file * | grep 'PNG image'` then cp | ~/go_to_opt/Videos/"Shy dog": the name lies, bytes say PNG |
| 03InsecureDog | `cat remember.txt` ; `cp super_secret/DGDO.png /tmp/` | ~/go_to_opt/Doguments/secret_stuff/super_secret (d-wx--x--x: no ls, but cp by exact name works) |
| 01CompileDog | `gcc compile_dog.c -o compile_dog ; ./compile_dog` | ~/go_to_opt/Programs -> /regular_paw_storage/.../Compile_dog.png |
| 02MuteDog | `./mute_dog > /tmp/out.txt 2> /tmp/err.txt` | ~/go_to_opt/Programs: real path is on stderr, art on stdout |
| 02SpitoonDog | run `./spitoon`, exit with Ctrl+C (not Enter); or `strings spitoon | grep -i tough` | ~/go_to_opt/Programs -> /regular_paw_storage/.../Spitoon_dog.png |
| 03ProDog | `until pkill -x prodog; do sleep 0.1; done` | prodog (user ctf) lives ~0.3s; on SIGTERM the loop prints /regular_paw_storage/.../Pro_dog.png |
| 01EditDog | `sed -i '7,11s/^\(.\{19\}\)\(.\)/\1\2\2\2\2\2\2\2\2/' EDITME` | ~/go_to_opt/Doguments/EDITME: grow 5 dog lines by 7 chars; checker prints Edit_dog.png |

Note: CatDog, ShyDog and InsecureDog pictures land in /tmp (built or copied there);
the rest print a /regular_paw_storage/<hex>/ path owned by the task. Copy each to the PC
with scp and send the PNG to the bot.
