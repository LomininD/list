#include <stdio.h>
#include "list_functions.h"
#include "image_generator.h"
#include "list_dump.h"

int main()
{
    
    /*
    
    lst list = {};

    set_list_debug_mode(&list, on);
    set_list_verification(&list, off);

    if (get_list_debug_mode(&list) == on)
        initialize_list_log(on);
    
    list_ctor(&list, 10);

    // TEST ERROR PROGRAM    
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

    // WORKING LIST
    // insert_after(&list, 0, 20);
    // insert_after(&list, 1, 30);
    // insert_before(&list, 1, 10);
    // delete_ind(&list, 2);
    // print_dump(&list, diagnostic);

    // LOTS OF INSERTIONS
    insert_after(&list, 0, 50);
    insert_after(&list, 1, 10);
    insert_before(&list, 1, 20);
    insert_after(&list, 1, 10);
    insert_before(&list, 1, 20);
    insert_after(&list, 1, 10);
    insert_before(&list, 1, 20);
    insert_after(&list, 1, 10);
    insert_before(&list, 1, 20);
    insert_after(&list, 1, 10);
    insert_before(&list, 1, 20);
    insert_after(&list, 1, 10);
    insert_before(&list, 1, 20);
    insert_after(&list, 1, 10);
    insert_before(&list, 1, 20);
    print_dump(&list, diagnostic);
    
    

    // SEVERELY CORRUPTED STRUCTURE
    // insert_after(&list, 0, 10);
    // insert_after(&list, 1, 20);
    // insert_after(&list, 2, 30);
    // insert_after(&list, 3, 40);
    // insert_after(&list, 4, 50);
    // insert_after(&list, 5, 60);
    // list.prev[4] = -100;
    // list.next[6] = 2;
    // list.capacity = 3;
    // list.size = 10;
    // insert_after(&list, 5, 60);


    for (size_t i = 0; i < get_list_capacity(&list); i++)
    {
        printf("%d\n", list.data[i + 1]);
    }
    list_dtor(&list);
    
    */

    md_t debug_mode = on;
    md_t verification_mode = on;

    vanilla_list vlist = {};

    initialize_list_log(debug_mode);

    initialise_vlist(&vlist, debug_mode, verification_mode);

    vlist_el* first = vlist_insert_after(&vlist, NULL, 10);
    vlist_insert_after(&vlist, first, 20);
    vlist_el* just_el = vlist_insert_after(&vlist, first, 30);
    vlist_insert_after(&vlist, first, 40);
    vlist_insert_before(&vlist, first, 50);
    vlist_insert_before(&vlist, just_el, 60);

    destroy_vlist(&vlist);

    fclose(log_ptr);
    return 0;
}




