#ifndef IMAGE_GENERATOR_H
#define IMAGE_GENERATOR_H

#include <stdio.h>
#include "list_properties.h"
#include "debug.h"

enum directions
{
    ltor,
    rtol
};

void generate_dump_image(const lst* list);
void convert_to_image(char* code_file_name, char* image_file_name);
void list_items(FILE* fp, const lst* list);
void fill_preamble(FILE* fp);
void put_pointers(FILE* fp, const lst* list);
void draw_arrows(FILE* fp, const lst* list);
void connect_nodes(FILE* fp, const lst* list, directions dir);

void vlist_generate_dump_image(const vanilla_list* vlist);
void vlist_list_items(FILE* fp, const vanilla_list* vlist);

const size_t file_name_size = 64;
const size_t sys_str_size = 200;

#define IMAGE_CODE_EXT "txt"
#define IMAGE_EXT "png"
#define IMAGE_CODE_PATH "dump_img/code/"
#define IMAGE_PATH "dump_img/"

#endif