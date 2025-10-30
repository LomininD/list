#ifndef LIST_FUNCTIONS_H
#define LIST_FUNCTIONS_H

#include "debug.h"
#include "list_properties.h"

err_t list_ctor(lst* list, ssize_t capacity);
err_t allocate_list_memory(lst* list, ssize_t capacity);
ssize_t insert_after(lst* list, ssize_t pos, lst_t el);
ssize_t insert_before(lst* list, ssize_t pos, lst_t el);
err_t delete_ind(lst* list, ssize_t pos);
void list_dtor(lst* list);
void initialize_log(md_t debug_mode);

#endif
