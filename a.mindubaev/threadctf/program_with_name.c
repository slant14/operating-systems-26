#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void* meeting(void* arg){
    char *flag=malloc(256);
    if(!flag){
        return NULL;
    }
    FILE *f=fopen("/root/flag.txt","r");
    if(!f){
        strcpy(flag,"could not retrieve fabric");
        return flag;
    }
    fgets(flag,256,f);
    fclose(f);
    return flag;
}