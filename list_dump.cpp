#include "list_dump.h"
#include "assert.h"
#include "debug.h"

err_t verify_list(const lst* list)
{
    if (list == NULL)
    {
        printf(MAKE_BOLD_RED("ERROR:") "[from verifier] -> list not found\n");
        return no_data;
    }

    bool found_errors = false;

    md_t debug_mode = list->debug_mode;

    if (list->size > list->capacity)
    {
        printf_err(debug_mode, "[from verifier] -> list overflow (capacity = %zu, size = %zu)\n", list->capacity, list->size);
        found_errors = true;
    }

    if (list->err_stat != ok)
    {
        printf_err(debug_mode, "[from verifier] -> error status is not ok\n");
        found_errors = true;
    }

    if (list->data == NULL)
    {
        printf_err(debug_mode, "[from verifier] -> data not found\n");
        return no_data;
    }

    if (list->next == NULL)
    {
        printf_err(debug_mode, "[from verifier] -> next not found\n");
        return no_data;
    }

    if (list->prev == NULL)
    {
        printf_err(debug_mode, "[from verifier] -> prev not found\n");
        return no_data;
    }

    if (list->free_pos < 0 || list->free_pos > (ssize_t) list->capacity)
    {
        printf_err(debug_mode, "[from verifier] -> incorrect free_pos value (free_pos = %zd)\n", list->free_pos);
        found_errors = true;
    }

    if (found_errors)
        return error;
    return ok;
}


void print_dump(lst* list)
{
    assert(list != NULL);


}