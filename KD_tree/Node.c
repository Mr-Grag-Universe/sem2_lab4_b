//
// Created by Stephan on 08.05.2022.
//
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "KD_tree.h"
#include "Errors.h"
#include "MyMath.h"

KD_node * KD_node_init(size_t number_of_items, size_t dimension) {
    KD_node * node = malloc(sizeof(KD_node));
    if (node == NULL)
        return NULL;

    node->right = NULL;
    node->left = NULL;
    node->face = 0;

    node->max_number_of_items = number_of_items;
    node->number_of_items = 0;
    node->current_node_dimension_index = dimension;

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
    free(node);

    return IT_IS_OK;
}

Error KD_node_free_items(KD_node * node) {
    if (node == NULL) {
        fprintf(stderr, "NULL-ptr node in freeing of it's items");
        return FREEING_OF_NULL_PTR;
    }

    if (node->items == NULL) {
        fprintf(stderr, "NULL items ptr in freeing of node's items");
        return FREEING_OF_NULL_PTR;
    }

    for (size_t i = 0; i < node->number_of_items; ++i)
        KD_item_free(node->items[i]);

    return IT_IS_OK;
}

unsigned int KD_node_count_face(KD_node * node) {
    if (node == NULL) {
        fprintf(stderr, "NULL node ptr in counting of it's face.\n");
        return 0;
    }

    unsigned long long sum = 0;
    for (size_t i = 0; i < node->number_of_items; ++i)
        sum += node->items[i]->key->keys[node->current_node_dimension_index];

    return (unsigned int)(sum / node->number_of_items);
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
        node->left = KD_node_init(node->max_number_of_items, (node->current_node_dimension_index + 1) % node->max_number_of_items);
        node->right = KD_node_init(node->max_number_of_items, (node->current_node_dimension_index + 1) % node->max_number_of_items);

        node->face = KD_node_count_face(node);

        if (item->key->keys[node->current_node_dimension_index] > node->face) {
            node = node->right;
        } else {
            node = node->left;
        }
    }

    size_t ind = KD_BS(node, item->key->keys[node->current_node_dimension_index]);
    if (ind >= node->max_number_of_items) {
        fprintf(stderr, "index out of range in adding to node.\n");
        exit(WRONG_INPUT);
    }
    memmove(node->items + ind + 1, node->items + ind, (node->number_of_items-ind) * sizeof(KD_item*));
    node->items[ind] = item;
    node->number_of_items++;

    return IT_IS_OK;
}

KD_item * KD_node_get_item(const KD_node * node, unsigned int x) {
    if (node == NULL) {
        fprintf(stderr, "NULL node in get item in node.\n");
        return NULL;
    }

    size_t ind = KD_BS(node, x);
    if (ind >= node->max_number_of_items)
        return NULL;

    if (node->items[ind]->key->keys[node->current_node_dimension_index] != x)
        return NULL;

    return node->items[ind];
}

Error KD_node_delete(KD_node * node, KD_key * key) {
    if (node == NULL || key == NULL) {
        fprintf(stderr, "NULL ptr node or key in deleting item from node");
        return WRONG_INPUT;
    }

    KD_item * item = KD_node_get_item(node, key->keys[node->current_node_dimension_index]);
    size_t ind = KD_BS(node, key->keys[node->current_node_dimension_index]);
    if (item == NULL) {
        fprintf(stderr, "strange null in node deleting.\n");
        return NULL_PTR_IN_UNEXCITED_PLACE;
    }

    KD_item_free(item);

    memmove(node->items + ind, node->items + ind + 1, sizeof(KD_item*) * (node->number_of_items-ind-1));
    node->items[node->number_of_items-1] = NULL;
    node->number_of_items--;

    return IT_IS_OK;
}