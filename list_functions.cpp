#include "list_functions.h"
#include <assert.h>


err_t list_ctor(lst* list, ssize_t capacity)
{
    assert(list != NULL);
    
    //extern md_t debug_mode;
    md_t debug_mode = list->debug_mode;

    if (debug_mode == on)
    {
        initialize_log(debug_mode);
    }

    printf_log_msg(debug_mode, "list_ctor: began initialization\n");

    list->capacity = capacity;

    if (allocate_list_memory(list, capacity) != ok)
        return error;

    //list->head_pos = 0;
    //list->tail_pos = 0;
    list->free_pos = 1;
    list->size = 0;
    //list->current_pos = 0;

    printf("list_ctor: initialization completed\n");
    return ok;
}


void initialize_log(md_t debug_mode)
{
    printf_log_only(debug_mode, "<pre>\n");
    printf_log_only(debug_mode, "<h3> +++ LIST LOG +++ </h3>\n");
}


err_t allocate_list_memory(lst* list, ssize_t capacity)
{
    assert(list != NULL);

    //extern md_t debug_mode;
    md_t debug_mode = list->debug_mode;
    
    list->data = (lst_t*) calloc((size_t) capacity + 1, sizeof(lst_t));
    list->next = (ssize_t*) calloc((size_t) capacity + 1, sizeof(ssize_t));
    list->prev = (ssize_t*) calloc((size_t) capacity + 1, sizeof(ssize_t));

    if (list->data == NULL || list->next == NULL || list->prev == NULL)
    {
        printf_err(debug_mode, "[%s:%d] -> allocate_list_memory: could not allocate memory\n", __FILE__, __LINE__);
        list->err_stat = error;
        return error;
    }

    for (ssize_t i = 0; i < capacity + 1; i++)
    {
        list->data[i] = poison_value;
        list->prev[i] = -1;

        if (i == capacity or i == 0) 
            list->next[i] = 0;
        else
            list->next[i] = i + 1;
    }

    list->prev[0] = 0;
    list->next[list->capacity] = 0;

    return ok;
}


ssize_t insert_after(lst* list, ssize_t pos, lst_t el) // TODO - check if list is full
{
    assert(list != NULL);
    
    //extern md_t debug_mode;
    md_t debug_mode = list->debug_mode;

    printf_log_msg(debug_mode, "insert_after: began insertion of %d after index %zd\n", el, pos);

    if (pos != 0 && (pos < 0 || pos > list->capacity || list->data[pos] == poison_value))
    {
        list->err_stat = error;
        printf_err(debug_mode, "[%s:%d] -> insert_after: could not insert after not existing element\n", __FILE__, __LINE__);
        return -1;
    }

    ssize_t insertion_pos = list->free_pos;
    
    list->free_pos = list->next[list->free_pos];
    list->data[insertion_pos] = el;

    list->prev[list->next[pos]] = insertion_pos;
    list->next[insertion_pos] = list->next[pos];
    list->next[pos] = insertion_pos;
    list->prev[insertion_pos] = pos;

    list->size++;
    printf_log_msg(debug_mode, "insert_after: insertion finished\n");
    return insertion_pos;
}


ssize_t insert_before(lst* list, ssize_t pos, lst_t el) // TODO - check if list is full
{
    assert(list != NULL);
    
    //extern md_t debug_mode;
    md_t debug_mode = list->debug_mode;

    printf_log_msg(debug_mode, "insert_before: began insertion of %d before index %zd\n", el, pos);
    printf("pos = %zd\n", pos);

    if (pos <= 0 || pos > list->capacity || list->data[pos] == poison_value)
    {
        list->err_stat = error;
        printf_err(debug_mode, "[%s:%d] -> insert_before: could not insert before not existing element\n", __FILE__, __LINE__);
        return -1;
    }

    ssize_t insertion_pos = list->free_pos;
    
    list->free_pos = list->next[list->free_pos];
    list->data[insertion_pos] = el;

    list->next[list->prev[pos]] = insertion_pos;
    list->prev[insertion_pos] = list->prev[pos];
    list->prev[pos] = insertion_pos;
    list->next[insertion_pos] = pos;

    list->size++;

    printf_log_msg(debug_mode, "insert_before: insertion finished\n");
    return insertion_pos;
}


err_t delete_ind(lst* list, ssize_t pos)
{
    assert(list != NULL);

    md_t debug_mode = list->debug_mode;

    printf_log_msg(debug_mode, "delete_ind: began deleting element on index %zd\n", pos);

    if (pos <= 0 || pos > list->capacity || list->data[pos] == poison_value)
    {
        list->err_stat = error;
        printf_err(debug_mode, "[%s:%d] -> delete_ind: could not delete not existing element\n", __FILE__, __LINE__);
        return error;
    }

    list->data[pos] = poison_value;
    list->prev[list->next[pos]] = list->prev[pos];
    list->next[list->prev[pos]] = list->next[pos];
    list->next[pos] = list->free_pos;
    list->free_pos = pos;
    list->prev[pos] = -1;

    list->size--;

    printf_log_msg(debug_mode, "delete_ind: deleting finished\n");
    return ok;
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