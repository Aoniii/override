## Level06

First, I disassembled the executable and converted it into easy-to-understand C code (see `source.c`).
</br>
Now that we've reproduced the source code, we can see that the program requires two inputs: the first is a `login`, and the second is an `integer value`.
</br>
The program hashes the login; if it matches the integer value, `/bin/sh` is called.



```c
printf("Hash: %u\n", hash);
```

We add a `printf` statement before the function returns to display the hash.

```bash
$ gcc -m32 source.c
$ ./a.out
***********************************
*               level06           *
***********************************
-> Enter Login: snourry
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 1
Hash: 6233772
```

We can see that for `snourry`, the hash is 6233772.

```bash
$ ./level06
***********************************
*               level06           *
***********************************
-> Enter Login: snourry
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 6233772
Authenticated!
```