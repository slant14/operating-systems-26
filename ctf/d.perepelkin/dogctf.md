### 1. Powered Dog (`powered_dog.png`)
```bash
pwd
# /home/ctf/go_to_opt
cd /opt
cat README.md
# (got path: /regular_paw_storage/c0f7dd844643ddc3/powered_dog.png)
# (locally) scp email-dog@osctf.nnk.moe:/regular_paw_storage/c0f7dd844643ddc3/powered_dog.png .
```
### 2. Edit dog (Edit_dog.png)
```bash
cd ~/go_to_opt/Doguments
ls
cat EDITME
vim EDITME #(made dog 4 characters longer) (got path /regular_paw_storage/ecbd362b7c598b3d/Edit_dog.png)
# (locally) scp email-dog@osctf.nnk.moe:/regular_paw_storage/ecbd362b7c598b3d/Edit_dog.png .
```
### 3. Secret Dog (DGOD.png) (03InsecureDog)
```bash
cd ~/go_to_opt/Doguments/secret_stuff
cat remember.txt
cd super_secret
ls {D,O,G}{D,O,G}{D,O,G}{D,O,G}.png 2>/dev/null
# (output: DGOD.png)
# (locally scp)
```
### 4. Restore Dog  (Restore_dog.png)
```bash
cd ~/go_to_opt/Doguments
cat crashed.txt
ls -la
vim -r .crashed.txt.swp
# (got path, locally scp)
```
### 5. Mascot (mascot.png) (02PetDog)
```bash
cd ~/go_to_opt/Doguments/auto_petter
cat README.md
cat ./tools/assemble_mascot.sh
git reflog
git checkout bea5bd6 -- frames/part1.bin
git checkout af2f71f -- frames/part2.bin
git checkout 01ba238 -- frames/part3.bin
./tools/assemble_mascot.sh
# (output: mascot.png assembled OK (114456 bytes))
# (locally scp)
```
### 6. Rem dog (Rem_dog.png)
```bash
cd ~/go_to_opt/Doguments/unsorted
ls
cat README
md5sum * | awk '{print $1}' | sort | uniq -c
cat $(md5sum * | awk '/^96fe/{print $2}')
rm $(md5sum * | awk '/^d3ca/{print $2}')
# (got path, locally scp)
```
### 7. HOI 3 (hoi_:3.png) (01HelloDog)
```bash
cd ~/go_to_opt
# (locally scp /home/ctf/go_to_opt/hoi_:3.png)
```
### 8. Tailheaded (tailheaded.png)
```bash
cd ~/go_to_opt/Downloads
ls
cat dog.log
head -n 5 dog.log
tail -n 15 dog.log
# (make it together will be a path, locally scp)
```
### 9. Cat_dog (Cat_dog.png)
```bash
cd ~/go_to_opt/Pictures
ls
cat Cat_dog_top Cat_dog_bottom > Cat_dog.png
# (locally scp)
```
### 10. Compile dog (Compile_dog.png)
```bash
cd ~/go_to_opt/Programs
ls
gcc compile_dog.c -o test.out
./test.out
# (got path, locally scp)
```
### 11. Spitoon dog (Spitoon_dog.png)
```bash
cd ~/go_to_opt/Programs
chmod +r spitoon
strings spitoon | awk '/\/regular/{print $0}'
# (got path, locally scp)
```
### 12. Mute dog (Mute_dog.png)
```bash
cd ~/go_to_opt/Programs
./mute_dog < /dev/null > /tmp/out.txt 2>/tmp/err.txt
cat /tmp/out.txt /tmp/err.txt
```
### 13. Shy Dog
```bash
cd ~/go_to_opt/Videos/Shy\ Dog
ls
file * | awk '/PNG/{print $1}'
```
### 14. Scattered dog
```bash
cd ~/go_to_opt/Videos/Scattered\ Dog
ls | sort -n
cat ./{-50..49} > /tmp/scattered_dog.png
```
### 15. Purged dog (03SpyDog)
```bash
cd /home/dog/Doguments/cat_tracker
ls
cat README.md
cat suspects.txt
git log --all --stat 2>/dev/null
git fsck --full | awk '/dangling (commit|tree)/{print $3}'
git ls-tree -r "b85f20f59e91bc57ca892d1911fd8206ffb78268"
# Found cat_cam/frame_0046.png
git cat-file -p 440d27f1ca591f2f1ac937b35be75c5eaaa6036e > /tmp/purged_dog.png
```
### 16. Linked dog (03ChainedDog)
```bash
cd /home/dog/Downloads
ls
cat start
f=start; while true; do sed '/Next/d; s/Content: //' $f; [ $f = end ] && break; f=$(awk '/Next/{print $2}' $f); done > /tmp/dog.png
```
### 17. Used dog
```bash
cd /home/dog
ls
# (Used_dog.png)
```
### 18. History dog
```bash
cat ~/.bash_history  /home/dog/.bash_history ~/.bashrc /home/dog/.bashrc 2>/dev/null | grep "png"
```
### 19. Background dog
```bash
jobs -l
fg %1
# (got path)
```
### 20. Netdog
```bash
cd /usr/local/bin
ps aux
ls -la /usr/share/netdog.png
./netdog
cat < /dev/tcp/127.0.0.1/2184 > /tmp/netdog.png
```
### 21. Prodog
```bash
ps aux
for i in {1..200}; do ps aux; done | grep -vE "sleep|ps aux|dropbear|-bash|netdog|prodog_loop|USER" | sort -u
while ! pkill -u ctf prodog; do :; done
# (/regular_paw_storage/6054898edffa8924/Pro_dog.png)
```
### 22. Aqueous Dog
```bash
cat ~/.config/fish/config.fish
# (here is text encoded in base64, decode it for example through online converter and get path, or ...)
bash ~/.config/fish/config.fish
# (got path)
```