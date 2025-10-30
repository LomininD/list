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
    
    ssize_t capacity;
    //ssize_t head_pos;
    //ssize_t tail_pos;
    ssize_t free_pos;
    ssize_t size;
    //ssize_t current_pos;

    err_t err_stat;
    md_t debug_mode;
};

#endif