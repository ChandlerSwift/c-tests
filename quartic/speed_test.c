#include <stdio.h>
#include <stdlib.h>

int main() {
    unsigned long long f = 1;
    for (int f = 1; f < 65536; f++) { // 65536**4-1 is the max we can guarantee no overflow. 
        if (f > 640)
            break;
        if (f % 20 == 0) {
            printf("Searching at %d\n", f);
        }
        if (f % 5 == 0 || f % 2 == 0) {
            f += 1;
            continue;
        }
        for (unsigned long long d = 1; d < f; d++) {
            if (d % 5 == 0)
                continue;
            for (unsigned long long c = 5; c < f; c += 5) {
                if (c % 2 == d % 2)
                    continue;
                for (unsigned long long b = 10; b < f; b += 10) {
                    for (unsigned long long a = b; a < f; a += 10) {
                        if (a*a*a*a + b*b*b*b + c*c*c*c + d*d*d*d == f*f*f*f)
                            printf("Found (%d, %d, %d, %d) => %d\n", a, b, c, d, f);
                    }
                }
            }
        }
    }
    printf("Exhausted unsigned long long. Find a better compiler.\n");
}
