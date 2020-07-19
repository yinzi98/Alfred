#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "storage.h"

mem_storage mem_open_storage_func(mem_controller controller,
                      char *filename, int line, int page_size) {
    mem_storage storage;
    storage = mem_malloc_func(controller, filename, line,
                              sizeof(struct mem_storage_tag));
    storage->page_list = NULL;
    assert(page_size >= 0);
    if (page_size > 0) {
        storage->current_page_size = page_size;
    } else {
        storage->current_page_size = DEFAULT_PAGE_SIZE;
    }

    return storage;
}

void* mem_storage_malloc_func(mem_controller controller,
                        char *filename, int line, MEM_Storage storage,
                        size_t size) {
    int cell_num;
    MemoryPage  *new_page;
    void    *p;

    cell_num = ((size - 1) / CELL_SIZE) + 1;

    if (storage->page_list != NULL
        && (storage->page_list->use_cell_num + cell_num
            < storage->page_list->cell_num)) {
        p = &(storage->page_list->cell[storage->page_list->use_cell_num]);
        storage->page_list->use_cell_num += cell_num;
    } else {
        int alloc_cell_num = cell_num > storage->current_page_size ? 
                            cell_num : storage->current_page_size;

        new_page = mem_malloc_func(controller, filename, line,
                                   sizeof(MemoryPage)
                                   + CELL_SIZE * (alloc_cell_num - 1));
        new_page->next = storage->page_list;
        new_page->cell_num = alloc_cell_num;
        storage->page_list = new_page;

        p = &(new_page->cell[0]);
        new_page->use_cell_num = cell_num;
    }

    return p;
}

void mem_dispose_storage_func(mem_controller controller, mem_storage storage) {
    MemoryPage  *temp;

    while (storage->page_list) {
        temp = storage->page_list->next;
        mem_free_func(controller, storage->page_list);
        storage->page_list = temp;
    }
    mem_free_func(controller, storage);
}
