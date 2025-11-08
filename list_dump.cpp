#include "list_dump.h"
#include "assert.h"
#include "debug.h"
#include "image_generator.h"

err_t verify_list(const lst* list)
{
    if (list == NULL)
    {
        printf(MAKE_BOLD_RED("ERROR:") "[from list verifier] -> list not found\n");
        return no_data;
    }

    bool found_errors = false;

    md_t debug_mode = list->debug_mode;

    if (list->size > list->capacity)
    {
        printf_err(debug_mode, "[from list verifier] -> list overflow (capacity = %zu, size = %zu)\n", list->capacity, list->size);
        found_errors = true;
    }

    if (list->err_stat != ok)
    {
        printf_err(debug_mode, "[from list verifier] -> error status is not ok\n");
        found_errors = true;
    }

    if (list->data == NULL)
    {
        printf_err(debug_mode, "[from list verifier] -> data not found\n");
        return no_data;
    }

    if (list->next == NULL)
    {
        printf_err(debug_mode, "[from list verifier] -> next not found\n");
        return no_data;
    }

    if (list->prev == NULL)
    {
        printf_err(debug_mode, "[from list verifier] -> prev not found\n");
        return no_data;
    }

    if (list->free_pos < 0 || list->free_pos > (ssize_t) list->capacity)
    {
        printf_err(debug_mode, "[from list verifier] -> incorrect free_pos value (free_pos = %zd)\n", list->free_pos);
        found_errors = true;
    }

    err_t next_ok = check_connections(list, list->next);
    err_t prev_ok = check_connections(list, list->prev);
    if (next_ok != ok || prev_ok != ok)
        found_errors = true;

    if (found_errors)
        return error;
    return ok;
}


err_t process_verification(const lst* list)
{
    err_t verified = verify_list(list);

    if (verified == no_data)
    {
        printf(MAKE_BOLD_RED("\nverification failed")
        ", not enough data to provide additional information\n");
        return error;  
    }   
    else if (verified == error)
    {                                                               
        printf(MAKE_BOLD_RED("\nverification failed\n"));             
        print_dump(list, program_failure);                          
        return error;                                               
    }                                                               
    else                                                            
    {                                                               
        printf_log_msg(list->debug_mode, "\nverification passed\n");  
        return ok;
    }      
}


err_t check_connections(const lst* list, ssize_t * arr)
{
    assert(list != NULL);

    md_t debug_mode = list->debug_mode;

    size_t steps = 0;
    ssize_t current_pos = 0;

    while (steps <= list->size)
    {
        current_pos = arr[current_pos];
        if (current_pos < 0 || current_pos > (ssize_t) list->capacity)
        {
            printf_err(debug_mode, "[from list check_connections] -> index %zd points on not existent position\n", \
                                                                                            current_pos);
            return error;
        }
        steps++;
    }

    if (current_pos != 0)
    {
        printf_err(debug_mode, "[from list check_connections] -> corrupted array loop\n");
            return error;
    }
    return ok;
}


void print_dump(const lst* list, dump_purpose purpose)
{
    assert(list != NULL);

    md_t debug_mode = list->debug_mode;

    if (debug_mode == off)
        return;

    printf_log_bold(debug_mode, "\n=================================LIST DUMP=================================\n\n", NULL);

    if (purpose == diagnostic)
        printf_log_msg(debug_mode, "cause: diagnostic\n\n");
    else
        printf_log_msg(debug_mode, "cause: error\n\n");

    printf_log_bold(debug_mode, "list [%p]\n\n", list);
    printf_log_msg(debug_mode, "capacity = %zu\n", list->capacity);
    printf_log_msg(debug_mode, "size = %zu\n\n", list->size);

    printf_log_msg(debug_mode, "err_stat = %d (0 - no error, 1 - error)\n\n", list->err_stat);

    printf_log_bold(debug_mode, "data [%p]\n", list->data);
    printf_log_bold(debug_mode, "next [%p]\n", list->next);
    printf_log_bold(debug_mode, "prev [%p]\n\n", list->prev);

    printf_log_msg(debug_mode, "free_pos = %zu\n\n", list->free_pos);

    printf_log_msg(debug_mode, "   ____i____|_____data_____|____next____|____prev____\n\n");

    for (size_t i = 0; i <= list->capacity; i++)
    {
        put_number((ssize_t)i, debug_mode);
        printf_log_msg(debug_mode, ":  ");

        put_number(list->data[i], debug_mode);
        printf_log_msg(debug_mode, "  ");

        put_number(list->next[i], debug_mode);
        printf_log_msg(debug_mode, "  ");

        put_number(list->prev[i], debug_mode);
        printf_log_msg(debug_mode, "\n");
    }

    printf_log_bold(debug_mode, "\n===========================================================================\n\n", NULL);

    generate_dump_image(list, -1);
}

