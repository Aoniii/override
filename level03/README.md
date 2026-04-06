## Level03

First, I disassembled the executable and converted it into easy-to-understand C code (see `source.c`).
</br>
There are two functions here: `decrypt` and `test`. The main function calls `test`, which in turn calls `decrypt`.
</br>
It is clear that the test only allows certain values to trigger `decrypt` without randomization.
</br>
We could try a brute-force attack on the few possible combinations, but let's try to understand `decrypt` instead.

```c
void decrypt(int key) {
	char	encrypted[] = "\x51\x7d\x7c\x75\x60\x73\x66\x67\x7e\x73\x66\x7b\x7d\x7c\x61\x33";
	int		len;
	int		i;

	len = strlen(encrypted);
	i = 0;
	while (i < len) {
		encrypted[i] = encrypted[i] ^ key;
		i++;
	}

	if (strncmp(encrypted, "Congratulations!", 17) == 0)
		system("/bin/sh");
	else
		puts("\nInvalid Password");
}
```

The `decrypt` function takes an `encrypted` string as input and uses a decryption algorithm that performs an XOR operation on each character using the argument passed as a parameter by `test`. Furthermore, we can see that the result of this algorithm must be equal to `Congratulations!` in order to execute `/bin/sh`.

Now you can use CyberChef to find the key.
</br>
<<https://gchq.github.io/CyberChef/>>
</br>
Convert hex to characters using `From Hex` then perform an `XOR brute-force`.
</br>
You can see that the key is `0x12`.
</br>
Therefore, `decrypt` must be called with `0x12`.

```c
void test(int input, int reference) {
	int	diff = reference - input;

	switch (diff) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
			decrypt(diff);
			break ;
		default:
			decrypt(rand());
			break ;
	}
}
```

Looking at `test`, we can see that we need to check the value passed to `test` in the `main` function to perform a calculation that will result in `0x12`.

```c
test(password, 0x1337d00d);
```

```bash
$ python -c "print(0x1337d00d - 0x12)"
322424827
```

```bash
$ ./level03
***********************************
*               level03         **
***********************************
Password:322424827
$ cat /home/users/level04/.pass
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
```