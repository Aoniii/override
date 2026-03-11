## Level00

The executable is waiting for a password.

```bash
$ ./level00
***********************************
*            -Level00 -           *
***********************************
Password:1234

Invalid Password!
```

Several things can be seen in the ``main`` of this program, such as a call to a ``system``. It would be necessary to know what the call system is and how to make the call.

```x86asm
$ objdump -d ./level00 -M intel | sed -n '/<main>:/,/^$/p'
08048494 <main>:
 8048494:       55                      push   ebp
 8048495:       89 e5                   mov    ebp,esp
 8048497:       83 e4 f0                and    esp,0xfffffff0
 804849a:       83 ec 20                sub    esp,0x20
 804849d:       c7 04 24 f0 85 04 08    mov    DWORD PTR [esp],0x80485f0
 80484a4:       e8 e7 fe ff ff          call   8048390 <puts@plt>
 80484a9:       c7 04 24 14 86 04 08    mov    DWORD PTR [esp],0x8048614
 80484b0:       e8 db fe ff ff          call   8048390 <puts@plt>
 80484b5:       c7 04 24 f0 85 04 08    mov    DWORD PTR [esp],0x80485f0
 80484bc:       e8 cf fe ff ff          call   8048390 <puts@plt>
 80484c1:       b8 2c 86 04 08          mov    eax,0x804862c
 80484c6:       89 04 24                mov    DWORD PTR [esp],eax
 80484c9:       e8 b2 fe ff ff          call   8048380 <printf@plt>
 80484ce:       b8 36 86 04 08          mov    eax,0x8048636
 80484d3:       8d 54 24 1c             lea    edx,[esp+0x1c]
 80484d7:       89 54 24 04             mov    DWORD PTR [esp+0x4],edx
 80484db:       89 04 24                mov    DWORD PTR [esp],eax
 80484de:       e8 ed fe ff ff          call   80483d0 <__isoc99_scanf@plt>
 80484e3:       8b 44 24 1c             mov    eax,DWORD PTR [esp+0x1c]
 80484e7:       3d 9c 14 00 00          cmp    eax,0x149c
 80484ec:       75 1f                   jne    804850d <main+0x79>
 80484ee:       c7 04 24 39 86 04 08    mov    DWORD PTR [esp],0x8048639
 80484f5:       e8 96 fe ff ff          call   8048390 <puts@plt>
 80484fa:       c7 04 24 49 86 04 08    mov    DWORD PTR [esp],0x8048649
 8048501:       e8 9a fe ff ff          call   80483a0 <system@plt>
 8048506:       b8 00 00 00 00          mov    eax,0x0
 804850b:       eb 11                   jmp    804851e <main+0x8a>
 804850d:       c7 04 24 51 86 04 08    mov    DWORD PTR [esp],0x8048651
 8048514:       e8 77 fe ff ff          call   8048390 <puts@plt>
 8048519:       b8 01 00 00 00          mov    eax,0x1
 804851e:       c9                      leave
 804851f:       c3                      ret
```

``system`` is directly called with ``/bin/sh``.

```gdb
$ gdb ./level00
(gdb) b main
Breakpoint 1 at 0x8048497
(gdb) r
Starting program: /home/users/level00/level00

Breakpoint 1, 0x08048497 in main ()
(gdb) x/s 0x8048649
0x8048649:       "/bin/sh"
```

To reach the call to ``system``, the value sent at the time of ``scanf`` must be different from ``0x149c``.

```x86asm
 80484de:       e8 ed fe ff ff          call   80483d0 <__isoc99_scanf@plt>
 80484e3:       8b 44 24 1c             mov    eax,DWORD PTR [esp+0x1c]
 80484e7:       3d 9c 14 00 00          cmp    eax,0x149c
 80484ec:       75 1f                   jne    804850d <main+0x79>
 80484ee:       c7 04 24 39 86 04 08    mov    DWORD PTR [esp],0x8048639
 80484f5:       e8 96 fe ff ff          call   8048390 <puts@plt>
 80484fa:       c7 04 24 49 86 04 08    mov    DWORD PTR [esp],0x8048649
 8048501:       e8 9a fe ff ff          call   80483a0 <system@plt>
```

0x149c = 5276.

```bash
$ ./level00
***********************************
*            -Level00 -           *
***********************************
Password:5276

Authenticated!
$ cat /home/users/level01/.pass
uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL
```
