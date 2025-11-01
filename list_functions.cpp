#include "list_functions.h"
#include "image_generator.h"
#include <assert.h>

static err_t allocate_list_memory(lst* list, size_t capacity);
static err_t reallocate_list_memory(lst* list);

static vanilla_list* create_vlist_element(md_t debug_mode, md_t verification);
// static void destroy_vlist_element(vanilla_list* vlist);


err_t list_ctor(lst* list, size_t capacity)
{
    if (list == NULL)
    {
        printf(MAKE_BOLD_RED("ERROR: ") "[from list_ctor] -> list not found\n");
        return error;
    }
    
    md_t debug_mode = list->debug_mode;

    printf_log_msg(debug_mode, "list_ctor: began initialization\n");

    list->capacity = capacity;

    if (allocate_list_memory(list, capacity) != ok)
        return error;

    list->free_pos = 1;
    list->size = 0;

    VERIFY_LIST();

    printf_log_msg(debug_mode, "list_ctor: initialization completed\n");
    return ok;
}


void initialize_list_log(md_t debug_mode)
{
    log_ptr = fopen("list_log.html", "w");
    printf_log_only(debug_mode, "<pre>\n");
    printf_log_only(debug_mode, "<h3> +++ LIST LOG +++ </h3>\n");
}


err_t allocate_list_memory(lst* list, size_t capacity)
{
    md_t debug_mode = list->debug_mode;

    printf_log_msg(debug_mode, "allocate_list_memory: began memory allocation\n");
    
    list->data = (lst_t*) calloc(capacity + 1, sizeof(lst_t));
    list->next = (ssize_t*) calloc(capacity + 1, sizeof(ssize_t));
    list->prev = (ssize_t*) calloc(capacity + 1, sizeof(ssize_t));

    if (list->data == NULL || list->next == NULL || list->prev == NULL)
    {
        printf_err(debug_mode, "from list [%s:%d] -> allocate_list_memory: could not allocate memory\n", __FILE__, __LINE__);
        list->err_stat = error;
        return error;
    }

    for (size_t i = 0; i < capacity + 1; i++)
    {
        list->data[i] = poison_value;
        list->prev[i] = -1;

        if (i == capacity or i == 0) 
            list->next[i] = 0;
        else
            list->next[i] = (ssize_t) i + 1;
    }

    list->prev[0] = 0;
    list->next[list->capacity] = 0;

    VERIFY_LIST();

    printf_log_msg(debug_mode, "allocate_list_memory: done memory allocation\n");

    return ok;
}


err_t reallocate_list_memory(lst* list)
{
    VERIFY_LIST();

    md_t debug_mode = list->debug_mode;

    printf_log_msg(debug_mode, "reallocate_list_memory: began memory reallocation\n");

    size_t prev_capacity = list->capacity;

    if (list->capacity < 10)
        list->capacity = 10;
    
    list->capacity *= 2;
    printf_log_msg(debug_mode, "previous capacity = %zu, new capacity = %zu\n", prev_capacity, list->capacity);

    printf_log_msg(debug_mode, "old data: %p\n", list->data);
    printf_log_msg(debug_mode, "old next: %p\n", list->next);
    printf_log_msg(debug_mode, "old prev: %p\n", list->prev);

    lst_t* new_data = (lst_t*) realloc(list->data, (list->capacity + 1) * sizeof(lst_t));
    ssize_t* new_next = (ssize_t*) realloc(list->next, (list->capacity + 1) * sizeof(ssize_t));
    ssize_t* new_prev = (ssize_t*) realloc(list->prev, (list->capacity + 1) * sizeof(ssize_t));

    printf_log_msg(debug_mode, "new data: %p\n", new_data);
    printf_log_msg(debug_mode, "new next: %p\n", new_next);
    printf_log_msg(debug_mode, "new prev: %p\n", new_prev);

    if (new_data == NULL || new_next == NULL || new_prev == NULL)
    {
        printf_err(debug_mode, "from list [%s:%d] -> reallocate_list_memory: could not reallocate memory\n", __FILE__, __LINE__);
        list->err_stat = error;
        return error;
    }

    list->data = new_data;
    list->next = new_next;
    list->prev = new_prev;

    for (size_t i = prev_capacity + 1; i < list->capacity + 1; i++)
    {
        list->data[i] = poison_value;
        list->prev[i] = -1;

        if (i == list->capacity) 
            list->next[i] = 0;
        else
            list->next[i] = (ssize_t) i + 1;
    }

    list->next[list->capacity] = 0;

    list->free_pos = (ssize_t) prev_capacity + 1;

    VERIFY_LIST();

    printf_log_msg(debug_mode, "reallocate_list_memory: done memory reallocation\n");
    return ok;
}


