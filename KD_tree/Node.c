// Created by Stephan on 08.05.2022.
//
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "KD_tree.h"
#include "Errors.h"

KD_node * KD_node_init(size_t number_of_items, size_t dimension) {
    KD_node * node = malloc(sizeof(KD_node));
    if (node == NULL)
        return NULL;

    node->right = NULL;
    node->left = NULL;
    node->face = 0;

    node->max_number_of_items = number_of_items;
    node->number_of_items = 0;
    node->dimension = dimension;

    node->items = malloc(sizeof(KD_item*) * number_of_items);
    if (node->items == NULL) {
        free(node);
        return NULL;
    }
    for (size_t i = 0; i < number_of_items; ++i)
        node->items[i] = NULL;

    return node;
}

Error KD_node_free(KD_node * node) {
    if (node == NULL) {
        fprintf(stderr, "NULL-ptr node was tried to get free.\n");
        return FREEING_OF_NULL_PTR;
    }

    if (node->left || node->right)
        fprintf(stderr, "there were not NULL children of freed node.\n");
    node->left = NULL;
    node->right = NULL;

    free(node->items);

    return IT_IS_OK;
}

Error KD_node_add(KD_node * node, KD_item * item) {
    if (node == NULL) {
        fprintf(stderr, "NULL-ptr node in adding");
        return NULL_PTR_IN_UNEXCITED_PLACE;
    }
    if (node->items == NULL) {
        fprintf(stderr, "NULL-ptr items array in adding");
        return NULL_PTR_IN_UNEXCITED_PLACE;
    }

    if (node->number_of_items == node->max_number_of_items) {
        node->left = KD_node_init(node->max_number_of_items, (node->dimension+1)%node->max_number_of_items);
        node->right = KD_node_init(node->max_number_of_items, (node->dimension+1)%node->max_number_of_items);

        if (item->key->keys[node->dimension] > node->face) {
            node = node->right;
        } else {
            node = node->left;
        }
    }


    size_t ind = KD_BS(node, item->key->keys[node->dimension], node->dimension);
    if (ind >= node->max_number_of_items) {
        fprintf(stderr, "index out of range in adding to node.\n");
        exit(WRONG_INPUT);
    }
    memmove(node->items + ind + 1, node->items + ind, (node->number_of_items-ind) * sizeof(KD_item));
    node->items[ind] = item;
    node->number_of_items++;

    return IT_IS_OK;
}