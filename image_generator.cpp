#include <assert.h>
#include "image_generator.h"


void generate_dump_image(const lst* list)
{
    static unsigned long long image_count = 0; // size_t?

    char code_file_name[file_name_size] = {};
    char image_file_name[file_name_size] = {};
    snprintf(code_file_name, file_name_size, IMAGE_CODE_PATH "%llu." IMAGE_CODE_EXT, image_count);
    snprintf(image_file_name, file_name_size, IMAGE_PATH "%llu." IMAGE_EXT, image_count);

    FILE* fp = fopen(code_file_name, "w");

    fprintf(fp, "digraph G\n");
    fprintf(fp, "{\n");

    fill_preamble(fp);
    list_items(fp, list);
    put_pointers(fp, list);
    draw_arrows(fp, list);

    fprintf(fp, "}\n");

    fclose(fp);
    image_count++;

    convert_to_image(code_file_name, image_file_name);

    printf_log_only(list->debug_mode, "\n\n <img src=\"%s\"> \n\n", image_file_name);
}


void convert_to_image(char* code_file_name, char* image_file_name)
{
    assert(code_file_name != NULL);
    assert(image_file_name != NULL);

    char sys_str[sys_str_size] = {};
    snprintf(sys_str, sys_str_size, "dot %s -T " IMAGE_EXT " -o %s", code_file_name, image_file_name); 

    //puts(sys_str);

    system(sys_str);
}


void fill_preamble(FILE* fp)
{
    assert(fp != NULL);

    fprintf(fp, "rankdir = LR;\n");
    //fprintf(fp, "splines = ortho\n");
    fprintf(fp, "bgcolor = \"white\"\n");
    fprintf(fp, "node[shape = doubleoctagon, style = \"filled\", fillcolor = \"red\", fontcolor = \"white\"]\n");
}


void list_items(FILE* fp, const lst* list)
{
    assert(fp != NULL);
    assert(list != NULL);

    fprintf(fp, "{\n");
    fprintf(fp, "edge[color=\"white\", weight = 100, len = 0]\n");

    for(size_t i = 0; i < list->capacity + 1; i++)
    {
        if (list->data[i] == poison_value)
            fprintf(fp, "%zu [rank = %zu, label = \"ind = %zu | value = %x | { prev = %zd | next = %zd }\", \
                                   shape = Mrecord, style = filled, fillcolor = \"#C0C0C0\", fontcolor = \"black\"]\n", i, i, i, \
                                                                list->data[i], list->prev[i], list->next[i]);
        else
            fprintf(fp, "%zu [rank = %zu, label = \"ind = %zu | value = %d | { prev = %zd | next = %zd }\", \
                                   shape = Mrecord, style = filled, fillcolor = \"#F0F990\", fontcolor = \"black\"]\n", i, i, i, \
                                                                list->data[i], list->prev[i], list->next[i]);
    }
    
    for (size_t i = 0; i < list->capacity; i++)
        fprintf(fp, "%zu->%zu\n", i, i + 1);

    fprintf(fp, "}\n\n");
}


void put_pointers(FILE* fp, const lst* list)
{
    assert(fp != NULL);
    assert(fp != NULL);

    fprintf(fp, "HEAD [shape = invhouse; label = \"head\"; fontcolor = \"black\"; fillcolor = \"#FAA18F \"]\n");
    fprintf(fp, "{rank = same; HEAD; %zd}\n\n", list->next[0]);

    fprintf(fp, "TAIL [shape = invhouse; label = \"tail\"; fillcolor = \"#D5486B\"]\n");
    fprintf(fp, "{rank = same; TAIL; %zd}\n\n", list->prev[0]);

    fprintf(fp, "FREE [shape = invhouse; label = \"free\"; fillcolor = \"#45503B\"; fontcolor = \"white\"]\n");
    fprintf(fp, "{rank = same; FREE; %zd}\n\n", list->free_pos);
}


