#ifndef CHUNK_LIST_H
#define CHUNK_LIST_H

#include "chunk.h"

#define CHUNK_LIST_CAPACITY 1024

typedef struct {
    size_t count;
    chunk chunks[CHUNK_LIST_CAPACITY];
} chunk_list;

void chunk_list_insert(chunk_list* list, chunk c);
int chunk_list_find(const chunk_list* list, void* ptr);
void chunk_list_delete(chunk_list* list, size_t index);
void chunk_list_coalesce(chunk_list* list);

#endif