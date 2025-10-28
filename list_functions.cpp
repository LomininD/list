#include "list_functions.h"
#include <assert.h>


err_t list_ctor(lst* list, size_t size)
{
    assert(list != NULL);

    md_t debug_mode = list->debug_mode;

    printf_log_msg(debug_mode, "list_ctor: began initialization\n");

    list->size = size;

    if (allocate_list_memory(list, size) != ok)
        return error;

    list->head_pos = 0;
    list->tail_pos = 0;
    list->free_pos = 1;
    list->current_pos = 0;

    printf("list_ctor: initialization completed\n");
    return ok;
}


err_t allocate_list_memory(lst* list, size_t size)
{
    assert(list != NULL);

    md_t debug_mode = list->debug_mode;
    
    list->data = (lst_t*) calloc(size + 1, sizeof(lst_t));
    list->ind_arr.next = (size_t*) calloc(size + 1, sizeof(size_t));
    list->ind_arr.prev = (size_t*) calloc(size + 1, sizeof(size_t));

    if (list->data == NULL || list->ind_arr.next == NULL || list->ind_arr.prev == NULL)
    {
        printf_err(debug_mode, "[%s:%d] -> allocate_list_memory: could not allocate memory\n", __FILE__, __LINE__);
        list->err_stat = error;
        return error;
    }

    for (size_t i = 0; i < size + 1; i++)
    {
        list->data[i] = poison_value;

        if (i == size) 
            list->ind_arr.next[i] = 0;
        else if (i == 0)
            list->ind_arr.next[i] = 0;
        else
            list->ind_arr.next[i] = i + 1;
    }

    return ok;
}


void list_dtor(lst* list)
{
    assert(list != NULL);

    md_t debug_mode = list->debug_mode;

    printf_log_msg(debug_mode, "list_dtor: began termination\n");

    free(list->data);
    free(list->ind_arr.next);
    free(list->ind_arr.prev);

    printf_log_msg(debug_mode, "list_dtor: termination completed\n");
}