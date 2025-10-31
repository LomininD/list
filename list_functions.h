#ifndef LIST_FUNCTIONS_H
#define LIST_FUNCTIONS_H

#include "debug.h"
#include "list_properties.h"

err_t list_ctor(lst* list, size_t capacity);
err_t allocate_list_memory(lst* list, size_t capacity);
err_t reallocate_list_memory(lst* list);

ssize_t insert_after(lst* list, ssize_t pos, lst_t el);
ssize_t insert_before(lst* list, ssize_t pos, lst_t el);
err_t delete_ind(lst* list, ssize_t pos);

size_t get_list_size(lst* list);
size_t get_list_capacity(lst* list);

void list_dtor(lst* list);
void initialize_log(md_t debug_mode);

#endif
