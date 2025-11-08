#ifndef LIST_FUNCTIONS_H
#define LIST_FUNCTIONS_H

#include "debug.h"
#include "list_properties.h"
#include "list_dump.h"

void initialize_list_log(md_t debug_mode);

err_t list_ctor(lst* list, size_t capacity);

ssize_t insert_after(lst* list, ssize_t pos, lst_t el);
ssize_t insert_before(lst* list, ssize_t pos, lst_t el);
ssize_t push_front(lst* list, lst_t el);
ssize_t push_back(lst* list, lst_t el);
err_t delete_ind(lst* list, ssize_t pos);
err_t clean_up_list(lst* list);
bool is_cleaned_up(lst* list);

size_t get_list_size(lst* list);
size_t get_list_capacity(lst* list);
void set_list_debug_mode(lst* list, md_t mode);
md_t get_list_debug_mode(lst* list);

err_t list_dtor(lst* list);


err_t initialise_vlist(vanilla_list* vlist, md_t debug_mode);
vlist_el* vlist_insert_after(vanilla_list* vlist,  vlist_el* el, lst_t value);
vlist_el* vlist_insert_before(vanilla_list* vlist,  vlist_el* el, lst_t value);
err_t vlist_delete(vanilla_list* vlist, vlist_el* el);
void destroy_vlist(vanilla_list* vlist);


#define ASSERT_CAPACITY() do{                       \
    if (list->size == list->capacity)               \
        if (reallocate_list_memory(list) != ok)     \
            return error;                           \
}                                                   \
while(0)

#ifdef LIST_VERIFICATION

#define VERIFY_LIST(RET) do{                                    \
    if (process_verification(list) != ok)                       \
        return RET;                                             \
}                                                               \
while(0)

#else

#define VERIFY_LIST(RET) do{                                    \
    if (list == NULL)                                           \
        return RET;                                             \
}                                                               \
while(0)

#endif

#define DISPLAY_LIST(HIGHLIGHT) do{                                 \
    if (debug_mode == on)                                           \
        generate_dump_image(list, HIGHLIGHT);                       \
}                                                                   \
while(0)

#ifdef VLIST_VERIFICATION

#define VERIFY_VLIST(RET) do{                                    \
    if (process_vlist_verification(vlist) != ok)                 \
        return RET;                                              \
}                                                                \
while(0)

#else

#define VERIFY_VLIST(RET) do{       \
    if (vlist == NULL)              \
        return RET;                 \
}                                   \
while(0)

#endif

#define DISPLAY_VLIST(HIGHLIGHTED) do{                              \
    if (debug_mode == on)                                           \
        vlist_generate_dump_image(vlist, HIGHLIGHTED);              \
}                                                                   \
while(0)

#endif
