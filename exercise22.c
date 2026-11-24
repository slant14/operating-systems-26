#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
int upper(int c) {
    if (c >= 65 && c <= 90) {
    	return 1;
    }
    else {
    	return 0;
    }
}

int main() {
    FILE* file;
    file = fopen("text.txt","r");
    int fd = fileno(file);
    
    long long offset = 0;
    long long file_size = 500 * 1024 * 1024;
    int chunk_size = 4096 * 1024;
    int k = 0;
    
    while (offset < file_size) {
        char* ch = mmap(NULL,chunk_size,PROT_READ,MAP_PRIVATE,fd,offset);
        for (int i = 0; i < chunk_size; i++) {
            if (upper(ch[i])) {
            	k+=1;
            }
        }
        offset += chunk_size;
        printf("%d ",k);
        k = 0;
        munmap(ch,chunk_size);
    }
    
    fclose(file);
}
