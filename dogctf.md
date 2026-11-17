## 01BackgroundDog
When the container for ctf is started, there is one stopped process. We can return it to the foreground: \
fg %1 \
regular_paw_storage/7d1dca5ceb348508/Background_dog.png

## 01CatDog
Pictures folder \
The picture consists of two parts that need to be concatenated: \
cat Cat_dog_top Cat_dog_bottom > catdog.png

## 01CompileDog
In the Programs folder, there was a file that needed to be compiled and executed: \
gcc compile_dog.c -o compile_dog \
./compile_dog

## 01EditDog
Doguments -> EDITME \
The challenge requires to modify the dog in file EDITME by making the dog longer. \
regular_paw_storage/81473ce290813132/Edit_dog.png

## 01HelloDog
When the container is started there is hoi_:3.png picture in the go_to_opt folder. \
/home/ctf/go_to_opt/hoi_:3.png

## 01HistoryDog
The history command gives the location of the picture: \
history \
regular_paw_storage/a6d6f4feae934112/grandpaw.png

## 01PoweredDog
cd / -> cd opt -> cat README.md \
at README.md: \
Congrats on discovering your address! \
Here is your dog image: /regular_paw_storage/b19f675cf9fd70cb/powered_dog.png

## 01TailheadedDog
cd Downloads -> nano dog.log \
The start of the file gives the beginning of the location: regular_paw_storage/aaea26ec \
The end of the file gives the ending of the location: d2ff6b4c/tailheaded.png \
regular_paw_storage/aaea26ecd2ff6b4c/tailheaded.png

## 02MuteDog
By running ./mute_dog, the program tells us that terminals are listening. Therefore, we can put information into file, and it turns \ out that it will give us the location of the file. input.txt will contain the first part and error.txt will contain the second part. \
./mute_dog > input.txt 2> error.txt \
/regular_paw_storage/6810fa15a0da2515/Mute_dog.png 

## 02SpitoonDog
By running ./spitoon, the program gives us a hint by saying: "Come back when you are more of a man". Therefore we need to run man spitoon. \
By completing the dialogue successfully, we obtain the location of the file: \
regular_paw_storage/19f456eed91fe397/Spitoon_dog.png


## 03InsecureDog
For this challenge, I wrote a program that enumerates all possible combinations with two repeated letters and checks whether the file \ with such name is not null, if it is not null, then we receive our file name: \
home/ctf/go_to_opt/Doguments/secret_stuff/super_secret/DDGO.png

## 03UsedDog
cd / -> cd home -> cd dog. \
This folder contains Used_dog.png
