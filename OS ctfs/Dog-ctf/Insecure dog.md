In the `Doguments/secret_stuff` there is a remember.txt and `super_secret` folder, which contains a photo of a dog. We know, that the file name is 4 letters long, and consists only of D O and G. 

For this task I wrote a script:
```bash
#!/bin/bash

for i in 'D' 'O' 'G'; do
	for j in 'D' 'O' 'G'; do
		for k in 'D' 'O' 'G'; do
			cat super_secret/$i$i$j$k.png | head -n 1
			cat super_secret/$i$j$j$k.png | head -n 1
			cat super_secret/$i$j$k$k.png | head -n 1
		done
	done
done
```

which printed a png's start instead of a "Not found" error on the correct name/

Using the results, I managed to figure out the name - `OGDD.png`