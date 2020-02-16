# Quartic Pythagorean quintuple finder.

3^2 + 4^2 == 5^2. 3^3 + 4^3 + 5^3 == 6^3. What values can we find of `a`, `b`,
`c`, `d`, and `f` such that a^4 + b^4 + c^4 + d^4 == f^4?

Python as "executable pseudocode", C for a ~100x speedup. Compiling with -O3
speeds up a further 4x.

### Found combinations
```
30, 120, 272, 315 => 353
240, 340, 430, 599 => 651
```
as well as some duplicates we didn't eliminate, and non-primitive solutions
(solutions where all numbers share a common factor).
