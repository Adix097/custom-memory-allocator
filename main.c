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
void get_chunks_info(void);

int main() {
    for (size_t i = 0; i < 1024; ++i) {
        heap_alloc(i);
    }

    get_chunks_info();

    return 0;
}

void* heap_alloc(size_t size) {
    // allocating zero sized chunk basically allocated returns a pointer 
    // which is freely available for the allocator to write
    // either way following the standard malloc behaviour this code returns NULL on size 0
    // otherwise the allocator would still recogonize chunk of size 0 as a valid chunk
    if (size == 0) {
        fprintf(stderr, "\033[33mwarning: allocating a chunk of size 0\033[0m\n");
        return NULL;
    }

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

void get_chunks_info(void) {
    printf("--------------allocated chunks--------------\n");
    for (size_t i = 0; i < heap_allocated_size; ++i) {
        printf("start: %p, size: %zu\n", heap_allocated_chucks[i].start, heap_allocated_chucks[i].size);
    }
}