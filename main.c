#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

#define HEAP_CAPACITY 1024*1024
#define CHUNK_LIST_CAPACITY 1024

typedef struct {
    void* start;
    size_t size;
} chunk;

typedef struct {
    size_t count;
    chunk chunks[CHUNK_LIST_CAPACITY];
} chunk_list;

char heap[HEAP_CAPACITY] = {0};
size_t heap_size = 0;

chunk_list allocated_chunks = {0};
chunk_list freed_chunks = {0};

// ---------------------------------
void* heap_alloc(size_t size);
void get_chunks_info(void);
void heap_free(void* ptr);

// ---------------------------------
chunk_list_insert(chunk_list* list, void* ptr, size_t size);
chunk_list_find(const chunk_list* list, size_t size);
chunk_list_delete(chunk_list* list, size_t size);


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
    void* ptr = heap + heap_size;
    heap_size += size;

    chunk_list_insert(&allocated_chunks, ptr, size);
    return ptr;
}

void get_chunks_info(void) {
    if (allocated_chunks.count > 0) {
        printf("--------------allocated chunks--------------\n");
        for (size_t i = 0; i < allocated_chunks.count; ++i) {
            printf("start: %p, size: %zu\n", allocated_chunks.chunks[i].start, allocated_chunks.chunks[i].size);
        }
    }

    if (freed_chunks.count > 0) {
        printf("--------------freed chunks--------------\n");
        for (size_t i = 0; i < freed_chunks.count; ++i) {
            printf("start: %p, size: %zu\n", freed_chunks.chunks[i].start, freed_chunks.chunks[i].size);
        }
    }
}

void heap_free(void* ptr) {
    
}