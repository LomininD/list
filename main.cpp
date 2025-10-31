#include <stdio.h>
#include "list_functions.h"
#include "image_generator.h"
#include "list_dump.h"

int main()
{
    lst list = {};

    list.debug_mode = on;

    if (list.debug_mode == on)
    {
        log_ptr = fopen("list_log.html", "w");
    }
    
    list_ctor(&list, 10);

    verify_list(&list);

    generate_dump_image(&list);

    insert_after(&list, 0, 10);

    generate_dump_image(&list);

    //list.prev[8] = 45;
    //list.next[10] = 56;

    //generate_dump_image(&list); 

    for (int i = 0; i < 10; i++)
    {
        printf("%d: %d\n", i + 1, list.data[i + 1]);
    }

    list_dtor(&list);

    return 0;
}




