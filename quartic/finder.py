#!/usr/bin/env python3
import time

start = time.time()
f = 1

while True:
    if f % 20 == 0:
        print(f"Searching at {f} ({time.time() - start} seconds)")
    start = time.time()
    if f % 5 == 0 or f % 2 == 0:
        f += 1
        continue
    for d in range(1, f):
        if d % 5 == 0:
            continue
        for c in range(5, f, 5):
            for b in range(10, f, 10):
                for a in range(b, f, 10):
                    if a**4 + b**4 + c**4 + d**4 == f**4:
                        print(f"({a},{b},{c},{d} => {f})")
    f  += 1
