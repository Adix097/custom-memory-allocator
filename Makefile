CC = gcc
GDB = gdb
CFLAGS = -Wall -Wextra -std=c11 -pedantic -ggdb

heap.out: main.c
	$(CC) $(CFLAGS) -o heap.out main.c

run: heap.out
	./heap.out

debug: heap.out
	$(GDB) -tui ./heap.out

clean:
	rm -f *.out