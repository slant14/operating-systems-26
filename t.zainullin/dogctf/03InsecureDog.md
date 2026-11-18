# dogctf - 03InsecureDog

**Flag:** accepted by the bot as a file (`OGDG.png`)

## Task

```
.bash_profile:  chmod 311 /home/ctf/go_to_opt/Doguments/secret_stuff/super_secret
```

`311` means `--wx--x--x`: you can enter the directory and create files, but you
can NOT list its contents.

## Idea

The protection is incomplete ("insecure"): directory listing is blocked, but a
file inside is still world-readable, so opening it **by its exact name** works
fine. The file name was visible in the earlier directory listing (before the
permissions were tightened) and via `find`:

```
find / -xdev -iname '*.png' | grep -v 'Shy dog'
...
/home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/OGDG.png
```



```
file ~/go_to_opt/Doguments/secret_stuff/super_secret/OGDG.png
# PNG image data, 300 x 225, 8-bit/color RGBA
```
