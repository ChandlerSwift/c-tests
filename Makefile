all: fib fibio quartic rot13

.PHONY: all fib fibio quartic rot13

fib:
	$(MAKE) -C fib

fibio:
	$(MAKE) -C fibio

quartic:
	$(MAKE) -C quartic

rot13:
	$(MAKE) -C rot13
