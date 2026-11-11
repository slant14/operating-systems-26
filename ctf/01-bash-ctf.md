# Bash CTF (dogctf)

I am using this command to move every picture to my local computer in this ctf.
```
scp v.orekhova-dog@osctf.nnk.moe:/tmp/cat_dog.png .
```

## 1. BackgroundDog
So, i need to find processes working in background. Maybe let's just bring it to the foreground.
```
fg
```
> /regular_paw_storage/5cd0794c46e3f9d3/Background_dog.png

## 2. CatDog
There are two png pictures in Picture directory: with Bottom and Top. So maybe we need to combine them to get a png picture.
```
cat Cat_dog_top Cat_dog_bottom > /tmp/cat_dog.png
```
> /tmp/cat_dog.png

## 3. CompileDog
Doguments -> Compile C file. We got a path to the picture, so take it.
> /regular_paw_storage/276643af9e19748d/Compile_dod.png

## 4. EditDog
There was a file EDITME, so i edited it.. Lenghten the wiener dog.
> Woof! You have solved Edit_dog! Here's your dog pic: /regular_paw_storage/f2f78946b59c3f29/Edit_dog.png

## 5. HelloDog
Just take the picture /home/ctf/go_to_opt/hoi_:3.png.
> /home/ctf/go_to_opt/hoi_:3.png

## 6. HistoryDog
Let's just look at the commands' history. There we find a path to the file.
```
history
```
The string: cp /super_secret_doggo_vault/grandpaw.png /regular_paw_storage/af36f9a553b06d07/grandpaw.png
> /regular_paw_storage/af36f9a553b06d07/grandpaw.png

## 7. PoweredDog
The directory is called "go_to_opt". So, let's actually go to opt. Yeah, there is a README file.
```
cd /opt
ls -la
cat README.md
```
> Congrats on discovering your address! Here is your dog image: /regular_paw_storage/22ee3e4f0169cb3a/powered_dog.pngctf@8ad359da46e0$ 

## 8. TailheadedDog
End file:
1424b775/tailheaded.png
I wonder if there is more at the start of it..

Start file:
/regular_paw_storage/a8257da1
I wonder if there is more at the end of it..  
So, by combining them, we got the picture. 
> /regular_paw_storage/a8257da11424b775/tailheaded.png

## 9. MuteDog
So, the terminals are listening. So, we need to redirect the stream into files

```
./mute_dog > /tmp/out.txt 2> /tmp/err.txt
cat /tmp/out.txt
cat /tmp/err.txt
```
> /regular_paw_storage/120ee54037cb991d/Mute_dog.png

## 10. PetDog
`src/petting_core.sh` - function-instructions
`tools/mascot` - mascot png is what we are looking for, it seems. It is divided into frames.
`tools/assemble_mascot.sh` we have the instruction how to assemble mascot.
```
cat frames/part3.bin frames/part2.bin frames/part1.bin > mascot.png
```
But, for now, this is a wrong mascot. There are .git folder, so we need to work with commits somehow and make a correct mascot.
```
git status
```
We need to resolve all the merge conflicts in config/petting.cfg.
```
git checkout --theirs config/petting.cfg
git add config/petting.cfg
git rebase --continue
```
CONFLICT (content): Merge conflict in frames/part2.bin
```
git checkout --theirs frames/part2.bin
git add frames/part2.bin
git rebase --continue
```
CONFLICT (content): Merge conflict in frames/part3.bin
error: could not apply 0330794... wip: upscale part3 for the retina mockup (ringing everywhere, parking this)
```
git rebase --skip
```
Auto-merging frames/part1.bin
CONFLICT (content): Merge conflict in frames/part1.bin
error: could not apply 5ae5170... wip: hi-bitrate re-encode of part1 (adds artifacts, parking this)
```
git rebase --skip
```
```
git checkout --theirs src/petting_core.sh
git add src/petting_core.sh
git rebase --continue
```

The file is still corrupted. let's check some parts. Let's take all the old versions.
```
git log frames/part1.bin
git log frames/part2.bin

git show ea7fac1:frames/part1.bin > frames/part1.bin
git show e90e740:frames/part2.bin > frames/part2.bin
./tools/assemble_mascot.sh
```
> /home/ctf/go_to_opt/Doguments/auto_petter/mascot.png

## 11. RemDog
Go to the directory unsorted. The README.md says that someday we need to delete all the angry dogs. Cat every file and look for angry dogs, then delete them. For easy searching make a pretty output of all the files together:
```
for f in *; do echo "===== $f ====="; cat "$f"; echo; done
rm 4d05c570 711af14b 74f3f16d 7741377a 91e17350 97d728d2 998c65ed b5e35dee bc4ecc3c be8248d2 c04a88d9 c15c12b9
```
> Woof! You have solved Rem_dog! Here's your dog pic: /regular_paw_storage/ffe4c920193d1a6f/Rem_dog.png

