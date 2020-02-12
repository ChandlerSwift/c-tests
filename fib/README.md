# fib

Fibonacci calculator-based benchmark quick test. Not intended to be
comprehensive, but vaguely effective when comparing similar implementations of
C vs Python. An equivalent Python interpretation is below:

```
#!/usr/bin/env python3

import timeit

def fib(n: int) -> int:
    if n == 1 or n == 2:
        return 1
    return fib(n-1) + fib(n-2)

while True:
    ITERATIONS=1
    nth = int(input("Fibonacci number to calculate: "))
    print(timeit.timeit(lambda: fib(nth), number=ITERATIONS))
```

### Sample results:
```
chandler@debian-dev:~/c-tests/fib$ python3 test.py
Fibonacci number to calculate: 5
3.84499981009867e-06
Fibonacci number to calculate: 10
1.5226998584694229e-05
Fibonacci number to calculate: 15
0.0002734059999056626
Fibonacci number to calculate: 20
0.0016399929991166573
Fibonacci number to calculate: 25
0.016380140999899595
Fibonacci number to calculate: 30
0.1507444979997672
Fibonacci number to calculate: 35
1.4347497620001377
Fibonacci number to calculate: 38
6.473693714999172

chandler@debian-dev:~/c-tests/fib$ for fib in fib-*; do echo $fib:; time ./$fib 38 >/dev/null; done
fib-O0:

real    0m0.165s
user    0m0.164s
sys     0m0.000s
fib-O1:

real    0m0.104s
user    0m0.099s
sys     0m0.005s
fib-O2:

real    0m0.073s
user    0m0.069s
sys     0m0.004s
fib-O3:

real    0m0.075s
user    0m0.074s
sys     0m0.000s
fib-O3-ffast-math:

real    0m0.075s
user    0m0.074s
sys     0m0.000s
```
