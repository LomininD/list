#include <stdio.h>
#include "list_functions.h"
#include "image_generator.h"

int main()
{
    lst list = {};

    list.debug_mode = on;

    if (list.debug_mode == on)
    {
        log_ptr = fopen("list_log.html", "w");
    }
    
    list_ctor(&list, 10);

    generate_dump_image(&list);

    insert_after(&list, 0, 10);

    generate_dump_image(&list);

    insert_before(&list, 1, 10);

    generate_dump_image(&list);

    insert_after(&list, 0, 10);

    generate_dump_image(&list);

    insert_before(&list, 1, 10);

    generate_dump_image(&list);

    delete_ind(&list, 1);

    generate_dump_image(&list);



    //list.prev[8] = 45;
    //list.next[10] = 56;

    //generate_dump_image(&list); 

    for (int i = 0; i < 10; i++)
    {
        printf("%d: %d\n", i + 1, list.data[i + 1]);
    }

//     for (int i = 0; i < 11; i++)
//     {
//         printf("%d: %zd\n", i, list.next[i]);
//     }
// 
//     for (int i = 0; i < 11; i++)
//     {
//         printf("%d: %zd\n", i, list.prev[i]);
//     }

    list_dtor(&list);

    return 0;
}