## 12. ScatteredDog
Let's go to the folder ctf/Videos. There are a lot of videos, so, let's concatenate them:
```
cat -- $(ls | sort -n) > /tmp/kitty.png
```
(why the file is named kitty idk, i was watching kitty memes on my break..)
> /tmp/kitty.png .

## 13. ShyDog
shy -> maybe it is hiding. There are a lot of files in Shy directory. Let's sort them by their file extensions. There is only one png image - must be our picture.
```
file * | awk -F': ' '{print$2}' | sort | uniq -c
```
> /home/ctf/go_to_opt/Videos/Shy dog/PIC_Maybe_Dog_a2ea7da7.png

## 14. SpitoonDog
The file doesn't have read permissions, so let's give it to it.
```
chmod u+r spitoon
```
Let's run the script.
```
./spitoon
```
I think the dialog should looks like this.
```
Welcome to the salty spitoon, how tough are ya?
How tough am I? How tough am I?! I ran 243 of linux programs this lab
Yea, so?
Without 243 errors
```

To find the answers I used:
```
strings spitoon | grep tough
strings spitoon | grep -nE 'tough|yea|so|listen|kid|come|back|man'
strings spitoon | grep -E '243'
```
But my dialog always didn't work. I always got: "Listen, kid: come back when you are more of a man".
So, I used:
```
strings spitoon | grep -iE 'spitoon|tough|yea|without|243|this way|listen|kid'
```
> /regular_paw_storage/f2a0de2f9736b488/Spitoon_dog.png                       

## 15. AqueousDog

## 16. ChainedDog
There are a lot of .txt files in dog/Downloads directory. As we can see from start and end files, they all have a string with `Next: <next file>`. so we should combine all of them in such order. But it's gonna be hard to do by hand, so let's use a script-like command.
```
f=start
> /tmp/chain.png
while [ "$f" != end ]; do
    tail -n +2 "$f" | tail -c +10 >> /tmp/chain.png
    f=$(head -1 "$f" | cut -d' ' -f2)
done
tail -n +2 end | tail -c +10 >> /tmp/chain.png
```
> /tmp/chain.png

## 17. InsecureDog
Search in the directory Doguments/secret_stuff and always divide the possible set by 2 to find what file actually exists.
```
cat /home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/{DOOG,DOGO,DGOO,DGDO,DGOD,DDOG,DODG,DOGD,ODOG,OGOD,OGDO,ODGO,OD
GD,ODDG,OGDG,OGGD,OODG,OOGD,GDOO,GODO,GOOD,GDDO,GDOD,GODD,GDOG,GDGO,GODG,GGDO,GGOD,DOGG,DGGO,DGOG,ODGG,OGDD}.png
```
> The right dog is /home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/OGDD.png

## 18. NetDog

## 19. ProDog

## 20. RestoreDog
```
cat crashed.txt
```
The output:
```
My session crashed while I was writing down where I hid my favorite dog...
Good thing vim keeps swap files around.
I encrypted my notes, so here is the recovery key: c0d2b7d46f006fca
```
Vim stores swap files around, so let's find it and restore it using recovery key.
```
vim -r .crashed.txt.swp
c0d2b7d46f006fca
```
> /regular_paw_storage/tw4lozj/Restore_dog.png

## 21. SpyDog

## 22. UsedDog
Maybe there is a file that somebody is using. There is a file Used_dog.png in /home/dog directory. Just move to the local computer.
```
ls -la
```
> /home/dog/Used_dog.png


## Unsolved Yet

n. smth with cats
```
ls
```
There 2 folders now.. Has it always been this number, or I have hallucinations..
```
ctf  dog
```

```
cd dog
ls -s
cd Doguments/
ls -la
cd cat_tracker
cat README.md 

./collar/gps_poll.sh > logs/track.csv
./tools/parse_tracks.sh logs/track.csv

cd ..
cat suspects.txt 
# suspects under surveillance
1. the orange one - naps 14h/day, still somehow plotting
2. the gray one - fast, loud, knocks things
3. the small one - may not be a cat, further research needed
note: do NOT re-upload the cat cam archive. the fence has seen enough.

./gps_poll.sh 
1793998037,26579,39406,1
1793998038,28929,30974,1
1793998039,11652,28027,0
1793998040,61357,52314,1
1793998041,23145,4059,0
1793998042,5640,32295,0
1793998043,36494,43807,0
1793998044,9458,38206,1
1793998045,32438,10420,0
1793998046,23319,60812,0
1793998047,15780,38331,0
1793998048,53955,49871,1

cat gps_poll.sh
```
