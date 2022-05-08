// Created by Stephan on 08.05.2022.
//
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "Errors.h"
#include "KD_tree.h"

KD_item * KD_item_init(size_t number_of_dimensions, KD_key* key, KD_info* info) {
    KD_item * item = malloc(sizeof(KD_item));
    if (item == NULL) {
        fprintf(stderr, "there is not enough memory to init item\n");
        exit(MEMORY_OVERFLOW);
    }

    item->key = key;
    item->info = info;

    item->number_of_dimensions = number_of_dimensions;

    return item;
}