ssize_t insert_after(lst* list, ssize_t pos, lst_t el)
{
    VERIFY_LIST();

    md_t debug_mode = list->debug_mode;

    printf_log_msg(debug_mode, "insert_after: began insertion of %d after index %zd\n", el, pos);
    printf_log_msg(debug_mode, "capacity = %zu, size = %zu\n", list->capacity, list->size);

    if (pos != 0 && (pos < 0 || pos > (ssize_t) list->capacity || list->data[pos] == poison_value))
    {
        list->err_stat = error;
        printf_err(debug_mode, "from list [%s:%d] -> insert_after: could not insert after not existing element\n", __FILE__, __LINE__);
        return -1;
    }

    ASSERT_CAPACITY();
    
    ssize_t insertion_pos = list->free_pos;
    
    list->free_pos = list->next[list->free_pos];
    list->data[insertion_pos] = el;

    list->prev[list->next[pos]] = insertion_pos;
    list->next[insertion_pos] = list->next[pos];
    list->next[pos] = insertion_pos;
    list->prev[insertion_pos] = pos;
    list->size++;

    VERIFY_LIST();

    printf_log_msg(debug_mode, "insert_after: insertion finished\n");
    
    DISPLAY_LIST();

    return insertion_pos;
}


ssize_t insert_before(lst* list, ssize_t pos, lst_t el)
{
    VERIFY_LIST();
    
    //extern md_t debug_mode;
    md_t debug_mode = list->debug_mode;

    printf_log_msg(debug_mode, "insert_before: began insertion of %d before index %zd\n", el, pos);
    printf_log_msg(debug_mode, "capacity = %zu, size = %zu\n", list->capacity, list->size);

    if (pos <= 0 || pos > (ssize_t) list->capacity || list->data[pos] == poison_value)
    {
        list->err_stat = error;
        printf_err(debug_mode, "from list [%s:%d] -> insert_before: could not insert before not existing element\n", __FILE__, __LINE__);
        return -1;
    }

    ASSERT_CAPACITY();

    ssize_t insertion_pos = list->free_pos;
    
    list->free_pos = list->next[list->free_pos];
    list->data[insertion_pos] = el;

    list->next[list->prev[pos]] = insertion_pos;
    list->prev[insertion_pos] = list->prev[pos];
    list->prev[pos] = insertion_pos;
    list->next[insertion_pos] = pos;
    list->size++;

    VERIFY_LIST();

    printf_log_msg(debug_mode, "insert_before: insertion finished\n");

    DISPLAY_LIST();

    return insertion_pos;
}


err_t delete_ind(lst* list, ssize_t pos)
{
    VERIFY_LIST();

    md_t debug_mode = list->debug_mode;

    printf_log_msg(debug_mode, "delete_ind: began deleting element on index %zd\n", pos);

    if (pos <= 0 || pos > (ssize_t) list->capacity || list->data[pos] == poison_value)
    {
        list->err_stat = error;
        printf_err(debug_mode, "from list [%s:%d] -> delete_ind: could not delete not existing element\n", __FILE__, __LINE__);
        return error;
    }

    list->data[pos] = poison_value;
    list->prev[list->next[pos]] = list->prev[pos];
    list->next[list->prev[pos]] = list->next[pos];
    list->next[pos] = list->free_pos;
    list->free_pos = pos;
    list->prev[pos] = -1;
    list->size--;

    VERIFY_LIST();

    printf_log_msg(debug_mode, "delete_ind: deleting finished\n");
    
    DISPLAY_LIST();
        
    return ok;
}


size_t get_list_size(lst* list)
{
    if (list == NULL)
    {
        printf(MAKE_BOLD_RED("ERROR: ") "[from list_ctor] -> list not found\n");
        return 0;
    }

    return list->size;
}


size_t get_list_capacity(lst* list)
{
    if (list == NULL)
    {
        printf(MAKE_BOLD_RED("ERROR: ") "[from list_ctor] -> list not found\n");
        return 0;
    }

    return list->capacity;
}

void set_list_debug_mode(lst* list, md_t mode)
{
    if (list == NULL)
    {
        printf(MAKE_BOLD_RED("ERROR: ") "[from list_ctor] -> list not found\n");
        return;
    }

    list->debug_mode = mode;
}


