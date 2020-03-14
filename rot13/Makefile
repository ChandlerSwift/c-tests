.PHONY: rot13
rot13:
	gcc -o rot13 rot13.c

install: rot13
	mkdir -p $(HOME)/bin; cp rot13 $(HOME)/bin/rot13

clean:
	rm rot13
