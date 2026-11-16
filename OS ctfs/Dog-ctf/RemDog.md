Folder  `Doguments/unsorted` contains a list of files, each of them one of two types - angry dog, or good dog. The difference is in file size - the good dog is smaller (1.7 KiB), and the angry one is bigger (1.9KiB).

We use `find . -type f -size +1843c -delete` to remove all the files with size > 1.8