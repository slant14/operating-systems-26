#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>

#define FLAGLEN 55
#define SECRET_OFFSET 64

typedef void *(*guess_function)(char *);

void run(void)
{
    guess_function get_guess=(guess_function)dlsym(RTLD_DEFAULT,"guess");
    if(!get_guess){
        write(2, "could not resolve guess()\n", 26);
        return;
    }
    unsigned char *ptr=get_guess("wrong");
    if(!ptr){
        write(2,"allocation failed\n",19);
        return;
    }
    unsigned char flag[FLAGLEN];
    //the main magic!!
    volatile unsigned char *secret=(volatile unsigned char *)(ptr+SECRET_OFFSET);
    for(int i=0;i<FLAGLEN;i++){
        flag[i]=secret[i];
    }
    write(1,"recovered flag: ",16);
    write(1,flag,FLAGLEN);
    write(1,"\n",1);
}