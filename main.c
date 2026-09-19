#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

#define HEAP_CAPACITY 1024*1024
#define HEAP_ALLOCATED_CAPACITY 1024

typedef struct {
    void* start;
    size_t size;
} heap_chuck;

char heap[HEAP_CAPACITY] = {0};
heap_chuck heap_allocated_chucks[HEAP_ALLOCATED_CAPACITY];
size_t heap_size = 0;
size_t heap_allocated_size = 0;

// ---------------------------------
void* heap_alloc(size_t size);

int main() {
    char* root = heap_alloc(26);

    for (size_t i = 0; i < 26; ++i) {
        root[i] = i + 'A';
    }

    return 0;
}

void* heap_alloc(size_t size) {
    assert(heap_size + size <= HEAP_CAPACITY);
    void* result = heap + heap_size;
    heap_size += size;

    const heap_chuck chuck = {
        .start = result,
        .size = size,
    };
    assert(heap_allocated_size < HEAP_ALLOCATED_CAPACITY);
    heap_allocated_chucks[heap_allocated_size++] = chuck;

    return result;
}