// Created by Stephan on 08.05.2022.
//
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "time.h"
#include "string.h"
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
    item->node = NULL;

    item->number_of_dimensions = number_of_dimensions;

    return item;
}

Error KD_item_free(KD_item * item) {
    if (item == NULL) {
        fprintf(stderr, "item ptr is NULL was 'freed'.\n");
        return FREEING_OF_NULL_PTR;
    }

    KD_info_free(item->info);
    KD_key_free(item->key);
    item->node = NULL;
    free(item);

    return IT_IS_OK;
}

Error KD_item_print(KD_item * item) {
    if (item == NULL) {
        printf("null\n");
        return IT_IS_OK;
    }
    KD_key_print(item->key);
    KD_info_print(item->info);

    return IT_IS_OK;
}

KD_item ** KD_item_random_array(size_t number_of_dimensions, unsigned int min, unsigned int max, size_t number, size_t len) {
    srand(time(NULL));

    KD_item ** items = malloc(number * sizeof(KD_item *));
    for (size_t i = 0; i < number; ++i) {
        KD_key * key = KD_key_random(number_of_dimensions, min, max);
        KD_info * info = KD_info_random(len);
        items[i] = KD_item_init(number_of_dimensions, key, info);
    }

    return items;
}

static size_t current_dimension = 0;

int comp(const void * k1, const void * k2) {
    long res = KD_key_cmp((*((KD_item**) k1))->key, (*((KD_item**) k2))->key, 0, current_dimension);
    if (res > 0)
        return 1;
    if (res)
        return -1;
    return 0;
}

void KD_item_sort(KD_item ** items, size_t number, size_t dim) {
    if (items == NULL || items[0] == NULL) {
        fprintf(stderr, "NULL in the keys sort.\n");
        return;
    }
    if (dim >= items[0]->number_of_dimensions) {
        fprintf(stderr, "too big dimension index.\n");
        return;
    }

    size_t d = items[0]->number_of_dimensions;
    for (size_t i = 0; i < number; ++i) {
        if (items[i] == NULL || d != items[i]->number_of_dimensions) {
            fprintf(stderr, "there is an Error in item sort:\n");
            if (items[i] == NULL) {
                fprintf(stderr, "NULL item is here.\n");
            } else {
                fprintf(stderr, "different dimension's numbers.\n");
            }
            return;
        }
    }

    current_dimension = dim;
    //KD_item ** items_sorted = malloc(sizeof(KD_item**) * d);
    //for (size_t i = 0; i < d; ++i) {
        //items_sorted = malloc(sizeof(KD_item*) * number);
        //memmove(items_sorted, items, sizeof(KD_item*) * number);
        qsort(items, number, sizeof(KD_item*), comp);
        //current_dimension = (current_dimension + 1) % d;
    //}
}

unsigned int KD_item_arithmetic_mean(KD_item ** items, size_t number_of_items, size_t current_node_dimension_index) {
    if (items == NULL) {
        fprintf(stderr, "there is null in AM items.\n");
        return 0;
    }

    unsigned long long sum = 0;
    for (size_t i = 0; i < number_of_items; ++i)
        sum += items[i]->key->keys[current_node_dimension_index];

    return (unsigned int)(sum / number_of_items);
}