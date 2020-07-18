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

void* mem_realloc_func(mem_controller controller, char *filename, int line, void *ptr, size_t size) {
    void        *new_ptr;
    size_t      alloc_size;
    void        *real_ptr;

    alloc_size = size;
    real_ptr = ptr;
    new_ptr = realloc(real_ptr, alloc_size);

    return new_ptr;
}

char *mem_strdup_func(mem_controller controller, char *filename, int line, char *str) {
    char        *ptr;
    int         size;
    size_t      alloc_size;

    size = strlen(str) + 1;
    ptr = malloc(alloc_size);
    strcpy(ptr, str);

    return ptr;
}

void mem_free_func(mem_controller controller, void *ptr) {
    void        *real_ptr;

    real_ptr = ptr;

    free(real_ptr);
}