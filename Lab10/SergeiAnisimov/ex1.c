/* Lab 10 Sergei Anisimov CSE-04 */
#define _XOPEN_SOURCE 700
#include <dirent.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static int visit_links(const char *source, int remove_duplicates)
{
    struct stat reference;
    if (lstat(source, &reference) == -1 || !S_ISREG(reference.st_mode))
        return -1;
    DIR *directory = opendir(".");
    if (!directory)
        return -1;
    struct dirent *entry;
    int result = 0;
    while ((entry = readdir(directory)) != NULL) {
        const char *name = entry->d_name;
        struct stat st;
        if (lstat(name, &st) == -1 || st.st_dev != reference.st_dev ||
            st.st_ino != reference.st_ino)
            continue;
        if (remove_duplicates) {
            if (strcmp(name, source) != 0 && unlink(name) == -1) {
                result = -1;
                break;
            }
        } else {
            char *path = realpath(name, NULL);
            if (!path) {
                result = -1;
                break;
            }
            printf("inode=%ju %s\n", (uintmax_t)st.st_ino, path);
            free(path);
        }
    }
    closedir(directory);
    return result;
}

static int find_all_hlinks(const char *source)
{
    return visit_links(source, 0);
}

static int unlink_all(const char *source)
{
    if (visit_links(source, 1) == -1)
        return -1;
    char *path = realpath(source, NULL);
    if (!path)
        return -1;
    printf("Kept: %s\n", path);
    free(path);
    return 0;
}

static int create_sym_link(const char *source, const char *link_name)
{
    return symlink(source, link_name);
}

static int write_text(const char *name, const char *mode, const char *text)
{
    FILE *file = fopen(name, mode);
    if (!file)
        return -1;
    int failed = fputs(text, file) == EOF;
    return fclose(file) == EOF || failed ? -1 : 0;
}

int main(int argc, char **argv)
{
    if (argc != 2 && argc != 3) {
        fprintf(stderr, "Usage: %s directory [absolute-outside-path]\n", argv[0]);
        return 2;
    }
    const char *outside = argc == 3 ? argv[2] : "/tmp/myfile1.txt";
    if (outside[0] != '/') {
        fputs("The outside path must be absolute.\n", stderr);
        return 2;
    }
    if (chdir(argv[1]) == -1) {
        perror(argv[1]);
        return 1;
    }

    if (write_text("myfile1.txt", "wx", "Hello world.\n") == -1 ||
        link("myfile1.txt", "myfile11.txt") == -1 ||
        link("myfile1.txt", "myfile12.txt") == -1 ||
        find_all_hlinks("myfile1.txt") == -1) {
        perror("create and list hard links");
        return 1;
    }
    FILE *file = fopen("myfile1.txt", "r");
    if (!file) {
        perror("fopen");
        return 1;
    }
    int ch;
    while ((ch = fgetc(file)) != EOF)
        putchar(ch);
    fclose(file);

    /* Link then unlink to move the name without overwriting an existing file. */
    if (link("myfile1.txt", outside) == -1 || unlink("myfile1.txt") == -1 ||
        write_text("myfile11.txt", "a", "Changed through an inside hard link.\n") == -1 ||
        create_sym_link(outside, "myfile13.txt") == -1 ||
        write_text(outside, "a", "Changed through the outside name.\n") == -1 ||
        unlink_all("myfile11.txt") == -1) {
        perror("link experiment");
        return 1;
    }
    struct stat st;
    if (lstat("myfile11.txt", &st) == -1) {
        perror("lstat");
        return 1;
    }
    printf("myfile11.txt: inode=%ju links=%ju size=%jd\n",
           (uintmax_t)st.st_ino, (uintmax_t)st.st_nlink, (intmax_t)st.st_size);
    return 0;
}
