#ifndef LIST_FUNCTIONS_H
#define LIST_FUNCTIONS_H

#include "debug.h"
#include "list_properties.h"
#include "list_dump.h"

err_t list_ctor(lst* list, size_t capacity);

ssize_t insert_after(lst* list, ssize_t pos, lst_t el);
ssize_t insert_before(lst* list, ssize_t pos, lst_t el);
err_t delete_ind(lst* list, ssize_t pos);

size_t get_list_size(lst* list);
size_t get_list_capacity(lst* list);
void set_list_debug_mode(lst* list, md_t mode);
void set_list_verification(lst* list, md_t mode);
md_t get_list_debug_mode(lst* list);


err_t list_dtor(lst* list);
void initialize_log(md_t debug_mode);


#define ASSERT_CAPACITY() do{                       \
    if (list->size == list->capacity)               \
        if (reallocate_list_memory(list) != ok)     \
            return error;                           \
}                                                   \
while(0)

#define VERIFY_LIST() do{                                           \
    if (list->verification == on)                                   \
        if (process_verification(list) != ok)                       \
            return error;                                           \
}                                                                   \
while(0)

#define DISPLAY_LIST() do{                                          \
    if (debug_mode == on)                                           \
        generate_dump_image(list);                                  \
}                                                                   \
while(0)

#endif
