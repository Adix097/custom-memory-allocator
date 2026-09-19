#include <stdlib.h>
#include <stdio.h>
#include <allocator.h>
#include <chunk_list.h>
#include <log.h>

#define HEAP_CAPACITY 1024*1024
#define CHUNK_LIST_CAPACITY 1024

static char heap[HEAP_CAPACITY] = {0};

static chunk_list allocated_chunks = {0};
static chunk_list freed_chunks = {
    .count = 1,
    .chunks = { [0] = {.ptr = heap, .size = sizeof(heap),} },
};

void* heap_alloc(size_t size) {
    // allocating zero sized chunk basically allocated returns a pointer 
    // which is freely available for the allocator to write
    // either way following the standard malloc behaviour this code returns NULL on size 0
    // otherwise the allocator would still recogonize chunk of size 0 as a valid chunk
    if (size == 0) {
        warning("allocating a chunk of size 0");
        return NULL;
    }

    for (size_t i = 0; i < freed_chunks.count; ++i) {
        // because the chunks are stored in stored order
        // when we find the chunk just enough for our we found the perfect chunk
        const chunk c = freed_chunks.chunks[i];
        if (c.size >= size) {
            chunk_list_delete(&freed_chunks, i);
            chunk_list_insert(&allocated_chunks, (chunk){.ptr = c.ptr, .size = size});

            // split the unused part of the chunk into a new chunk and insert it back into freed chunks
            if (c.size > size) {
                chunk_list_insert(&freed_chunks, (chunk){.ptr = (char*)c.ptr + size, .size = c.size - size});
            }

            return c.ptr;
        }
    }

    return NULL;
}

void heap_free(void* ptr) {
    if (ptr == NULL) return;

    int index = chunk_list_find(&allocated_chunks, ptr);

    if (index == -1) {
        error("attempted to free unknown pointer");
        exit(1);
    }

    chunk_list_insert(&freed_chunks, allocated_chunks.chunks[index]);
    chunk_list_delete(&allocated_chunks, index);
    chunk_list_coalesce(&freed_chunks);
}

void get_chunks_info(void) {
    if (allocated_chunks.count > 0) {
        printf("--------------allocated chunks--------------\n");
        for (size_t i = 0; i < allocated_chunks.count; ++i) {
            printf("ptr: %p, size: %zu\n", allocated_chunks.chunks[i].ptr, allocated_chunks.chunks[i].size);
        }
    }

    if (freed_chunks.count > 0) {
        printf("--------------free chunks--------------\n");
        for (size_t i = 0; i < freed_chunks.count; ++i) {
            printf("ptr: %p, size: %zu\n", freed_chunks.chunks[i].ptr, freed_chunks.chunks[i].size);
        }
    }
}