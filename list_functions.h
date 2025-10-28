#ifndef LIST_FUNCTIONS_H
#define LIST_FUNCTIONS_H

#include "debug.h"
#include "list_properties.h"

err_t list_ctor(lst* list, size_t size);
err_t allocate_list_memory(lst* list, size_t size);
void list_dtor(lst* list);

#endif
