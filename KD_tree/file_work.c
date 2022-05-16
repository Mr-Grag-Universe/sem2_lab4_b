//
// Created by Stephan on 17.05.2022.
//
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "KD_tree.h"

KD_tree * find_all_numbers(FILE * file) {
    if (file == NULL)
        return NULL;
    KD_tree * tree = KD_tree_init(1, 3, NULL);

    while (!feof(file)) {
        unsigned int * number = malloc(sizeof(unsigned int));
        long offset = ftell(file);
        fread(number, sizeof(unsigned int), 1, file);
        KD_key * key = KD_key_init(1, number);
        KD_info * info = KD_info_init();
        info->long_int_info = offset;
        KD_item * item = KD_item_init(1, key, info);
        KD_tree_add(tree, item);
    }

    return tree;
}