#include <stdio.h>
#include <string.h>

#define MAX_PATH 2048

struct Directory;

struct File {
    unsigned int id;              
    char name[64];                
    unsigned int size;            
    char data[1024];              
    struct Directory *directory;  
};

struct Directory {
    char name[64];
    struct File files[64];
    struct Directory *sub_dirs[64];
    unsigned char nf;
    unsigned char nd;
    char path[MAX_PATH];
};


void show_file(struct File *file)
{
    printf("%s ", file->name);
}

void show_dir(struct Directory *dir)
{
    printf("\nDIRECTORY\n");
    printf(" path: %s\n", dir->path);
    printf(" files:\n");
    printf(" [ ");
    for (int i = 0; i < dir->nf; i++)
    {
        show_file(&(dir->files[i]));
    }
    printf("]\n");
    printf(" directories:\n");
    printf(" { ");
    for (int i = 0; i < dir->nd; i++)
    {
        show_dir(dir->sub_dirs[i]);
    }
    printf("}\n");
}

void add_dir(struct Directory *dir1, struct Directory *dir2)
{
    if (dir1 && dir2)
    {
        dir2->sub_dirs[dir2->nd] = dir1;
        dir2->nd++;
        char temp_path[MAX_PATH];
        if (strcmp(dir2->path, "/"))
        {
            strcpy(temp_path, dir2->path);
            strcat(temp_path, "/");
            strcat(temp_path, dir1->name);
            strcpy(dir1->path, temp_path);
        }
        else
        {
            strcpy(temp_path, "/");
            strcat(temp_path, dir1->name);
            strcpy(dir1->path, temp_path);
        }
    }
}

void overwrite_to_file(struct File *file, const char *str)
{
    strcpy(file->data, str);
    file->size = (unsigned int)strlen(file->data) + 1;
}

void append_to_file(struct File *file, const char *str)
{
    strcat(file->data, str);
    file->size = (unsigned int)strlen(file->data) + 1;
}

void printp_file(struct File *file)
{
    if (file && file->directory)
    {
        const char *dpath = file->directory->path;
        if (strcmp(dpath, "/") == 0)
            printf("/%s\n", file->name);
        else
            printf("%s/%s\n", dpath, file->name);
    }
}

void add_file(struct File *file, struct Directory *dir)
{
    if (file && dir)
    {
        dir->files[dir->nf] = *file;
        dir->files[dir->nf].directory = dir;
        dir->nf++;
    }
}

int main(void)
{
    struct Directory root;
    strcpy(root.name, "/");
    root.nf = 0;
    root.nd = 0;
    strcpy(root.path, "/");

    struct Directory home;
    strcpy(home.name, "home");
    home.nf = 0;
    home.nd = 0;
    home.path[0] = '\0';

    struct Directory bin;
    strcpy(bin.name, "bin");
    bin.nf = 0;
    bin.nd = 0;
    bin.path[0] = '\0';

    add_dir(&home, &root);  
    add_dir(&bin,  &root);  

    struct File bash;
    bash.id = 1;
    strcpy(bash.name, "bash");
    bash.size = 0;
    bash.data[0] = '\0';
    bash.directory = NULL;

    add_file(&bash, &bin);

    struct File ex3_1;
    ex3_1.id = 2;
    strcpy(ex3_1.name, "ex3_1.c");
    strcpy(ex3_1.data, "int printf(const char * format, ...);");
    ex3_1.size = (unsigned int)strlen(ex3_1.data) + 1;
    ex3_1.directory = NULL;

    struct File ex3_2;
    ex3_2.id = 3;
    strcpy(ex3_2.name, "ex3_2.c");
    strcpy(ex3_2.data, "//This is a comment in C language");
    ex3_2.size = (unsigned int)strlen(ex3_2.data) + 1;
    ex3_2.directory = NULL;

    add_file(&ex3_1, &home);
    add_file(&ex3_2, &home);

    overwrite_to_file(&bin.files[0], "Bourne Again Shell!!");

    append_to_file(&home.files[0], "int main(){printf(\"Hello World!\")}");

    printp_file(&bin.files[0]);    
    printp_file(&home.files[0]);   
    printp_file(&home.files[1]);

    return 0;
}