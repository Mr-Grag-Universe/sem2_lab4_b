//
// Created by Stephan on 17.05.2022.
//
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "KD_tree.h"

unsigned int * read_number(FILE * file) {
    unsigned int * number = malloc(sizeof(unsigned int));
    size_t report = fread(number, sizeof(unsigned int), 1, file);
    if (report == 0) {
        free(number);
        return NULL;
    }
    return number;
}

KD_tree * find_all_numbers(FILE * file) {
    if (file == NULL)
        return NULL;

    KD_item ** items = NULL;
    size_t number_of_items = 0;

    while (feof(file) != EOF) {
        long offset = ftell(file);
        unsigned int * number = read_number(file);
        if (number == NULL)
            break;
        KD_key * key = KD_key_init(1, number);
        KD_info * info = KD_info_init(NULL);
        info->long_int_info = offset;
        KD_item * item = KD_item_init(1, key, info);
        items = realloc(items, sizeof(KD_info*) * (number_of_items+1));
        items[number_of_items] = item;
        number_of_items++;
    }
    KD_item_iterator_container * container = malloc(sizeof(KD_item_iterator_container));
    container->iterator = items;
    container->number_of_elements = number_of_items;

    KD_tree * tree = KD_tree_init(1, 3, container);
    KD_item_container_free(container);

    return tree;
}