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
    fprintf(fp, "splines = ortho\n");
    fprintf(fp, "node[shape = Mrecord, style = \"filled\", fillcolor = \"lightgrey\"]\n");
}


void list_items(FILE* fp, const lst* list)
{
    assert(fp != NULL);
    assert(list != NULL);

    fprintf(fp, "{\n");
    fprintf(fp, "edge[color=white; weight = 500]\n");

    for(size_t i = 0; i < list->size + 1; i++)
        fprintf(fp, "%zu [label = \"ind = %zu | value = %d | { prev = %zu | next = %zu } \"]\n", \
                            i, i, list->data[i], list->prev[i], list->next[i]);
    
    for (size_t i = 0; i < list->size; i++)
        fprintf(fp, "%zu->%zu\n", i, i + 1);

    fprintf(fp, "}\n\n");
}


void put_pointers(FILE* fp, const lst* list)
{
    assert(fp != NULL);
    assert(fp != NULL);

    fprintf(fp, "HEAD [shape = egg; label = \"head_pos\"]\n");
    fprintf(fp, "{rank = same; HEAD; %zu}\n\n", list->head_pos);

    fprintf(fp, "TAIL [shape = egg; label = \"tail_pos\"]\n");
    fprintf(fp, "{rank = same; TAIL; %zu}\n\n", list->tail_pos);

    fprintf(fp, "FREE [shape = egg; label = \"free_pos\"]\n");
    fprintf(fp, "{rank = same; FREE; %zu}\n\n", list->free_pos);
}


void draw_arrows(FILE* fp, const lst* list)
{
    assert(fp != NULL);
    assert(list != NULL);

    fprintf(fp, "{\n");
    fprintf(fp, "edge[color = \"lightgrey\"]\n");
    fprintf(fp, "FREE->%zu\n", list->free_pos);
    
    connect_nodes(fp, list->next, list->free_pos);

    fprintf(fp, "}\n\n");

    fprintf(fp, "{\n");
    fprintf(fp, "edge[color = \"red\"]\n");
    fprintf(fp, "HEAD->%zu\n", list->head_pos);
    
    connect_nodes(fp, list->next, list->head_pos);

    fprintf(fp, "}\n\n");

    fprintf(fp, "{\n");
    fprintf(fp, "edge[color = \"green\"]\n");
    fprintf(fp, "TAIL->%zu\n", list->tail_pos);
    
    connect_nodes(fp, list->prev, list->tail_pos);

    fprintf(fp, "}\n\n");
}


void connect_nodes(FILE* fp, const size_t* arr, size_t begin_pos)
{
    assert(fp != NULL);
    
    size_t i = arr[begin_pos];
    size_t prev = begin_pos; 
    while(i != 0)
    {
        fprintf(fp, "%zu->%zu\n", prev, i);
        prev = i;
        i = arr[i];
    }
}
