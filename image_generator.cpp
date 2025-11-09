#include <assert.h>
#include "image_generator.h"

static bool check_ind(const lst* list, ssize_t ind);
static void convert_to_image(char* code_file_name, char* image_file_name);
static void list_items(FILE* fp, const lst* list, ssize_t highlighted_pos);
static void fill_preamble(FILE* fp);
static void put_pointers(FILE* fp, const lst* list);
static void draw_arrows(FILE* fp, const lst* list);

static void vlist_list_items(FILE* fp, const vanilla_list* vlist, vlist_el* highlighted_pos);


void generate_dump_image(const lst* list, ssize_t highlighted_pos)
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
    list_items(fp, list, highlighted_pos);
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

    system(sys_str);
}


void fill_preamble(FILE* fp)
{
    assert(fp != NULL);

    fprintf(fp, "rankdir = LR;\n");
    fprintf(fp, "bgcolor = \"white\"\n");
    fprintf(fp, "node[shape = doubleoctagon, style = \"filled\", fillcolor = \"red\", fontcolor = \"white\"]\n");
}


void list_items(FILE* fp, const lst* list, ssize_t highlighted_pos)
{
    assert(fp != NULL);
    assert(list != NULL);

    fprintf(fp, "{\n");
    fprintf(fp, "edge[color=\"white\", weight = 100, len = 0]\n");

    for(size_t i = 0; i < list->capacity + 1; i++)
    {
        fprintf(fp, "%zu [rank = %zu, label = \"ind = %zu | ", i, i, i);

        if (list->data[i] == poison_value)
            fprintf(fp, "value = %x", list->data[i]);
        else
            fprintf(fp, "value = %d", list->data[i]);

        fprintf(fp, " | { prev = %zd | next = %zd }\", shape = Mrecord, style = filled," \
                                            " fontcolor = \"black\", ", list->prev[i], list->next[i]);

        if (highlighted_pos == (ssize_t) i)
        {
            if (list->data[i] == poison_value)
                fprintf(fp, "fillcolor = \"#BA8484\"]\n");
            else
                fprintf(fp, "fillcolor = \"#C6F990\"]\n");
        }
        else
        {
            if (list->data[i] == poison_value)
                fprintf(fp, "fillcolor = \"#C0C0C0\"]\n");
            else
                fprintf(fp, "fillcolor = \"#F0F990\"]\n");
        }
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

    fprintf(fp, "edge[color = \"red\", weight = 0, penwidth = 3, constraint = false]\n");

    for (ssize_t i = 0; i <= (ssize_t) list->capacity; i++)
    {
        if (list->prev[i] == -1)
        {
            if (list->next[i] != 0)
            {
                if (list->next[i] >= 0 && list->next[i] <= (ssize_t) list->capacity)
                    fprintf(fp, "%zu->%zd [color = \"#45503B\", weight = 0, penwidth = 1, constraint = false]\n", i, list->next[i]);
                else 
                    fprintf(fp, "%zu->%zd\n", i, list->next[i]);
            }
        }
        else
        {
            if (check_ind(list, list->next[i]))
            {
                if (list->prev[list->next[i]] == i)
                {
                    if (list->next[list->next[i]] != i || (list->next[list->next[i]] == i && i < list->next[i]))
                        fprintf(fp, "%zu->%zd [weight = 0, penwidth = 1, constraint = false, color = \"#88C809\", dir = both]\n", i, list->next[i]);
                }
                else
                {
                    fprintf(fp, "%zu->%zd [weight = 0, penwidth = 1, constraint = false, color = \"#FAA18F\"]\n", i, list->next[i]);


                    if (check_ind(list, list->prev[list->next[i]]))
                    {
                        if (list->next[list->prev[list->next[i]]] != list->next[i])
                            fprintf(fp, "%zu->%zd [weight = 0, penwidth = 1, constraint = false, color = \"#D5486B\"]\n", list->next[i], list->prev[list->next[i]]);
                    }
                    else
                    {
                        fprintf(fp, "%zu->%zd\n", list->next[i], list->prev[list->next[i]]);
                    }
                }
            }
            else
            {
                fprintf(fp, "%zu->%zd\n", i, list->next[i]);
            }
        }
    }
}


bool check_ind(const lst* list, ssize_t ind)
{
    assert(list != NULL);

    if (ind >= 0 && ind <= (ssize_t) list->capacity)
        return true;
    return false;
}


void vlist_generate_dump_image(const vanilla_list* vlist, vlist_el* highlighted_pos)
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
    vlist_list_items(fp, vlist, highlighted_pos);

    fprintf(fp, "}\n");

    fclose(fp);
    image_count++;

    convert_to_image(code_file_name, image_file_name);

    printf_log_only(vlist->debug_mode, "\n\n <img src=\"%s\"> \n\n", image_file_name);
}


void vlist_list_items(FILE* fp, const vanilla_list* vlist, vlist_el* highlighted_pos)
{
    assert(fp != NULL);
    assert(vlist != NULL);

    fprintf(fp, "{\n");
    fprintf(fp, "edge[color=\"white\", weight = 100, len = 0]\n");

    vlist_el* current = vlist->head;
    size_t node_count = 0;

    while (current != NULL && node_count < vlist->size)
    {
        fprintf(fp, "NODE_%p [label = <<TABLE BORDER=\"0\" CELLBORDER=\"1\"><TR><TD COLSPAN=\"2\">ptr =<FONT color = \"#%x\"> %p </FONT></TD></TR>", current, hash((long long int) current), current);
        if (current->data == poison_value)
            fprintf(fp, "<TR><TD COLSPAN=\"2\">value = %x</TD></TR>", current->data);
        else
            fprintf(fp, "<TR><TD COLSPAN=\"2\">value = %d</TD></TR>", current-> data);
        
        fprintf(fp, "<TR><TD> prev =<FONT color = \"#%x\"> %p </FONT></TD><TD> next =<FONT color = \"#%x\"> %p </FONT></TD></TR></TABLE>>, shape = Mrecord, style = filled, ",\
                                                                     hash((long long int) current->prev), current->prev, hash((long long int) current->next), current->next);

        if (highlighted_pos == current)
            fprintf(fp, "fillcolor = \"#C6F990\", fontcolor = \"black\"]\n");
        else
            fprintf(fp, "fillcolor = \"#C0C0C0\", fontcolor = \"black\"]\n");

        if (current->next != NULL)
        {
            fprintf(fp, "NODE_%p->NODE_%p\n", current, current->next);
            if (current->next->prev == current)
            {
                fprintf(fp, "NODE_%p->NODE_%p [constraint = false, dir = both, color = \"#88C809\"]\n", current, current->next);
            }
            else
            {
                fprintf(fp, "NODE_%p->NODE_%p [constraint = false, color = \"#FAA18F\"]\n", current, current->next);
                if (current->next->prev != NULL && current->next->prev->next != current->next)
                    fprintf(fp, "NODE_%p->NODE_%p [constraint = false, color = \"#D5486B\"]\n", current->next, current->next->prev);
            }
        }

        node_count++;
        current = current->next;
    }

    fprintf(fp, "}\n\n");
}


int hash(long long int ptr)
{
    int hashed = 0;

    if (ptr == 0)
    {
        return 16711680;
    }

    while (ptr != 0)
    {
        hashed += (7717 * (ptr % 10) % 1013 + 349) * 147867;
        ptr /= 10; 
    }

    hashed %= 16777215;

    return hashed;
}

