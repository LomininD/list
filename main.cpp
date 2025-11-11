#include <stdio.h>
#include <math.h>
#include <time.h>

#include <sys/resource.h>
struct rusage usage;
#include <pthread.h>

#include "list_functions.h"
#include "image_generator.h"
#include "list_dump.h"

int main()
{
    ///*

    // TIME COMPARASION 

    lst list = {};
    set_list_debug_mode(&list, off);
    list_ctor(&list, 10);

    // vanilla_list vlist = {};
    // initialise_vlist(&vlist, off);
    


    for (int i = 3; i < 9; i++)
    {
        getrusage(RUSAGE_SELF, &usage);
        struct timeval begin = usage.ru_utime;
        long long start_time = begin.tv_usec + 1000000 * begin.tv_sec;
        printf("start_time (sec) = %ld (msec) = %d\n", begin.tv_sec, begin.tv_usec);
        //clock_t start = clock();
        //printf("%lu\n", start);
        for (long long int j = 0; j < pow(10, i); j++)
        {
            ssize_t ind = insert_after(&list, 0, 10);
            delete_ind(&list, ind);
        }
        //clock_t finish = clock();
        //printf("%lu\n", finish);
        getrusage(RUSAGE_SELF, &usage);
        struct timeval end = usage.ru_utime;
        long long finish_time = end.tv_usec + 1000000 * end.tv_sec;
        printf("start_time (sec) = %ld (msec) = %d\n", end.tv_sec, end.tv_usec);
        
        printf("LIST: number of iterations = 10^%d, delta = %llu\n\n", i, finish_time - start_time);
   }

    // for (int i = 3; i < 9; i++)
    // {
    //     getrusage(RUSAGE_SELF, &usage);
    //     struct timeval begin = usage.ru_utime;
    //     long long start_time = begin.tv_usec + 1000000 * begin.tv_sec;
    //     printf("start_time (sec) = %ld (msec) = %d\n", begin.tv_sec, begin.tv_usec);
    //     //clock_t start = clock();
    //     //printf("%lu\n", start);
    //     for (long long int j = 0; j < pow(10, i); j++)
    //     {
    //         vlist_el* ptr = vlist_insert_after(&vlist, NULL, 10);
    //         vlist_delete(&vlist, ptr);
    //     }
    //     getrusage(RUSAGE_SELF, &usage);
    //     struct timeval end = usage.ru_utime;
    //     long long finish_time = end.tv_usec + 1000000 * end.tv_sec;
    //     printf("start_time (sec) = %ld (msec) = %d\n", end.tv_sec, end.tv_usec);
    //     
    //     //clock_t finish = clock();
    //     //printf("%lu\n", finish);
    //     printf("VLIST: number of iterations = 10^%d, delta = %llu\n\n", i, finish_time-start_time);
    // }


    list_dtor(&list);
    //destroy_vlist(&vlist);
    //*/
    
    
    // LIST
    /*
    
    lst list = {};

    set_list_debug_mode(&list, on);

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
//     list.prev[3] = 1;
// 
//     insert_after(&list, 8, 900);

    //WORKING LIST
    insert_after(&list, 0, 20);
    insert_after(&list, 1, 30);
    insert_before(&list, 1, 10);
    delete_ind(&list, 2);
    //clean_up_list(&list);
    print_dump(&list, diagnostic);

    insert_after(&list, 0, 10);
    insert_after(&list, 1, 20);
    insert_after(&list, 2, 30);
    insert_after(&list, 3, 40);
    insert_after(&list, 4, 50);
    //insert_after(&list, 1, 100);
    push_back(&list, 25);
    clean_up_list(&list);


    // LOTS OF INSERTIONS
    insert_after(&list, 0, 50);
    insert_after(&list, 1, 10);
    insert_before(&list, 1, 40);
    insert_after(&list, 1, 10);
    insert_before(&list, 1, 40);
    insert_after(&list, 1, 10);
    insert_before(&list, 1, 40);
    insert_after(&list, 1, 10);
    insert_before(&list, 1, 40);
    insert_after(&list, 1, 10);
    insert_before(&list, 1, 40);
    insert_after(&list, 1, 10);
    insert_before(&list, 1, 40);
    insert_after(&list, 1, 10);
    insert_before(&list, 1, 40);
    is_cleaned_up(&list);
    clean_up_list(&list);
    is_cleaned_up(&list);
    clean_up_list(&list);
    print_dump(&list, diagnostic);
    

    size_t cap = get_list_capacity(&list); 

    for (size_t i = 0; i < cap; i++)
    {
        printf("%d\n", list.data[i + 1]);
    }
    list_dtor(&list);
    
    */


    /*
    // VLIST 
    
    md_t debug_mode = on;

    vanilla_list vlist = {};

    initialize_list_log(debug_mode);

    initialise_vlist(&vlist, debug_mode);

    // TEST PROGRAM 1
    // vlist_el* first = vlist_insert_after(&vlist, NULL, 10);
    // vlist_insert_after(&vlist, first, 20);
    // vlist_el* just_el = vlist_insert_after(&vlist, first, 30);
    // vlist_insert_after(&vlist, first, 40);
    // vlist_insert_before(&vlist, first, 50);
    // vlist_insert_before(&vlist, just_el, 60);
    // vlist_delete(&vlist, just_el);
    // vlist_delete(&vlist, first);
    // print_vlist_dump(&vlist, diagnostic);

    // TEST PROGRAM 2
    vlist_el* first = vlist_insert_after(&vlist, NULL, 10);
    vlist_el* second = vlist_insert_after(&vlist, first, 20);
    vlist_el* third = vlist_insert_after(&vlist, second, 30);
    vlist_delete(&vlist, second);
    vlist_delete(&vlist, third);
    vlist_delete(&vlist, first);

    // TEST PROGRAM 3
//     vlist_el* first = vlist_insert_after(&vlist, NULL, 50);
//     vlist_el* second = vlist_insert_after(&vlist, first, 40);
//     vlist_el* third = vlist_insert_after(&vlist, second, 60);
// 
//     vlist_el* fourth = vlist_insert_after(&vlist, third, 20);
// 
//     second->prev = fourth;
// 
//     vlist_insert_after(&vlist, fourth, 100);
// 
//     destroy_vlist(&vlist);
    */

    //fclose(log_ptr);
    return 0;
}