void put_number(ssize_t number, md_t debug_mode)
{
    char str[max_number_size + 1] = {};
	for (size_t i = 0; i < max_number_size ; i++)
		str[i] = ' ';

    bool is_negative = number < 0;
    bool is_zero = number == 0;

	if (is_negative)
	{
        str[0] = '-';
		number *= -1;
	}

    for (int i = max_number_size - 1; i >= is_negative; i--)
    {
		if (is_zero)
        {
            is_zero = false;
            str[i] = (char) (48 + number % 10);
            number /= 10;
        }
        else
        {
            if (number == 0)
            {
                if (is_negative)
                {
                    str[i] = '-';
                    is_negative = false;
                }
                else
                {
                    str[i] = ' ';
                }
            }
            else
            {
                str[i] = (char) (48 + number % 10);
                number /= 10;
            }
        }
    }

    
    printf_log_msg(debug_mode, "%s", str);
}


err_t verify_vlist(const vanilla_list* vlist)
{
    if (vlist == NULL)
    {
        printf(MAKE_BOLD_RED("ERROR:") "[from vlist verifier] -> list not found\n");
        return no_data;
    }

    bool found_errors = false;

    md_t debug_mode = vlist->debug_mode;

    if (vlist->err_stat != ok)
    {
        printf_err(debug_mode, "[from vlist verifier] -> error status is not ok\n");
        found_errors = true;
    }

    size_t needed_size = vlist->size;
    size_t factual_size = 0;

    vlist_el* current_el = vlist->head;

    while(current_el != NULL && factual_size <= needed_size)
    {
        factual_size++;

        if (current_el->prev != NULL)
        {
            if (current_el->prev->next != current_el)
            {
                printf_err(debug_mode, "[from vlist verifier] -> next and prev pointers do not match " \
                                                "(prev of [%p] is [%p], while next of [%p] is [%p])\n", \
                                 current_el, current_el->prev, current_el->prev, current_el->prev->next);
                found_errors = true;
            }
        }

        if (current_el->next == NULL && current_el != vlist->tail)
        {
            printf_err(debug_mode, "[from vlist verifier] -> wrong tail pointer [%p] (should be [%p])\n", \
                                                                                    vlist->tail, current_el);
            found_errors = true;
        }

        if (current_el->prev == NULL && current_el != vlist->head)
        {
            printf_err(debug_mode, "[from vlist verifier] -> wrong head pointer [%p] (should be [%p])\n", \
                                                                                    vlist->head, current_el);
            found_errors = true;
        }

        if (current_el->data == poison_value)
        {
            printf_err(debug_mode, "[from vlist verifier] -> vlist element [%p] is poisoned  \n", current_el);
            found_errors = true;
        }

        current_el = current_el->next;
    }

    if (needed_size != factual_size)
    {
        printf_err(debug_mode, "[from vlist verifier] -> wrong size %zu (should be %zu)\n", \
                                                                                    needed_size, factual_size);
        found_errors = true;
    }
    
    if (found_errors)
        return error;
    return ok;
}


err_t process_vlist_verification(const vanilla_list* vlist)
{    
    err_t verified = verify_vlist(vlist);

    if (verified == no_data)
    {
        printf(MAKE_BOLD_RED("\nverification failed")
        ", not enough data to provide additional information\n");
        return error;  
    }   
    else if (verified == error)
    {                                                               
        printf(MAKE_BOLD_RED("\nverification failed\n"));             
        print_vlist_dump(vlist, program_failure); 
        //vlist_generate_dump_image(vlist);                         
        return error;                                               
    }                                                               
    else                                                            
    {                                                               
        printf_log_msg(vlist->debug_mode, "\nverification passed\n");  
        return ok;
    }      
}



void print_vlist_dump(const vanilla_list* vlist, dump_purpose purpose)
{
    assert(vlist != NULL);

    md_t debug_mode = vlist->debug_mode;

    if (debug_mode == off)
        return;

    printf_log_bold(debug_mode, "\n=============================VANILLA LIST DUMP=============================\n\n", NULL);

    if (purpose == diagnostic)
        printf_log_msg(debug_mode, "cause: diagnostic\n\n");
    else
        printf_log_msg(debug_mode, "cause: error\n\n");

    printf_log_bold(debug_mode, "vanilla list [%p]\n\n", vlist);
    printf_log_msg(debug_mode, "size = %zu\n\n", vlist->size);

    printf_log_msg(debug_mode, "err_stat = %d (0 - no error, 1 - error)\n\n", vlist->err_stat);

    printf_log_bold(debug_mode, "head [%p]\n", vlist->head);
    printf_log_bold(debug_mode, "tail [%p]\n", vlist->tail);

    size_t count = 0;
    vlist_el* current_el = vlist->head;

    while (count < vlist->size && current_el != NULL)
    {
        count++;
        printf_log_msg(debug_mode, "element [%p]: data = ", current_el);

        put_number(current_el->data, debug_mode);
        printf_log_msg(debug_mode, "  ");

        printf_log_msg(debug_mode, " prev = %p", current_el->prev);
        printf_log_msg(debug_mode, " next = %p\n", current_el->next);

        current_el = current_el->next;
    }

    printf_log_bold(debug_mode, "\n===========================================================================\n\n", NULL);

    vlist_generate_dump_image(vlist, NULL);
}


// VLIST VERIFIER
// check for each element:
// - if data is not poisoned
// - if next and prev elements are ok