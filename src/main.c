#include <stddef.h>
#include "allocator.h"

int main(void) {
    for (size_t i = 0; i < 10; ++i) {
        char* p = heap_alloc(i);

        if (i % 2 == 0) {
            heap_free(p);
        }
    }

    get_chunks_info();

    return 0;
}