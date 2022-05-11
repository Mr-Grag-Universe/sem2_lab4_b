//
// Created by Stephan on 08.05.2022.
//
#include "stdlib.h"
#include "stdio.h"
#include "MyString.h"
#include "KD_tree.h"

KD_key * KD_key_init(size_t number_of_dimensions, unsigned int * keys) {
    KD_key * key = malloc(sizeof(KD_key));
    if (key == NULL) {
        fprintf(stderr, "there is not enough memory for key initialisation.\n");
        exit(FREEING_OF_NULL_PTR);
    }

    key->number_of_dimensions = number_of_dimensions;
    key->keys = keys;

    return key;
}

Error KD_key_free(KD_key * key) {
    if (key == NULL) {
        fprintf(stderr, "NULL-ptr key was tried to get free.\n");
        return FREEING_OF_NULL_PTR;
    }

    if (key->keys == NULL) {
        fprintf(stderr, "NULL-ptr keys in key was tried to get free.\n");
        return FREEING_OF_NULL_PTR;
    }

    free(key->keys);
    free(key);

    return IT_IS_OK;
}

long KD_key_cmp(KD_key * key1, KD_key * key2, bool each_x , size_t index_of_dimension) {
    if (key1 == NULL || key2 == NULL || key1->keys == NULL || key2->keys == NULL) {
        fprintf(stderr, "NULL keys were compared.\n");
        if ((key1 == NULL || key1->keys == NULL) && key2 && key2->keys)
            return -1;
        else if (key1 && key1->keys)
            return 1;
        return 0;
    }

    if (key1->number_of_dimensions <= index_of_dimension) {
        fprintf(stderr, "too big number of current_node_dimension_index.\n");
        exit(WRONG_INPUT);
    }

    if (!each_x)
        return ((long long) key1->keys[index_of_dimension] - (long long) key2->keys[index_of_dimension]);

    for (size_t i = 0; i < key1->number_of_dimensions; ++i) {
        if (key1->keys[i] - key2->keys[i])
            return key1->keys[i] - key2->keys[i];
    }
    return 0;
}

KD_key * KD_key_enter(size_t dimension) {
    unsigned int * keys = malloc(sizeof(unsigned int) * dimension);
    if (keys == NULL) {
        fprintf(stderr, "memory overflow in keys enter.\n");
        exit(MEMORY_OVERFLOW);
    }

    for (size_t i = 0; i < dimension; ++i) {
        keys[i] = get_u_int();
    }
    KD_key * key = KD_key_init(dimension, keys);

    return key;
}

Error KD_key_print(const KD_key * key) {
    if (key == NULL) {
        fprintf(stderr, "NULL key in printing");
        return NULL_PTR_IN_UNEXCITED_PLACE;
    }

    printf("KEY: ");
    for (size_t i = 0; i < key->number_of_dimensions; ++i) {
        printf("%u ", key->keys[i]);
    }
    printf("\n");

    return IT_IS_OK;
}