#include<stdio.h>
#include<stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 64
#define MAX_PATH_LEN 2048
#define MAX_DATA_LEN 1024
#define MAX_FILES 255
#define MAX_DIRS 255

struct Directory;

struct File {
    int id;
    char name[MAX_NAME_LEN];
    int size;
    char data[MAX_DATA_LEN];
    struct Directory *directory;
};

struct Directory {
    char name[MAX_NAME_LEN];
    struct File *files[MAX_FILES];
    struct Directory *directories[MAX_DIRS];
    unsigned char nf;
    unsigned char nd;
    char path[MAX_PATH_LEN];
};

void overwrite_to_file(struct File* file, const char* str) {

}

void append_to_file(struct File* file, const char* str) {

}

void printp_file(struct File* file) {

}

void add_file(struct File* file, struct Directory* dir) {
    
}

int main() {

    return 0;
}