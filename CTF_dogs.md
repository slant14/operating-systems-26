## CTF dogs (findingdory)

### 01BackgroundDog
This task is related to background processes

**1) View all background processes**
```bash
ps aux
```
We get all processe, but the important one is:
ctf          239  0.0  0.0    980   656 pts/0    T    11:34   0:00 /usr/local/bin/background_dog /tmp/bgdog

**2) Check whether this process exists within my terminal and what its status is**
```bash
jobs
```
 We received this process, assigned the number 1, and it has been suspended.

**3) Continue running this process in the background**
```bash
bg %1
```
We get th dog: */regular_paw_storage/05eb161e2642d038/Background_dog.png*

**4) Kill the process**
```bash
kill %1
```

### 01CatDog

**1) Go to the desired directory**
```bash
ls
```
We get: Doguments  Downloads  Pictures  Programs  Videos  hoi_:3.png

```bash
cd Pictures
ls
```
We get: Cat_dog_bottom  Cat_dog_top

We have two parts of an image and need to combine them into a single file

**2) Combine two files**
```bash
cat Cat_dog_top Cat_dog_bottom > Cat_dog.png
```

```bash
ls
```
 We get the file, that we need: Cat_dog.png  Cat_dog_bottom  Cat_dog_top

**3) See the path**
```bash
pwd
```
We get the path of our dog: /home/ctf/go_to_opt/Pictures => */home/ctf/go_to_opt/Pictures/Cat_dog.png*

### 01CompileDog

**1) Go to the desired directory**
```bash
cd Programs
ls
```
We get: compile_dog.c  mute_dog  spitoon

**2) Compile the file compile_dog.c and run**
```bash
gcc compile_dog.c -o main
./main
```
We get the dog: */regular_paw_storage/b1377bc26bc83199/Compile_dog.png*

### 01EditDog

**1) Go to the desired directory**
```bash
cd Doguments
ls
```

We get: EDITME  auto_petter  crashed.txt  secret_stuff  unsorted

We see the file EDITME and let's open it

**2) Open the file EDITME**
```bash
nano EDITME
```

We see the task:

Make the dog 7 characters longer. Preserve the integrity of the dog.

DO NOT EDIT ANYTHING OUTSIDE THESE SAFETY LINES
\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\


                         .-.
     (__________________()6 `-,
     (   ______________   /''"`
     //\\             //\\
jgs  "" ""            "" ""


\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
DO NOT EDIT ANYTHING OUTSIDE THESE SAFETY LINES

We need to make the dog longer (+7 characters):

Make the dog 7 characters longer. Preserve the integrity of the dog.

DO NOT EDIT ANYTHING OUTSIDE THESE SAFETY LINES
\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\


                                .-.
     (_________________________()6 `-,
     (   _____________________   /''"`
     //\\                    //\\
jgs  "" ""                   "" ""


\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
DO NOT EDIT ANYTHING OUTSIDE THESE SAFETY LINES

Save the file: Ctrl + O, Enter, Ctrl + X

And we get the dog: Woof! You have solved Edit_dog! Here's your dog pic: */regular_paw_storage/d7f2d764bb17099a/Edit_dog.png*

### 01HelloDog
 
