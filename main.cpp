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

    insert_after(&list, 1, 20);

    generate_dump_image(&list);

    insert_after(&list, 2, 30);

    generate_dump_image(&list);

    insert_after(&list, 3, 40);

    generate_dump_image(&list);

    // delete_ind(&list, 1);

    // generate_dump_image(&list);

    insert_after(&list, 4, 50);

    generate_dump_image(&list);

    insert_after(&list, 5, 60);

    generate_dump_image(&list);

    insert_after(&list, 6, 70);

    generate_dump_image(&list);

    insert_after(&list, 7, 80);

    generate_dump_image(&list);

    insert_after(&list, 8, 90);

    generate_dump_image(&list);

    insert_after(&list, 9, 100);

    generate_dump_image(&list);

    insert_after(&list, 10, 110);

    generate_dump_image(&list);

    insert_after(&list, 11, 120);

    generate_dump_image(&list);

    insert_after(&list, 12, 130);

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




