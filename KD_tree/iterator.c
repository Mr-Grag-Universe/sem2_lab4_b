//
// Created by Stephan on 18.04.2022.
//
#include "KD_tree.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "Errors.h"

void recursion(KD_node_iterator_container * container, KD_node * node, KD_node * last_node, size_t * number_of_nodes) {
    if (node->left) {
        recursion(container, node->left, last_node, number_of_nodes);
    }

    container->iterator = realloc(container->iterator, sizeof(KD_node *) * (++*number_of_nodes));
    container->iterator[*number_of_nodes-1] = node;

    if (node->right) {
        recursion(container, node->right, last_node, number_of_nodes);
    }

    if (node == last_node) {
        return;
    }
}

KD_node_iterator_container * KD_node_iterator_create(const KD_node * node) {
    if (node == NULL) {
        fprintf(stderr, "WARNING: Node given for the iteration is empty or it's pointer is NULL.\n");
        return NULL;
    }
    KD_node_iterator_container * container = malloc(sizeof(KD_node_iterator_container));
    container->iterator = NULL;
    container->number_of_elements = 0;

    KD_node * last_node = get_max_node(node);
    KD_node * node1 = node;
    size_t number_of_nodes = 0;
    recursion(container, node1, last_node, &number_of_nodes);

    container->number_of_elements = number_of_nodes;

    return container;
}

Error KD_node_container_free(KD_node_iterator_container * container) {
    if (container == NULL) {
        fprintf(stderr, "NULL pointer container tried to be freed.\n");
        return FREEING_OF_NULL_PTR;
    }

    if (container->iterator)
        free(container->iterator);
    free(container);

    return IT_IS_OK;
}

void recursion_item(KD_item_iterator_container * container, KD_node * node, KD_node * last_node, size_t * number_of_items) {
    if (node->left) {
        recursion_item(container, node->left, last_node, number_of_items);
    }

    //container->iterator = realloc(container->iterator, sizeof(KD_item *) * (++*number_of_items));

    size_t number_of_eq = *number_of_items;
    for (size_t j = 0; j < node->number_of_items; ++j) {
        container->iterator = realloc(container->iterator, sizeof(KD_item*) * (number_of_eq+1));
        container->iterator[number_of_eq] = node->items[j];
        number_of_eq++;
    }
    *number_of_items = number_of_eq;

    if (node->right) {
        recursion_item(container, node->right, last_node, number_of_items);
    }

    if (node == last_node) {
        return;
    }
}

KD_item_iterator_container * KD_item_iterator_create(const KD_node * node) {
    KD_item_iterator_container * container = malloc(sizeof(KD_node_iterator_container));
    container->iterator = NULL;
    container->number_of_elements = 0;
    if (node == NULL) {
        // fprintf(stderr, "WARNING: Tree given for the iteration is empty or it's pointer is NULL.\n");
        return container;
    }

    KD_node * last_node = get_max_node(node);
    KD_node * node1 = node;
    size_t number_of_nodes = 0;
    recursion_item(container, node1, last_node, &number_of_nodes);

    container->number_of_elements = number_of_nodes;

    return container;
}

Error KD_item_container_free(KD_item_iterator_container * container) {
    if (container == NULL) {
        fprintf(stderr, "NULL pointer container tried to be freed.\n");
        return FREEING_OF_NULL_PTR;
    }

    if (container->iterator)
        free(container->iterator);
    free(container);

    return IT_IS_OK;
}

KD_item_iterator_container * KD_IIC_init(size_t number, KD_item ** items) {
    KD_item_iterator_container * container = malloc(sizeof(KD_item_iterator_container));
    container->iterator = items;
    container->number_of_elements = number;

    return container;
}

KD_item_iterator_container * KD_NC_unpack(KD_node_iterator_container * container) {
    if (container == NULL) {
        return NULL;
    }
    KD_item **items = NULL;
    size_t n = 0;
    for (size_t j = 0; j < container->number_of_elements; ++j) {
        for (size_t i = 0; i < container->iterator[j]->number_of_items; ++i) {
            items = realloc(items, sizeof(KD_item *) * (n + 1));
            items[n] = container->iterator[j]->items[i];
            n++;
        }
    }

    KD_item_iterator_container * container1 = malloc(sizeof(KD_item_iterator_container));
    container1->iterator = items;
    container1->number_of_elements = n;

    return container1;
}

KD_item_iterator_container * KD_tree_create_SIIC(const KD_tree * tree, KD_key * key) {
    if (tree == NULL || tree->root == NULL) {
        fprintf(stderr, "WARNING: Tree given for the iteration is empty or it's pointer is NULL.\n");
        return NULL;
    }

    unsigned int * keys = calloc(tree->number_of_dimensions, sizeof(unsigned int));
    KD_key * key_face = KD_key_init(tree->number_of_dimensions, keys);

    KD_node * node = tree->root;
    KD_item ** items = NULL;
    size_t number_of_items = 0;
    while (node && KD_key_bigger(key, key_face) >= 0) {
        for (size_t i = 0; i < node->number_of_items; ++i) {
            if (KD_key_bigger(key, node->items[i]->key) < 0) {
                items = realloc(items, sizeof(KD_item*) * (number_of_items + 1));
                items[number_of_items] = node->items[i];
                number_of_items++;
            }
        }
        node = node->right;
    }

    KD_item_iterator_container * container = KD_item_iterator_create(node);
    container->iterator = realloc(container->iterator, sizeof(KD_item*) * (container->number_of_elements + number_of_items));
    memmove(container->iterator + container->number_of_elements, items, sizeof(KD_item*) * number_of_items);
    container->number_of_elements += number_of_items;
    free(items);
    KD_key_free(key_face);

    return container;
}