//
// Created by Stephan on 18.04.2022.
//
#include "KD_tree.h"
#include "stdio.h"
#include "stdlib.h"
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

KD_node_iterator_container * KD_node_iterator_create(const KD_tree * tree) {
    if (tree == NULL || tree->root == NULL) {
        fprintf(stderr, "WARNING: Tree given for the iteration is empty or it's pointer is NULL.\n");
        return NULL;
    }
    KD_node_iterator_container * container = malloc(sizeof(KD_node_iterator_container));
    container->iterator = NULL;
    container->number_of_elements = 0;

    KD_node * last_node = get_max_node(tree->root);
    KD_node * node = tree->root;
    size_t number_of_nodes = 0;
    recursion(container, node, last_node, &number_of_nodes);

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

KD_item_iterator_container * KD_item_iterator_create(const KD_tree * tree) {
    if (tree == NULL || tree->root == NULL) {
        fprintf(stderr, "WARNING: Tree given for the iteration is empty or it's pointer is NULL.\n");
        return NULL;
    }
    KD_item_iterator_container * container = malloc(sizeof(KD_node_iterator_container));
    container->iterator = NULL;
    container->number_of_elements = 0;

    KD_node * last_node = get_max_node(tree->root);
    KD_node * node = tree->root;
    size_t number_of_nodes = 0;
    recursion_item(container, node, last_node, &number_of_nodes);

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