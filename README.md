# simple-crypto

Simple C lib of the MD5 & TEA algorithm

# Performance
Benchmark a `3.5MB` file comparing with `md5` command in `MacOS Mojave` by running three times `time` command then calculating their average.
```bash
# system command: total 22ms
real    0m0.015s
user    0m0.012s
sys     0m0.005s

# our md5 command: total 39ms
# build by
gcc md5.c -o md5 -O3 -DTEST_SIMPLE_CRYPTO

real    0m0.021s
user    0m0.013s
sys     0m0.005s
```
Our version is `17ms` slower than what system does.

Benchmark a `481.7MB` file comparing with `md5` command in `MacOS Mojave` by running three times `time` command then calculating their average.
```bash
# system command: total 2079ms
real    0m0.980s
user    0m0.964s
sys     0m0.135s

# our md5 command: total 2499ms
# build by
gcc md5.c -o md5 -O3 -DTEST_SIMPLE_CRYPTO

real    0m1.253s
user    0m1.003s
sys     0m0.243s
```
Our version is `400ms` slower than what system does.

# Install

```bash
git clone https://github.com/fumiama/simple-crypto.git
cd simple-crypto
mkdir build
cd build
cmake ..
make
make install
```

# Usage

1. Include `simplecrypto.h` in your c program.
```c
#include <simplecrypto.h>
```
2. Call functions.

> see all functions in simplecrypto.h
