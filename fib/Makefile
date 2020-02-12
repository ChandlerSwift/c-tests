ALL: O0 O1 O2 O3 O3-ffast-math

O0:
	gcc -o fib-O0 -O0 src/fib.c

O1:
	gcc -o fib-O1 -O1 src/fib.c

O2:
	gcc -o fib-O2 -O2 src/fib.c

O3:
	gcc -o fib-O3 -O3 src/fib.c

O3-ffast-math:
	gcc -o fib-O3-ffast-math -O3 -ffast-math src/fib.c

clean:
	rm fib-*
