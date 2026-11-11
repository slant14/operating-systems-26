# Threads CTF

## 1. decrypt
1) Download the Decrapinator torrent file.
2) Look inside the `shards.tar.gz`. There are 50 files inside. There is encrypted data inside of these shards, so we need to decrypt all of this using the decryptor. (flags: t - list, z - gzip, f - this file, x - extract)
```
tar tzf shards.tar.gz
```
3) If we are going to read every shard sequentially, there will not be enough time to do this, so we need to do it in parallel. We will do this with `fork()`. Let's write manager.c with such idea.
  
**manager.c**
```manager.c
int main(int argc, char *argv[]) {
    char *dec = argv[1];
    char *dir = argv[2];
    int n = atoi(argv[3]);
    int K = atoi(argv[4]);

    system("mkdir -p out");

    // run k decryptors for each shard, and wait for them to finish
    for (int i = 0; i < n; i++)
        for (int k = 0; k < K; k++)
            if (fork() == 0) {
                char shard[512], outp[512];
                sprintf(shard, "%s/%d.shard", dir, i);
                sprintf(outp,  "out/%d_%d.txt", i, k);
                freopen(outp, "w", stdout);
                freopen("/dev/null", "w", stderr);
                execl(dec, dec, "decrypt", shard, (char*)NULL);
                exit(1);
            }

    for (int i = 0; i < n * K; i++) wait(NULL);
    
    // read the first non-empty output for each shard
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < K; k++) {
            char outp[512], buf[4096];
            sprintf(outp, "out/%d_%d.txt", i, k);
            FILE *f = fopen(outp, "r");
            if (!f) continue;
            if (fgets(buf, sizeof buf, f)) {
                buf[strcspn(buf, "\r\n")] = 0;
                printf("%s", buf);
                fclose(f);
                break;
            }
            fclose(f);
        }
    }
    printf("\n");
    return 0;
}
```

```
cd ~/i/o/ctf
tar xzf shards.tar.gz
./manager /home/me/.void/Decrapinator.v2.Linux.MacOS.Cracked/decrapinator-x86_64-linux shards 50 10
```

> threadctf{YoU_70TaLlY_WrotE_a_pROp3R_pro9r4m_f0r_thIs_riGh7}


## 2. discodetective
crazy


## 3. fortforks


## 4. needle
1) Read the letter
2) write a `program_with_name.c` with a meeting function where you bring the name. But you need to bring the flag as your name, so it will be printed instead of your name.

**program_with_name.c**
```program_with_name.c
void* meeting(void* arg) {
    (void)arg;
    char* name = malloc(256);
    memset(name, 0, 256);

    FILE* f = fopen("/root/flag.txt", "r");
    if (f) {
        fgets(name, 256, f);
        fclose(f);
    }
    return name;
}
```
3. Then, join these two files using the given instruction.
> Please make sure your `libmeeting.so` is right next to the meeting place.  
> You can join it by using `gcc -shared -fPIC -o libmeeting.so program_with_name.c -lpthread`  
> Make sure to bring your name in the meeting function.  

```
vim program_with_name.c
gcc -shared -fPIC -o libmeeting.so program_with_name.c -lpthread
./meeting
```

> Polythread has your concent and name: threadctf{sh00t1ng_s0ME_4TOM1C_FeEt}  
> Good luck, you'll be dyeing the buttons forever now!  
> What did you think was going to happen when you give up your name to fay?  


## 5. princess
Half of the princesses are already dead. So, princesses are processes. Dead princesses are zombies. To get rid of zombies, we need to make the parent take its children or kill the parent, so init will take this child. The parent of a zombie never waited for them. We need to kill the cabin (the parent) of the princess. And never kill the living ones (not zombies). Kill the parent (kill \<ppid\>) of a princess if a princess is `<defunct>`. ($1=pid, $2=ppid, $3=state, $4=command).
```
ps -eo pid,ppid,args
ps -eo pid,ppid,state,comm | awk '$3 == "Z" && $4 ~ /princess/ { print $2 }' | xargs -r kill
```

> === The Forest ===  
> The forest falls silent. 50 cursed cabins lie in ruin, and the story is finally over.  
> Your reward waits in your home directory.  

```
ls -la
cat flag.txt
```
> threadctf{WH4t_d0_y0u_mEaN_th3y_ARe_Alre4Dy_d34D}


## 6. princess2
We cannot use the shell. So, we need to write a C file.
```
vim run.c
gcc run.c
./a.out
```

**run.c**
```run.c
int main(void) {
    for (;;) {
        DIR *d = opendir("/proc");
        struct dirent *e;

        while ((e = readdir(d))) {
            char path[300], comm[256], state;
            int pid, ppid;
            snprintf(path, sizeof path, "/proc/%s/stat", e->d_name);

            FILE *f = fopen(path, "r");
            if (!f) continue;
            if (fscanf(f, "%d (%255[^)]) %c %d", &pid, comm, &state, &ppid) == 4
                && state == 'Z'
                && strstr(comm, "princess")
                && ppid > 1) {
                    kill(ppid, SIGKILL);
                }
            fclose(f);
        }
        closedir(d);
        sleep(1);
    }
}
```

Wait for:
> === The Forest ===  
> The forest falls silent. 100 cursed cabins lie in ruin, and  
> the story is finally over. Your reward waits in your home directory.

```
vim flag.txt
```
> threadctf{i_hope_You_cAN_C_CleARly_n0W}
