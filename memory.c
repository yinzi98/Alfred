#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"

static struct mem_controller_tag st_default_controller = {
    // NULL,/* stderr */
    MEM_FAIL_AND_EXIT
};

mem_controller mem_default_controller = &st_default_controller;

void* mem_malloc_func(mem_controller controller, char *filename, int line, size_t size){
    void        *ptr;
    size_t      alloc_size;
    alloc_size = size;
    ptr = malloc(alloc_size);
    return ptr;
}

mem_controller mem_create_controller() {
    mem_controller    p;
    p = mem_malloc_func(&st_default_controller, __FILE__, __LINE__,  sizeof(struct mem_controller_tag));
    *p = st_default_controller;
    return p;
}