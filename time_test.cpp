#include <stdio.h>
#include <math.h>

#include <sys/resource.h>
struct rusage usage;

#include "list_functions.h"
#include "image_generator.h"
#include "list_dump.h"

int main()
{

    // TIME COMPARASION 
    //pthread_set_qos_class_self_np()

    lst list = {};
    set_list_debug_mode(&list, off);
    list_ctor(&list, 10);

    vanilla_list vlist = {};
    initialise_vlist(&vlist, off);

    for (int k = 0; k < 4; k++)
    {
        printf("LOOP %d\n\n", k);


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
                vlist_el* ptr = vlist_insert_after(&vlist, NULL, 10);
                vlist_delete(&vlist, ptr);
            }
            getrusage(RUSAGE_SELF, &usage);
            struct timeval end = usage.ru_utime;
            long long finish_time = end.tv_usec + 1000000 * end.tv_sec;
            printf("start_time (sec) = %ld (msec) = %d\n", end.tv_sec, end.tv_usec);
            
            //clock_t finish = clock();
            //printf("%lu\n", finish);
            printf("VLIST: number of iterations = 10^%d, delta = %llu\n\n", i, finish_time-start_time);
        }


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
    }


    list_dtor(&list);
    destroy_vlist(&vlist);
    
   return 0;
}