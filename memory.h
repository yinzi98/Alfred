#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef union Header_tag Header;
typedef struct mem_controller_tag *mem_controller;

typedef union {
    long    l_dummy;
    double  d_dummy;
    void    *p_dummy;
} Align;

#define MARK_SIZE (4)

typedef struct {
    int         size;
    char        *filename;
    int         line;
    Header      *prev;
    Header      *next;
    unsigned char   mark[MARK_SIZE];
} HeaderStruct;

#define ALIGN_SIZE      (sizeof(Align))
#define revalue_up_align(val)   ((val) ? (((val) - 1) / ALIGN_SIZE + 1) : 0)
#define HEADER_ALIGN_SIZE       (revalue_up_align(sizeof(HeaderStruct)))
#define MARK (0xCD)

union Header_tag {
    HeaderStruct        s;
    Align               u[HEADER_ALIGN_SIZE];
};

typedef enum {
    MEM_FAIL_AND_EXIT,
    MEM_FAIL_AND_RETURN
} MEM_Failmode;

struct mem_controller_tag {
    MEM_Failmode        fail_mode;
    Header      *block_header;
};

mem_controller mem_create_controller();

void *mem_malloc_func(mem_controller controller, char *filename, int line, size_t size);