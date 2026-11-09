//Creation of text.txt (500 MiB), page size is 4096 = 4 KiB, chunk size is 4 MiB
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
int main() {
    FILE* file1;
    FILE* file2;
    
    file1 = fopen("text.txt","a");
    file2 = fopen("/dev/random","r");
    
    int k = 0;
    int lines = 0;
    
    while (lines < 500*1024) {
    	char c = fgetc(file2);
    
    	if (isprint(c)) {
          fputc(c,file1);
    	    k += 1;
        }
        
        else {
    	    continue;
        }
        
        if (k >= 1024) {
   	    k = 0;
   	    fputs("\n",file1);
    	    lines++;
        }	
    }  
    
    fclose(file1);
    fclose(file2);
}
