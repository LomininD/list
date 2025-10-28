#include <stdio.h>
#include "list_functions.h"

int main()
{
    lst list = {};
    list.debug_mode = off;
    list_ctor(&list, 10);

    for (int i = 0; i < 10; i++)
    {
        printf("%d: %x\n", i + 1, list.data[i + 1]);
    }

    for (int i = 0; i < 11; i++)
    {
        printf("%d: %zu\n", i, list.ind_arr.next[i]);
    }

    list_dtor(&list);

    return 0;
}




