GCC = gcc -std=c11 -D_DEFAULT_SOURCE -pedantic -Wall -Wextra
OBJ = dirwalk.o
all: dirwalk
dirwalk: $(OBJ)
	$(GCC) $(LDFLAGS) $(OBJ) -o dirwalk
.c.o:
	$(GCC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@
clean:
	rm -f *.o
	rm -f dirwalk
