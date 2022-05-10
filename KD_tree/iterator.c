//
// Created by Stephan on 18.04.2022.
//
#include "KD_tree.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "Errors.h"

void recursion(KD_iterator_container * container, KD_node * node, KD_node * last_node, size_t * number_of_nodes) {
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

KD_iterator_container * KD_iterator_create(const KD_tree * tree) {
    if (tree == NULL || tree->root == NULL) {
        fprintf(stderr, "WARNING: Tree given for the iteration is empty or it's pointer is NULL.\n");
        return NULL;
    }
    KD_iterator_container * container = malloc(sizeof(KD_iterator_container));
    container->iterator = NULL;
    container->number_of_elements = 0;

    KD_node * last_node = get_max_node(tree->root);
    KD_node * node = tree->root;
    size_t number_of_nodes = 0;
    recursion(container, node, last_node, &number_of_nodes);

    container->number_of_elements = number_of_nodes;

    return container;
}

Error KD_container_free(KD_iterator_container * container) {
    if (container == NULL) {
        fprintf(stderr, "NULL pointer container tried to be freed.\n");
        return FREEING_OF_NULL_PTR;
    }

    if (container->iterator)
        free(container->iterator);
    free(container);

    return IT_IS_OK;
}