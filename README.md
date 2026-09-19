# Custom Memory Allocator

A small custom memory allocator written in C.

The project implements a simplified `malloc()` / `free()`-style allocator over a statically allocated heap. It maintains separate lists of allocated and free memory chunks and supports allocation, freeing, splitting, searching, and coalescing of memory blocks.

Instead of asking the operating system for memory for every allocation, this allocator manages a fixed region of memory:

```c
#define HEAP_CAPACITY (1024 * 1024) // 1 MiB
static char heap[HEAP_CAPACITY];
```

The allocator divides this region into **chunks** and maintains bookkeeping information about which portions are allocated and which are available.

The current implementation uses a **first-fit free-list strategy**:

1. Search the free chunks from the beginning.
2. Find the first chunk large enough for the requested allocation.
3. Remove it from the free list.
4. Split the chunk if it is larger than required.
5. Record the allocated chunk.
6. When memory is freed, return the chunk to the free list.
7. Coalesce adjacent free chunks to reduce fragmentation.

Conceptually:

```text
Initial heap

┌──────────────────────────────────────────────────────────┐
│                         FREE                             │
└──────────────────────────────────────────────────────────┘

              malloc(100)

┌──────────────────────┬───────────────────────────────────┐
│       ALLOCATED      │               FREE                │
│       100 bytes      │                                   │
└──────────────────────┴───────────────────────────────────┘

              malloc(50)

┌──────────────────────┬───────────────┬───────────────────┐
│       ALLOCATED      │   ALLOCATED   │       FREE        │
│       100 bytes      │    50 bytes   │                   │
└──────────────────────┴───────────────┴───────────────────┘

              free(first block)

┌──────────────────────┬───────────────┬───────────────────┐
│         FREE         │   ALLOCATED   │       FREE        │
│       100 bytes      │    50 bytes   │                   │
└──────────────────────┴───────────────┴───────────────────┘
```

If two free chunks become physically adjacent, they are merged:

```text
┌──────────────┬──────────────┐
│   FREE 100   │   FREE 200   │
└──────────────┴──────────────┘

              ↓ coalesce

┌─────────────────────────────┐
│          FREE 300           │
└─────────────────────────────┘
```

## Project Structure

```text
.
├── include/
│   ├── allocator.h
│   ├── chunk.h
│   ├── chunk_list.h
│   └── log.h
│
├── src/
│   ├── allocator.c
│   ├── chunk_list.c
│   ├── log.c
│   └── main.c
│
├── build/
├── docs/
├── Makefile
└── README.md
```

### Responsibilities

| Component           | Responsibility                        |
| ------------------- | ------------------------------------- |
| `allocator.c`       | Allocation and deallocation logic     |
| `chunk.c / chunk.h` | Representation of a memory chunk      |
| `chunk_list.c`      | Chunk-list operations                 |
| `log.c`             | Warnings and errors                   |
| `main.c`            | Test/demo program                     |
| `Makefile`          | Build, run and debug automation       |
| `docs/`             | Detailed documentation                |

for more detailed explaination check out: `docs/`