//implementation of my own mini shell
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#define PATH_LENGTH 500
char cwd[PATH_LENGTH];
bool isnumber(char name[]) {
    for (int i=0; i<strlen(name);i++) {
    	if (isdigit(name[i])) {
    	    continue;
    	}
    	else {
    	    return false;
    	}
    }
    return true;
}

int main(int argc, char* argv[]) {
    char* call = argv[1];
    //implementation of command pwd, which shows current working directory
    if (strcmp(call,"pwd") == 0) {
        if (getcwd(cwd,sizeof(cwd)) != NULL) {
    	    printf("%s\n",cwd);
        }
    }
    //implementation of command ls, which shows what's inside of directory
    else if (strcmp(call,"ls") == 0) {
        DIR *dir;
        dir = opendir(getcwd(cwd,sizeof(cwd)));
        struct dirent* current_directory;
        while ((current_directory=readdir(dir)) != NULL) {
            printf("%s\n",current_directory->d_name);
        }
    }
    //mini version of top, where the name of the process, pid and state are shown in real time
    else if (strcmp(call,"top") == 0) { 
    	DIR *dir1;
    	struct dirent* cd;
    	while (true) {
    	dir1 = opendir("/proc");
    	while ((cd=readdir(dir1)) != NULL) {
    		if (isnumber(cd->d_name)) {
    		    char path[PATH_LENGTH];
    		    strcpy(path,"/proc/");
    		    strcat(path, cd->d_name);
    		    strcat(path, "/status");
    		    FILE* f = fopen(path,"r");
    		    char s1[200];
    		    while (fscanf(f,"%s",s1) == 1) {
    		    	if (strcmp(s1,"State:") == 0) {
    		    		fscanf(f,"%s",s1);
    		    		printf("Pid: %s, State: %s\n",cd->d_name, s1);  
    		    	}
    		    	if (strcmp(s1,"Name:") == 0) {
    		    	        fscanf(f,"%s",s1);
    		    	        printf("Name: %s, ", s1);
    		    	}
    		    }
    		    fclose(f);
    	        }
    	}
    	closedir(dir1);
    	sleep(1);
    	}
    }
    	
    return 0;
}
