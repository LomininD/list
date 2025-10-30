#include <stdio.h>
#include "list_functions.h"
#include "image_generator.h"

int main()
{
    lst list = {};

    list.debug_mode = on;

    if (list.debug_mode == on)
    {
        log_ptr = fopen("list_debug.log", "w");
    }
    
    list_ctor(&list, 10);

    generate_dump_image(&list);

    insert_after(&list, 0, 10);

    for (int i = 0; i < 10; i++)
    {
        printf("%d: %d\n", i + 1, list.data[i + 1]);
    }

    for (int i = 0; i < 11; i++)
    {
        printf("%d: %zu\n", i, list.next[i]);
    }

    generate_dump_image(&list);

    list_dtor(&list);

    return 0;
}




