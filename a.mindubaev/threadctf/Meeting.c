#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>

pthread_mutex_t silence_spell = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t truth_and_order_spell = PTHREAD_MUTEX_INITIALIZER;
pthread_t retrieval;
pthread_t meeting_occurence;

static void* retrieve_the_fabric(void* arg) {
    char* flag_fabric_storage = malloc(256);
    FILE* f = fopen("/root/flag.txt", "r");
    if (f) {
        fgets(flag_fabric_storage, 256, f);
        fclose(f);
    }
    sleep(1);
    return flag_fabric_storage;
}

typedef void* (*meeting_func_t)(void*);

int main() {
    void* flag_fabric_storage_pointer;
    void* name_pointer;
    
    pthread_mutex_lock(&silence_spell);
    
    pthread_create(&retrieval, NULL, retrieve_the_fabric, NULL);
    
    void* lib = dlopen("./libmeeting.so", RTLD_NOW);
    if (!lib) {
        fprintf(stderr, "Failed to load library. Cant have a meeting. Error: %s\n", dlerror());
        return 1;
    }
    
    meeting_func_t meeting = (meeting_func_t)dlsym(lib, "meeting");
    if (!meeting) {
        fprintf(stderr, "Failed to find meeting function. Can't have a meeting. Error: %s\n", dlerror());
        return 1;
    }
    
    pthread_create(&meeting_occurence, NULL, meeting, NULL);
    
    pthread_mutex_lock(&truth_and_order_spell);
    sleep(5);
    pthread_mutex_unlock(&truth_and_order_spell);
    
    if (pthread_join(retrieval, &flag_fabric_storage_pointer) != 0) {
        printf("The fabric is nowhere to be found! Polythread is very angry, so you'll have to stay out of faywild for a while.\n");
        printf("Thankfully, your name is still with you, so you are free to lose it another day.\n");
        exit(0);
    }
    
    pthread_join(meeting_occurence, &name_pointer);
    
    pthread_mutex_unlock(&silence_spell);
    
    printf("Polythread has your concent and name: %s\n", (char*)name_pointer);
    printf("Good luck, you'll be dyeing the buttons forever now!\n");
    printf("What did you think was going to happen when you give up your name to fay?\n");
    
    
    return 1;
}
