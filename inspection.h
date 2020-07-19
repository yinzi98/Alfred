#pragma once

#include "memory.h"

static void chain_block(mem_controller controller, Header *new_header) {
    if (controller->block_header) {
        controller->block_header->s.prev = new_header;
    }
    new_header->s.prev = NULL;
    new_header->s.next = controller->block_header;
    controller->block_header = new_header;
}

static void rechain_block(mem_controller controller, Header *header) {
    if (header->s.prev) {
        header->s.prev->s.next = header;
    } else {
        controller->block_header = header;
    }
    if (header->s.next) {
        header->s.next->s.prev = header;
    }
}

static void unchain_block(mem_controller controller, Header *header) {
    if (header->s.prev) {
        header->s.prev->s.next = header->s.next;
    } else {
        controller->block_header = header->s.next;
    }
    if (header->s.next) {
        header->s.next->s.prev = header->s.prev;
    }
}

void set_header(Header *header, int size, char *filename, int line) {
    header->s.size = size;
    header->s.filename = filename;
    header->s.line = line;
    memset(header->s.mark, MARK, (char*)&header[1] - (char*)header->s.mark);
}

void set_tail(void *ptr, int alloc_size) {
    char *tail;
    tail = ((char*)ptr) + alloc_size - MARK_SIZE;
    memset(tail, MARK, MARK_SIZE);
}

void check_mark_sub(unsigned char *mark, int size) {
    for (int i = 0; i < size; i++) {
        if (mark[i] != MARK) {
            fprintf(stderr, "bad mark\n");
            abort();
        }
    }
}

void check_mark(Header *header) {
    unsigned char        *tail;
    check_mark_sub(header->s.mark, (char*)&header[1] - (char*)header->s.mark);
    tail = ((unsigned char*)header) + header->s.size + sizeof(Header);
    check_mark_sub(tail, MARK_SIZE);
}