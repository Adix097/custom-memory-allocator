#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

void* heap_alloc(size_t size);
void heap_free(void* ptr);

void get_chunks_info(void);

#endif