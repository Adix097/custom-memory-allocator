#include <stdlib.h>
#include <assert.h>
#include <chunk_list.h>

// static encapsulates the function within this file only
// nothing outside chunk_list.c nows this function exists
static int chunk_compare(const void* a, const void* b) {
    const chunk* first = a;
    const chunk* second = b;
    return (char*)first->ptr - (char*)second->ptr;
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

int chunk_list_find(const chunk_list* list, void* ptr) {
    // returns the index of chunk
    chunk key = {
        .ptr = ptr,
    };

    chunk* result = (chunk*)bsearch(&key, list->chunks, list->count, sizeof(list->chunks[0]), chunk_compare);

    return (result == NULL) ? -1 : (result - list->chunks);
}

void chunk_list_delete(chunk_list* list, size_t index) {
    assert(index < list->count);

    for (size_t i = index; i + 1 < list->count; ++i) {
        list->chunks[i] = list->chunks[i + 1];
    }

    list->count--;
}

void chunk_list_coalesce(chunk_list* list) {
    // marge consecutive free chunks into one free chunk
    // if the pointer to a chunk + the size == the pointer to chunk after that they can be merged
    // to merge them simply add their sizes and delete one of them
    size_t i = 0;
    
    while (i + 1 < list->count) {
        chunk* current = &list->chunks[i];
        chunk* next = &list->chunks[i + 1];
        
        if ((char*)current->ptr + current->size != next->ptr) {
            i++;
            continue;
        }
        
        // do not increment i in case of merge because more than two adjacent free chunks could be present
        current->size += next->size;
        chunk_list_delete(list, i + 1);
    }
}