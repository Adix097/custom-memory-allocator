#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define HEAP_CAPACITY 1024*1024
#define CHUNK_LIST_CAPACITY 1024

typedef struct {
    void* ptr;
    size_t size;
} chunk;

typedef struct {
    size_t count;
    chunk chunks[CHUNK_LIST_CAPACITY];
} chunk_list;

char heap[HEAP_CAPACITY] = {0};

chunk_list allocated_chunks = {0};
chunk_list freed_chunks = {
    .count = 1,
    .chunks = { [0] = {.ptr = heap, .size = sizeof(heap),} },
};

// ---------------------------------
void warning(const char* message);
void error(const char* message);

// ---------------------------------
void* heap_alloc(size_t size);
void get_chunks_info(void);
void heap_free(void* ptr);

// ---------------------------------
void chunk_list_insert(chunk_list* list, chunk c);
int chunk_list_find(const chunk_list* list, void* ptr);
void chunk_list_delete(chunk_list* list, size_t index);


int main() {
    for (size_t i = 0; i < 10; ++i) {
        char* p = heap_alloc(i);
        if (i % 2 == 0) {
            heap_free(p);
        }
    }

    get_chunks_info();

    return 0;
}

void warning(const char* message) {
    fprintf(stderr, "\033[33mwarning: %s\033[0m\n", message);
}

void error(const char* message) {
    fprintf(stderr, "\033[31merror: %s\033[0m\n", message);
}

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

void heap_free(void* ptr) {
    if (ptr == NULL) return;

    int index = chunk_list_find(&allocated_chunks, ptr);

    if (index == -1) {
        error("attempted to free unknown pointer");
        return;
    }

    chunk_list_insert(&freed_chunks, allocated_chunks.chunks[index]);
    chunk_list_delete(&allocated_chunks, index);
}

void chunk_list_insert(chunk_list* list, chunk c) {
    assert(list->count < CHUNK_LIST_CAPACITY);

    list->chunks[list->count].ptr = c.ptr;
    list->chunks[list->count].size = c.size;

    // put the chunks in sorted order based on ptr
    for (size_t i = list->count; i > 0 && list->chunks[i].ptr < list->chunks[i - 1].ptr; --i) {
        const chunk tmp = list->chunks[i];
        list->chunks[i] = list->chunks[i - 1];
        list->chunks[i - 1] = tmp;
    }

    list->count++;
}

int chunk_compare(const void* a, const void* b) {
    const chunk* first = a;
    const chunk* second = b;
    return (char*)first->ptr - (char*)second->ptr;
}

int chunk_list_find(const chunk_list* list, void* ptr) {
    // returns the index of chunk

    chunk key = {
        .ptr = ptr,
    };

    chunk* result = (chunk*)bsearch(&key, list->chunks, list->count, sizeof(list->chunks[0]), chunk_compare);
    if (result == NULL) return -1;

    return (result - list->chunks);
}

void chunk_list_delete(chunk_list* list, size_t index) {
    assert(index < list->count);

    for (size_t i = index; i + 1 < list->count; ++i) {
        list->chunks[i] = list->chunks[i + 1];
    }

    list->count--;
}