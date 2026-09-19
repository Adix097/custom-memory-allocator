#ifndef CHUNK_H
#define CHUNK_H

#include <stddef.h>

typedef struct {
    void* ptr;
    size_t size;
} chunk;

#endif