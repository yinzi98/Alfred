#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"

#ifdef DEBUG
#include "inspection.h"
#endif /* DEBUG */

 /* error handler */
static void error_handler(mem_controller controller, char *filename, int line, char *msg) {
    if (controller->error_fp == NULL) {
        controller->error_fp = stderr;
    }
    controller->error_handler(controller, filename, line, msg);

    if (controller->fail_mode == MEM_FAIL_AND_EXIT) {
        exit(1);
    }
}

static void default_error_handler(mem_controller controller, char *filename, 
                                    int line, char *msg) {
    fprintf(controller->error_fp,
            "MEM:%s failed in %s at %d\n", msg, filename, line);
}

/* init controller */
static struct mem_controller_tag st_default_controller = {
    NULL,/* stderr */
    default_error_handler,
    MEM_FAIL_AND_EXIT
};

mem_controller mem_default_controller = &st_default_controller;

mem_controller mem_create_controller() {
    mem_controller    p;

    p = mem_malloc_func(&st_default_controller, __FILE__, __LINE__,  sizeof(struct mem_controller_tag));
    *p = st_default_controller;

    return p;
}

/* functions of MEM module. */
void* mem_malloc_func(mem_controller controller, char *filename, int line, size_t size) {
    void        *ptr;
    size_t      alloc_size;

#ifdef DEBUG
    alloc_size = size + sizeof(Header) + MARK_SIZE;
#else
    alloc_size = size;
#endif
    ptr = malloc(alloc_size);
    if (ptr == NULL) {
        error_handler(controller, filename, line, "malloc");
    }

#ifdef DEBUG
    memset(ptr, 0xCC, alloc_size);
    set_header(ptr, size, filename, line);
    set_tail(ptr, alloc_size);
    chain_block(controller, (Header*)ptr);
    ptr = (char*)ptr + sizeof(Header);
#endif

    return ptr;
}

void* mem_realloc_func(mem_controller controller, char *filename, int line, void *ptr, size_t size) {
    void        *new_ptr;
    size_t      alloc_size;
    void        *real_ptr;

#ifdef DEBUG
    Header      old_header;
    int         old_size;

    alloc_size = size + sizeof(Header) + MARK_SIZE;
    if (ptr != NULL) {
        real_ptr = (char*)ptr - sizeof(Header);
        check_mark((Header*)real_ptr);
        old_header = *((Header*)real_ptr);
        old_size = old_header.s.size;
        unchain_block(controller, real_ptr);
    } else {
        real_ptr = NULL;
        old_size = 0;
    }
#else
    alloc_size = size;
    real_ptr = ptr;
#endif

    new_ptr = realloc(real_ptr, alloc_size);
    if (new_ptr == NULL) {
        if (ptr == NULL) {
            error_handler(controller, filename, line, "realloc(malloc)");
        } else {
            error_handler(controller, filename, line, "realloc");
            free(real_ptr);
        }
    }

#ifdef DEBUG
    if (ptr) {
        *((Header*)new_ptr) = old_header;
        ((Header*)new_ptr)->s.size = size;
        rechain_block(controller, (Header*)new_ptr);
        set_tail(new_ptr, alloc_size);
    } else {
        set_header(new_ptr, size, filename, line);
        set_tail(new_ptr, alloc_size);
        chain_block(controller, (Header*)new_ptr);
   }
    new_ptr = (char*)new_ptr + sizeof(Header);
    if (size > old_size) {
        memset((char*)new_ptr + old_size, 0xCC, size - old_size);
    }
#endif

    return new_ptr;
}

char *mem_strdup_func(mem_controller controller, char *filename, int line, char *str) {
    char        *ptr;
    int         size;
    size_t      alloc_size;

    size = strlen(str) + 1;
    #ifdef DEBUG
    alloc_size = size + sizeof(Header) + MARK_SIZE;
#else
    alloc_size = size;
#endif
    ptr = malloc(alloc_size);
    if (ptr == NULL) {
        error_handler(controller, filename, line, "strdup");
    }

#ifdef DEBUG
    memset(ptr, 0xCC, alloc_size);
    set_header((Header*)ptr, size, filename, line);
    set_tail(ptr, alloc_size);
    chain_block(controller, (Header*)ptr);
    ptr = (char*)ptr + sizeof(Header);
#endif
    strcpy(ptr, str);

    return ptr;
}

void mem_free_func(mem_controller controller, void *ptr) {
    void        *real_ptr;

#ifdef DEBUG
    int size;
#endif
    if (ptr == NULL)
        return;

#ifdef DEBUG
    real_ptr = (char*)ptr - sizeof(Header);
    check_mark((Header*)real_ptr);
    size = ((Header*)real_ptr)->s.size;
    unchain_block(controller, real_ptr);
    memset(real_ptr, 0xCC, size + sizeof(Header));
#else
    real_ptr = ptr;
#endif

    free(real_ptr);
}

int main() {
    #ifdef DEBUG
    printf("DEBUG\n");
    #endif /* DEBUG */
    return 0;
}