#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "memory.h"

typedef union {
    long        l_dummy;
    double      d_dummy;
    void        *p_dummy;
} Cell;

#define CELL_SIZE               (sizeof(Cell))
#define DEFAULT_PAGE_SIZE       (1024)  /* cell num */

typedef struct MemoryPage_tag MemoryPage;
typedef MemoryPage *MemoryPageList;

struct MemoryPage_tag {
    int                 cell_num;
    int                 use_cell_num;
    MemoryPageList      next;
    Cell                cell[1];
};

struct mem_storage_tag {
    MemoryPageList      page_list;
    int                 current_page_size;
};

mem_storage mem_open_storage_func(mem_controller controller,
                                  char *filename, int line, int page_size);

void *mem_storage_malloc_func(mem_controller controller, char *filename, int line,
                                mem_storage storage, size_t size);

void mem_dispose_storage_func(mem_controller controller, mem_storage storage);
