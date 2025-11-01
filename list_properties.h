#ifndef LIST_PROPERTIES_H
#define LIST_PROPERTIES_H

#include <stdlib.h>
#include "debug.h"

const int poison_value = 0xbadf00d;

typedef int lst_t;

struct lst
{
    lst_t* data;
    ssize_t* next;
    ssize_t* prev;
    ssize_t free_pos;
    
    size_t capacity;
    size_t size;

    err_t err_stat;
    md_t debug_mode;
    md_t verification;
};


struct vanilla_list
{
    vanilla_list* prev;
    vanilla_list* next;
    lst_t data;

    md_t debug_mode;
    md_t verification;
};

#endif