void draw_arrows(FILE* fp, const lst* list)
{
    assert(fp != NULL);
    assert(list != NULL);

    fprintf(fp, "{\n");
    fprintf(fp, "edge[color = \"#45503B\", weight = 0, penwidth = 1, constraint = false]\n");
    
    for (size_t i = 0; i < list->capacity; i++)
    {
        if (list->prev[i] == -1)
            fprintf(fp, "%zu->%zd\n", i, list->next[i]);
    }

    fprintf(fp, "}\n\n");

    fprintf(fp, "{\n");
    fprintf(fp, "edge[color = \"#FAA18F\", weight = 0, penwidth = 1, constraint = false]\n");

    connect_nodes(fp, list, ltor);

    fprintf(fp, "}\n\n");

    fprintf(fp, "{\n");
    fprintf(fp, "edge[color = \"#D5486B\", weight = 0, penwidth = 1, constraint = false]\n");
    
    connect_nodes(fp, list, rtol);

    fprintf(fp, "}\n\n");
}


void connect_nodes(FILE* fp, const lst* list, directions dir)
{
    assert(fp != NULL);

    ssize_t* arr = NULL;

    if (dir == ltor)
        arr = list->next;
    else
        arr = list->prev;
    
    for (size_t i = 0; i < list->capacity + 1; i++)
    {
        if (list->prev[i] != -1)
        {
            ssize_t next_pos = arr[i];
            if (next_pos >= 0 && next_pos <= (ssize_t) list->capacity)
            {
                ssize_t double_next_pos = arr[next_pos];
                if (double_next_pos == (ssize_t) i)
                {
                    if ((ssize_t) i < next_pos)
                    {
                        fprintf(fp, "%zu->%zd[dir = both]\n", i, arr[i]);
                        continue;
                    }
                    else
                    {
                        continue;
                    }
                }
            }
            else
            {
                fprintf(fp, "%zu->%zd\n [color = \"red\", penwidth = 3]", i, arr[i]);
                continue;
            }
            fprintf(fp, "%zu->%zd\n", i, arr[i]);
        }
    }
}


void vlist_generate_dump_image(const vanilla_list* vlist)
{
    assert(vlist != NULL);

    static unsigned long long image_count = 0; // size_t?

    char code_file_name[file_name_size] = {};
    char image_file_name[file_name_size] = {};
    snprintf(code_file_name, file_name_size, IMAGE_CODE_PATH "%llu." IMAGE_CODE_EXT, image_count);
    snprintf(image_file_name, file_name_size, IMAGE_PATH "%llu." IMAGE_EXT, image_count);

    FILE* fp = fopen(code_file_name, "w");

    fprintf(fp, "digraph G\n");
    fprintf(fp, "{\n");

    fill_preamble(fp);
    vlist_list_items(fp, vlist);

    fprintf(fp, "}\n");

    fclose(fp);
    image_count++;

    convert_to_image(code_file_name, image_file_name);

    printf_log_only(vlist->debug_mode, "\n\n <img src=\"%s\"> \n\n", image_file_name);
}


void vlist_list_items(FILE* fp, const vanilla_list* vlist)
{
    assert(fp != NULL);
    assert(vlist != NULL);

    fprintf(fp, "{\n");
    fprintf(fp, "edge[color=\"#FAA18F\", weight = 100, len = 0, dir = both]\n");

    const vlist_el* current_element = (const vlist_el*) vlist->head;
    size_t i = 0;

    while (current_element != NULL && i < vlist->size)
    {
        fprintf(fp, "%zu [label = \"ptr = %p | ", i, current_element);
        
        if (current_element->data == poison_value)
            fprintf(fp, "value = %x", current_element->data);
        else
            fprintf(fp, "value = %d", current_element->data);

        fprintf(fp, " | { prev = %p | next = %p }\", shape = Mrecord, style = filled, " \
                "fillcolor = \"#C0C0C0\", fontcolor = \"black\"]\n", current_element->prev, \
                                                                        current_element->next);
        
        if (i != 0)
            fprintf(fp, "%zu->%zu\n", i-1, i);

        if (current_element == vlist->head)
        {
            fprintf(fp, "HEAD [shape = invhouse; label = \"head\"; fontcolor = \"black\"; fillcolor = \"#FAA18F \"]\n");
            fprintf(fp, "{rank = same; HEAD; %zd}\n\n", i);
        }

        if (current_element == vlist->tail)
        {
            fprintf(fp, "TAIL [shape = invhouse; label = \"tail\"; fillcolor = \"#D5486B\"]\n");
            fprintf(fp, "{rank = same; TAIL; %zd}\n\n", i);
        }

        current_element = current_element->next;
        i++;
    }
    

    fprintf(fp, "}\n\n");
}
