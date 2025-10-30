#include "list_functions.h"
#include <assert.h>


err_t list_ctor(lst* list, size_t size)
{
    assert(list != NULL);

    //extern md_t debug_mode;
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

    //extern md_t debug_mode;
    md_t debug_mode = list->debug_mode;
    
    list->data = (lst_t*) calloc(size + 1, sizeof(lst_t));
    list->next = (size_t*) calloc(size + 1, sizeof(size_t));
    list->prev = (size_t*) calloc(size + 1, sizeof(size_t));

    if (list->data == NULL || list->next == NULL || list->prev == NULL)
    {
        printf_err(debug_mode, "[%s:%d] -> allocate_list_memory: could not allocate memory\n", __FILE__, __LINE__);
        list->err_stat = error;
        return error;
    }

    for (size_t i = 0; i < size + 1; i++)
    {
        list->data[i] = poison_value;

        if (i == size or i == 0) 
            list->next[i] = 0;
        else
            list->next[i] = i + 1;
    }

    return ok;
}


size_t insert_after(lst* list, size_t pos, lst_t el)
{
    assert(list != NULL);
    
    //extern md_t debug_mode;
    md_t debug_mode = list->debug_mode;

    printf_log_msg(debug_mode, "insert_after: began insertion\n");

    if (pos != 0 && (pos < 0 || list->data[pos] == poison_value))
    {
        list->err_stat = error;
        printf_err(debug_mode, "[%s:%d] -> insert_after: could not insert after not existing element\n", __FILE__, __LINE__);
        return 0;
    }
    
    
    if (pos == list->tail_pos)
    {
        list->tail_pos = list->free_pos;
    }
    else if (pos == 0) // FIXME - works only for the first time
    {
        size_t insertion_pos = list->free_pos;
    
        list->free_pos = list->next[list->free_pos];
        list->data[insertion_pos] = el;
        list->next[insertion_pos] = list->next[list->head_pos];

        list->head_pos = insertion_pos;
        printf_log_msg(debug_mode, "insert_after: insertion finished\n");
        return insertion_pos;
    }

    size_t insertion_pos = list->free_pos;
    
    list->free_pos = list->next[list->free_pos];
    list->data[insertion_pos] = el;
    list->next[insertion_pos] = list->next[pos];
    list->next[pos] = insertion_pos;

    printf_log_msg(debug_mode, "insert_after: insertion finished\n");
    return insertion_pos;
}


void list_dtor(lst* list)
{
    assert(list != NULL);

    //extern md_t debug_mode;
    md_t debug_mode = list->debug_mode;

    printf_log_msg(debug_mode, "list_dtor: began termination\n");

    free(list->data);
    free(list->next);
    free(list->prev);

    printf_log_msg(debug_mode, "list_dtor: termination completed\n");
}