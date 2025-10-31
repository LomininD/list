#ifndef LIST_DUMP_H
#define LIST_DUMP_H

#include "debug.h"
#include "list_properties.h"

const int max_number_size = 10;

enum dump_purpose
{
    diagnostic,
    program_failure
};

err_t verify_list(const lst* list);
err_t process_verification(const lst* list);
void print_dump(const lst* list, dump_purpose purpose);
err_t check_connections(const lst* list, ssize_t * arr);
void put_number(ssize_t number, md_t debug_mode);

#endif