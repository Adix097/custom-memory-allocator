CC = gcc
GDB = gdb

CFLAGS = -Wall -Wextra -std=c11 -pedantic -ggdb
CPPFLAGS = -Iinclude

TARGET = build/heap.out

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.o)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

build/%.o: src/%.c | build
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

run: $(TARGET)
	./$(TARGET)

debug: $(TARGET)
	$(GDB) -tui ./$(TARGET)

clean:
	rm -rf build