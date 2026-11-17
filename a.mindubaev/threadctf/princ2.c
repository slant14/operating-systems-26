
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

int read_status(int pid,char *name,char *state,int *ppid){
    char path[128],line[256];
    snprintf(path,sizeof(path),"/proc/%d/status",pid);
    FILE *f=fopen(path,"r");
    if(!f) return 0;
    name[0]='\0';
    *state='?';
    *ppid=-1;
    while(fgets(line,sizeof(line),f)){
        if(strncmp(line,"Name:",5)==0)
            sscanf(line+5,"%255s",name);
        else if(strncmp(line,"State:",6)==0)
            sscanf(line+6," %c",state);
        else if(strncmp(line,"PPid:",5)==0)
            sscanf(line+5,"%d",ppid);
    }
    fclose(f);
    return name[0]!='\0';
}
int is_cabin(int pid){
    char name[256],state;
    int ppid;
    if(!read_status(pid,name,&state,&ppid))
        return 0;
    return strstr(name,"cabin")!=NULL;
}
int has_dead_princess(int cabin_pid){
    DIR *proc=opendir("/proc");
    struct dirent *entry;
    if(!proc) return 0;
    while((entry=readdir(proc))!=NULL){
        int pid,parent;
        char name[256],state;
        if(sscanf(entry->d_name,"%d",&pid)!=1)
            continue;
        if(!read_status(pid,name,&state,&parent))
            continue;
        if(strncmp(name,"princess_",9)==0 && state=='Z' && parent==cabin_pid){
            closedir(proc);
            return 1;
        }
    }
    closedir(proc);
    return 0;
}
int main(void){
    int destroyed=0;
    printf("waiting to destroy 50 cabins\n");
    while(destroyed<50){
        DIR *proc=opendir("/proc");
        struct dirent *entry;
        if(!proc){
            usleep(100000);
            continue;
        }
        while((entry=readdir(proc))!=NULL && destroyed<50){
            int cabin;
            char name[256],state;
            int parent;
            if(sscanf(entry->d_name,"%d",&cabin)!=1)
                continue;
            if(!read_status(cabin,name,&state,&parent))
                continue;

            if(!strstr(name,"cabin") || state=='Z')
                continue;

            if(!has_dead_princess(cabin))
                continue;

            printf("targeting cabin %s (PID %d)\n",name,cabin);
            if(kill(cabin,SIGKILL)!=0)
                continue;
            while(kill(cabin,0)==0 || errno==EPERM)
                usleep(10000);
            destroyed++;
            printf("confirmed exited: %d/50\n",destroyed);
        }
        closedir(proc);
        usleep(100000);
    }
    printf("50 cabins confirmed exited\n");
    return 0;
}