#ifndef LIST_PROPERTIES_H
#define LIST_PROPERTIES_H

#include <stdlib.h>
#include "debug.h"

const int poison_value = 0xbadf00d;

typedef int lst_t;

struct ind_data
{
    size_t* next;
    size_t* prev;
};

struct lst
{
    lst_t* data;
    ind_data ind_arr;
    size_t size;
    size_t head_pos;
    size_t tail_pos;
    size_t free_pos;
    size_t current_pos;

    err_t err_stat;
    md_t debug_mode;
};

#endif