//
// Created by Stephan on 22.04.2022.
//
#include "Errors.h"
#include "KD_tree.h"
#include "stdlib.h"
#include "stdio.h"
#include "FGetLine.h"
#include "MyString.h"
#include "errno.h"

KD_key * read_key(FILE * file, long offset) {
    if (file == NULL)
        return NULL;

    KD_key * key = NULL;

    char * line = f_get_line(file, offset);
    if (line == NULL)
        return NULL;
    char ** str_keys = split(line, ";, \n\t\r");
    free(line);
    if (str_keys == NULL) {
        return NULL;
    }
    unsigned int * keys = NULL;
    size_t i = 0;
    while (str_keys[i]) {
        keys = realloc(keys, sizeof(unsigned int) * (i+1));
        keys[i] = strtol(str_keys[i], NULL, 10);
        free(str_keys[i]);
        if (errno == ERANGE) {
            fprintf(stderr, "not correct data in file!\ninfo val was turned to the 0\n");
            free(keys);
            return NULL;
        }
        i++;
    }
    free(str_keys);
    key = KD_key_init(i, keys);

    return key;
}

KD_info * read_info(FILE * file, long offset) {
    if (file == NULL)
        return NULL;

    KD_info * info = KD_info_init();

    char * line = f_get_line(file, offset);
    if (line == NULL || line[0] == '\0') {
        fprintf(stderr, "cannot read any line from this file.\n");
        if (line)
            free(line);
        return info;
    }

    info->str_info = line;

    return info;
}

KD_tree * read_tree(char * name_of_file) {
    if (name_of_file == NULL || name_of_file[0] == '\0')
        return NULL;

    FILE * file = fopen(name_of_file, "r");
    if (file == NULL) {
        return NULL;
    }

    size_t number = 0;
    KD_item ** items = NULL;
    while (feof(file) != EOF) {
        KD_key * key = read_key(file, ftell(file));
        if (key == NULL)
            break;
        KD_info * info = read_info(file, ftell(file));
        KD_item * item = KD_item_init(2, key, info);
        items = realloc(items, sizeof(KD_item*) * (number + 1));
        items[number] = item;
        number++;
    }

    KD_item_iterator_container * container = KD_IIC_init(number, items);

    KD_tree * tree = KD_tree_init(2, 3, container);
    free(container);

    return tree;
}