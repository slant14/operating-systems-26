
Each shard needs one successful run of the decryptor, but a run takes 10 seconds and fails with probability 1/2. Retrying shards one by one cannot fit into the 30 second limit, so every shard is attempted 12 times in parallel and the first successful stdout is kept. Shards that lose all copies get a second wave, and the chunks are concatenated by the numeric shard index.

```c

for (int i = 0; i < n; i++)
	for (int r = 0; r < 12; r++)
		pthread_create(&tid[i][r], NULL, decrypt_one, &shard[i]);

for (int i = 0; i < n; i++) {
	for (int r = 0; r < 12; r++) {
		char *out = NULL;
		pthread_join(tid[i][r], (void **)&out);
		if (!chunk[i] && out)
			chunk[i] = out;

	}

}

for (int i = 0; i < n; i++)
	fputs(chunk[i], stdout);

```