void set_list_verification(lst* list, md_t mode)
{
    if (list == NULL)
    {
        printf(MAKE_BOLD_RED("ERROR: ") "[from list_ctor] -> list not found\n");
        return;
    }

    list->verification = mode;
}


md_t get_list_debug_mode(lst* list)
{
    if (list == NULL)
    {
        printf(MAKE_BOLD_RED("ERROR: ") "[from list_ctor] -> list not found\n");
        return on;
    }

    return list->debug_mode;
}


err_t list_dtor(lst* list)
{
    if (list == NULL)
    {
        printf(MAKE_BOLD_RED("ERROR: ") "[from list_ctor] -> list not found\n");
        return error;
    }
    
    //extern md_t debug_mode;
    md_t debug_mode = list->debug_mode;

    printf_log_msg(debug_mode, "list_dtor: began termination\n");

    free(list->data);
    free(list->next);
    free(list->prev);

    printf_log_msg(debug_mode, "list_dtor: termination completed\n");
    return ok;
}


vanilla_list* create_vlist_element(md_t debug_mode, md_t verification)
{
    printf_log_msg(debug_mode, "create_vlist_ptr: began allocating memory for new element\n");
    
    vanilla_list* new_el_ptr = (vanilla_list*) calloc(1, sizeof(vanilla_list));

    if (new_el_ptr == NULL)
    {
        printf_err(debug_mode, "[from create_vlist_ptr] -> failed to allocate memory for vanilla list element\n");
        return NULL;
    }

    new_el_ptr->debug_mode = debug_mode;
    new_el_ptr->debug_mode = verification;
    new_el_ptr->data = poison_value;
    new_el_ptr->prev = NULL;
    new_el_ptr->next = NULL;

    printf_log_msg(debug_mode, "create_vlist_ptr: done allocating memory for new element [%p]\n", new_el_ptr);
    
    return new_el_ptr;
}


vanilla_list* create_vlist(lst_t value, md_t debug_mode, md_t verification)
{
    printf_log_msg(debug_mode, "create_vlist: began creating vlist\n");

    vanilla_list* el = create_vlist_element(debug_mode, verification);

    if (el == NULL)
        return NULL;

    el->data = value;

    printf_log_msg(debug_mode, "create_vlist: vlist created [%p]\n", el);

    DISPLAY_VLIST();

    return el;
}


vanilla_list* vlist_insert_after(vanilla_list* el, lst_t value)
{
    // verifier

    md_t debug_mode = el->debug_mode;

    printf_log_msg(debug_mode, "vlist_insert_after: began insertion after vlist element [%p]\n", el);

    vanilla_list* new_el = create_vlist_element(debug_mode, el->verification);

    if (new_el == NULL)
        return NULL;

    if (el->next != NULL)
        (el->next)->prev = new_el;

    new_el->data = value;
    new_el->next = el->next;
    new_el->prev = el;
    el->next = new_el;

    printf_log_msg(debug_mode, "vlist_insert_after: inserted element [%p]\n", new_el);

    DISPLAY_VLIST();

    return new_el;
}


vanilla_list* vlist_insert_before(vanilla_list* el, lst_t value) // FIXME - smth wrong with debug_mode, wouldnt work for 1 el
{
    // verifier

    md_t debug_mode = el->debug_mode;

    printf("%d\n", debug_mode);

    printf_log_msg(debug_mode, "vlist_insert_before: began insertion before vlist element [%p]\n", el);
    printf_log_msg(debug_mode, "vlist_insert_before: redirecting function call to vlist_insert_after [%p]\n", el->prev);

    vanilla_list* new_el = vlist_insert_after(el->prev, value);

    printf_log_msg(debug_mode, "vlist_insert_before: inserted element [%p]\n", new_el);

    return new_el;
}


void destroy_vlist(vanilla_list* vlist)
{
    // verifier 
    
    md_t debug_mode = vlist->debug_mode;

    printf_log_msg(debug_mode, "destroy_vlist: began destruction of vanilla list\n");
     
    vanilla_list* current_el = vlist;
    vanilla_list* next_el = NULL;

    while(current_el != NULL)
    {
        next_el = current_el->next;
        printf_log_msg(debug_mode, "destroy_vlist: deleting [%p]\n", current_el);
        free(current_el);
        current_el = next_el;
    }

    printf_log_msg(debug_mode, "destroy_vlist: done destruction of vanilla list\n");
}


