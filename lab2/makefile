GCC = gcc -c -W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic

.PHONY: all
all: start

start:	parent.o child.o
	$(GCC) parent.o -o parent
	$(GCC) child.o -o child
	
parent.o: parent.c
	$(GCC) $(CFLAGS) parent.c
	
child.o: child.c
	$(GCC) $(CFLAGS) child.c

clean:
	rm -rf *.o parent child	
