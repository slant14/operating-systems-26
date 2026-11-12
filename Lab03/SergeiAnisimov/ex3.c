/* Lab 03 Sergei Anisimov CSE-04 */
#include <limits.h>
#include <stdio.h>
#include <string.h>

enum { MAX_NAME = 63, MAX_PATH = 2048, MAX_DATA = 1024 };

struct Directory;

struct File {
    unsigned int id;
    char name[MAX_NAME + 1];
    size_t size;
    char data[MAX_DATA];
    struct Directory *directory;
};

struct Directory {
    char name[MAX_NAME + 1];
    struct File files[UCHAR_MAX];
    struct Directory *sub_dirs[UCHAR_MAX];
    unsigned char nf;
    unsigned char nd;
    char path[MAX_PATH];
};

int overwrite_to_file(struct File *file, const char *text)
{
    size_t size = strlen(text) + 1;
    if (size > sizeof file->data)
        return 0;

    memmove(file->data, text, size);
    file->size = size;
    return 1;
}

int append_to_file(struct File *file, const char *text)
{
    size_t used = strlen(file->data);
    size_t added = strlen(text);
    if (added >= sizeof file->data - used)
        return 0;

    memmove(file->data + used, text, added + 1);
    file->size = used + added + 1;
    return 1;
}

void printp_file(struct File *file)
{
    const char *path = file->directory->path;
    printf("%s%s%s\n", path, strcmp(path, "/") == 0 ? "" : "/", file->name);
}

int add_file(struct File *file, struct Directory *dir)
{
    if (dir->nf == UCHAR_MAX)
        return 0;

    file->directory = dir;
    dir->files[dir->nf++] = *file;
    return 1;
}

static int add_dir(struct Directory *child, struct Directory *parent)
{
    size_t parent_length = strlen(parent->path);
    size_t name_length = strlen(child->name);
    int needs_separator = strcmp(parent->path, "/") != 0;
    size_t prefix_length = parent_length + needs_separator;
    if (parent->nd == UCHAR_MAX || prefix_length + name_length >= sizeof child->path)
        return 0;

    memcpy(child->path, parent->path, parent_length);
    if (needs_separator)
        child->path[parent_length] = '/';
    memcpy(child->path + prefix_length, child->name, name_length + 1);
    parent->sub_dirs[parent->nd++] = child;
    return 1;
}

static void print_files(struct Directory *dir)
{
    for (unsigned int i = 0; i < dir->nf; ++i)
        printp_file(&dir->files[i]);
    for (unsigned int i = 0; i < dir->nd; ++i)
        print_files(dir->sub_dirs[i]);
}

int main(void)
{
    static struct Directory root = {.name = "/", .path = "/"};
    static struct Directory home = {.name = "home"};
    static struct Directory bin = {.name = "bin"};
    struct File bash = {.id = 1, .name = "bash"};
    struct File first = {.id = 2, .name = "ex3_1.c"};
    struct File second = {.id = 3, .name = "ex3_2.c"};

    if (!add_dir(&home, &root) || !add_dir(&bin, &root) ||
        !overwrite_to_file(&bash, "Bourne Again Shell!!") ||
        !overwrite_to_file(&first, "int printf(const char * format, ...);\n") ||
        !overwrite_to_file(&second, "//This is a comment in C language\n") ||
        !add_file(&bash, &bin) || !add_file(&first, &home) || !add_file(&second, &home) ||
        !append_to_file(&home.files[0], "int main(){printf(\"Hello World!\");}\n")) {
        fputs("Could not build the directory tree.\n", stderr);
        return 1;
    }

    print_files(&root);
    return 0;
}
