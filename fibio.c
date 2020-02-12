#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "fib.h"

int main() {
	char buf[64];
	int num;
	while (1) {
		printf("fibcalc> ");
		fgets(buf, 64, stdin);
		if (strncmp(buf, "exit", 64) == 0) {
			return 0;
		} else if ((num = strtol(buf, NULL, 10)) && errno != ERANGE) {
			printf("Fibonacci number %d is %d.\n", num, fib(num));
		} else {
			fprintf(stderr, "An error occurred.");
			return 1;
		}
	}
}
