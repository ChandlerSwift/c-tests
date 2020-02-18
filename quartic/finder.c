#include <stdio.h>
#include <stdlib.h>

int main() {
	// Limit search space to 65536. 65536**4 - 1 is the max we can guarantee no overflow, as ULL's are guaranteed to be at least 64 bits.
	for (unsigned long long f = 1; f < 65536ull; f++) {
		if (f % 20 == 0) {
			printf("Searching at %d\n", f);
		}
		if (f % 5 == 0 || f % 2 == 0) {
			continue;
		}
		for (unsigned long long d = 1; d < f; d++) {
			if (d % 5 == 0) {
				continue;
			}
			for (unsigned long long c = 5; c < f; c += 5) {
				if (c % 2 == d % 2) {
					continue;
				}
				for (unsigned long long b = 10; b < f; b += 10) {
					for (unsigned long long a = b; a < f; a += 10) {
						if (a*a*a*a + b*b*b*b + c*c*c*c + d*d*d*d == f*f*f*f) {
							printf("Found (%d, %d, %d, %d) => %d\n", a, b, c, d, f);
						}
					}
				}
			}
		}
	}
	printf("Exhausted unsigned long long. Find a better compiler.\n");
}
