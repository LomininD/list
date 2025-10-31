#include <stdio.h>
#include "list_functions.h"
#include "image_generator.h"
#include "list_dump.h"

int main()
{
    lst list = {};
    set_list_debug_mode(&list, on);
    if (get_list_debug_mode(&list) == on)
        log_ptr = fopen("list_log.html", "w");
    
    list_ctor(&list, 10);

    
//     insert_after(&list, 0, 10);
//     insert_after(&list, 1, 20);
//     insert_after(&list, 2, 30);
//     insert_after(&list, 3, 40);
//     insert_after(&list, 4, 500);
//     insert_after(&list, 5, 600);
//     insert_after(&list, 6, 700);
//     insert_after(&list, 7, 800);
// 
//     list.next[7] = 5;
//     list.prev[6] = 300;
// 
//     insert_after(&list, 8, 900);

    insert_after(&list, 0, 20);
    insert_after(&list, 1, 30);
    insert_before(&list, 1, 10);
    delete_ind(&list, 2);
    print_dump(&list, diagnostic);

    for (size_t i = 0; i < get_list_capacity(&list); i++)
    {
        printf("%d\n", list.data[i + 1]);
    }
    list_dtor(&list);
    return 0;
}




