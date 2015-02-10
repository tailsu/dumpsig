DumpSig
--------

Dump the strong name signature of an assembly.

Usage: `dumpsig <path to assembly>`

Example output for a delay-signed assembly:

```
Strong name signature (128 bytes):
 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0
```

Example output for a regular signed assembly:

```
Strong name signature (128 bytes):
a7 e6 e8 e3 42 c3 55 92 be ae ff e4  7 43 a8 d5
25 cd b8 d7 9d 8f 88 1d d3 68 8e b9 31 b0 bf 3c
24 3a b2 9b 70 91 bb  e 27 98 1e a1 84 e6 90 6f
d5 48 e0 df 43 d8 7c 37 6e 20 ae 5f 61 4c 83 2e
ce 9b 2f 26 87 3c d6 32 2e a7 c5  a 4a ed 4d 43
fd db b0 30 7e cc 7d ac af 1e  8  1 88  1 4c 9e
17 97 29 9b ff  d 87 27  0 8d e6 4f 86 2c 70 d7
15 83 31  4 5c 73 58 b1 a5 f8 c2 cd ae 9d 2f 9c
```

Based on [Finding the Raw Strong Name Signature](http://blogs.msdn.com/b/shawnfa/archive/2005/01/26/361109.aspx) by [ShawnFa](https://social.msdn.microsoft.com/profile/shawnfa/).