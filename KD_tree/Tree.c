//
// Created by Stephan on 06.05.2022.
//
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "Errors.h"
#include "KD_tree.h"
#include "SortsSearch.h"

size_t KD_BS(const KD_node * node, unsigned int x) {
    size_t dim = node->current_node_dimension_index;
    size_t left = 0, right = node->number_of_items;
    size_t size = sizeof(KD_item);
    void * array = node->items;
    while (left != right) {
        size_t m = (left+right)/2;
        void * p = (void*) ((size_t) array + size*m);
        if (x > node->items[m]->key->keys[dim]) left = m + 1;
        else if (x < node->items[m]->key->keys[dim]) right = m;
        else return m;
    }
    return left;
}

KD_tree * KD_tree_init(size_t number_of_dimension, size_t dots_in_dimension, long (**cmp_funcs)(void*, void*)) {
    KD_tree * tree = malloc(sizeof(KD_tree));
    if (tree == NULL) {
        fprintf(stderr, "Tree is not enough memory for tree initialisation.\n");
        exit(MEMORY_OVERFLOW);
    }

    tree->depth = 0;
    tree->dots_in_node = dots_in_dimension;
    tree->number_of_dimensions = number_of_dimension;
    tree->root = NULL;

    // мб есть более корректный вариант выделения памяти
    //tree->cmp_funcs = malloc(sizeof(void*) * number_of_dimension);
    //for (size_t i = 0; i < number_of_dimension; ++i) {
        //tree->cmp_funcs[i] = cmp_funcs[i];
    //}
    tree->cmp_funcs = NULL;

    tree->free = KD_tree_free;
    tree->add = KD_tree_add;

    return tree;
}

void recursion_free(KD_node * node) {
    if (node == NULL)
        return;

    recursion_free(node->left);
    recursion_free(node->right);

    node->left = NULL;
    node->right = NULL;

    KD_node_free_items(node);
    KD_node_free(node);
}

Error KD_tree_free(KD_tree * tree) {
    if (tree == NULL) {
        fprintf(stderr, "NULL-pointer tree was tried to get free.\n");
        return FREEING_OF_NULL_PTR;
    }

    recursion_free(tree->root);
    //free(tree->cmp_funcs);
    free(tree);

    return IT_IS_OK;
}

KD_iterator_container * KD_tree_get_node(const KD_tree * tree, KD_key * key) {
    if (tree == NULL) {
        fprintf(stderr, "tree ptr is NULL in adding.\n");
        return NULL;
    }

    KD_node * node = tree->root;
    if (node == NULL) {
        return NULL;
    }

    size_t ind = 0;
    KD_node ** nodes = NULL;
    size_t number_of_nodes = 0;

    while (node) {
        size_t d = node->current_node_dimension_index;

        for (size_t i = 0; i < node->number_of_items; ++i)
            if (!KD_key_cmp(node->items[ind]->key, key, 1, 0)) {
                number_of_nodes++;
                nodes = realloc(nodes, sizeof(KD_node*) * number_of_nodes);
                nodes[number_of_nodes-1] = node;
            }


        if (key->keys[d] > node->face) {
            if (node->right)
                node = node->right;
            else
                break;
        } else {
            if (node->left)
                node = node->left;
            else
                break;
        }
    }

    KD_iterator_container * container = malloc(sizeof(KD_iterator_container));
    container->number_of_elements = number_of_nodes;
    container->iterator = nodes;

    return container;
}

Error KD_tree_add(KD_tree * tree, KD_item * item) {
    if (tree == NULL) {
        fprintf(stderr, "tree ptr is NULL in adding.\n");
        return NULL_PTR_IN_UNEXCITED_PLACE;
    }
    if (item == NULL) {
        fprintf(stderr, "item ptr is NULL in adding.\n");
        return NULL_PTR_IN_UNEXCITED_PLACE;
    }

    if (tree->root == NULL) {
        tree->root = KD_node_init(tree->dots_in_node, 0);
        KD_node_add(tree->root, item);
        return IT_IS_OK;
    }

    KD_iterator_container * container = KD_tree_get_node(tree, item->key);
    if (container->number_of_elements) {
        KD_container_free(container);
        return IT_IS_OK;
    }
    KD_node * node = container->iterator[container->number_of_elements-1];
    KD_container_free(container);

    node = tree->root;
    size_t ind = 0;
    while (node) {
        size_t d = node->current_node_dimension_index;

        if (node->left == NULL && node->right == NULL)
            break;

        if (item->key->keys[d] > node->face) {
            if (node->right)
                node = node->right;
            else
                break;
        } else {
            if (node->left)
                node = node->left;
            else
                break;
        }
    }

    Error report = 0;
    report = KD_node_add(node, item);

    return report;
}

Error KD_tree_delete(KD_tree * tree, KD_key * key, size_t index_of_item) {
    if (tree == NULL || key == NULL) {
        fprintf(stderr, "NULL tree or key in delete func.\n");
        return WRONG_INPUT;
    }

    KD_iterator_container * container = KD_tree_get_node(tree, key);
    if (container->iterator == NULL || container->number_of_elements == 0) {
        KD_container_free(container);
        return NULL_PTR_IN_UNEXCITED_PLACE;
    }
    KD_node ** nodes = container->iterator;
    size_t number_of_nodes = container->number_of_elements;

    size_t * indexes = NULL;
    size_t number_of_eq = 0;
    for (size_t i = 0; i < number_of_nodes; ++i) {
        // можно улучшить ассимптотику через бинарный поиск
        for (size_t j = 0; j < nodes[i]->number_of_items; ++j) {
            if (KD_key_cmp(nodes[i]->items[j]->key, key, 1, 0)) {
                indexes = realloc(indexes, sizeof(size_t) * (number_of_eq+1));
                indexes[number_of_eq] = j;
                number_of_eq++;
            }
        }
    }

    if (number_of_eq == 0) {
        fprintf(stderr, "item was found with get_node from tree but it didn't with get_item from node.\n");
        return NULL_PTR_IN_UNEXCITED_PLACE;
    }

    Error report = 0;
    if (number_of_eq == 1) {
        report = KD_node_delete(nodes[0], key);
    } else {
        if (number_of_eq <= index_of_item) {
            free(indexes);
            fprintf(stderr, "too big index.\n");
            return WRONG_INPUT;
        }
        size_t ind = indexes[index_of_item];

        KD_item_free(node->items[ind]);
        memmove(node->items + ind, node->items + ind + 1, sizeof(KD_item*) * (node->number_of_items-ind-1));
        node->items[node->number_of_items-1] = NULL;
        node->number_of_items--;
    }

    return report;
}

size_t KD_tree_depth(KD_node * node) {
    if (node == NULL)
        return 0;
    size_t left_depth = 1+KD_tree_depth(node->left);
    size_t right_depth = 1+KD_tree_depth(node->right);
    return (left_depth > right_depth) ? left_depth: right_depth;
}

Error KD_tree_recursion_print(KD_node * node, char ** offset) {
    if (offset == NULL || *offset == NULL)
        return NULL_PTR_IN_UNEXCITED_PLACE;

    size_t l = strlen(*offset);
    if (l>0 && (*offset)[l-1] != ' ' || (*offset)[0] == '\0') {
        printf("%s\n", *offset);
        printf("%s\\", *offset);
    }
    else {
        (*offset)[l-1] = '|';
        printf("%s\n", *offset);
        (*offset)[l-1] = ' ';
        printf("%s\\", *offset);
    }
    if (node)
        for (size_t i = 0; i < node->number_of_items; ++i) {
            KD_item * item = node->items[i];
            printf("key: ");
            for (size_t j = 0; j < item->number_of_dimensions; ++j)
                printf("%u ", item->key->keys[j]);
            printf(": %s; ", item->info->str_info);
        }
    else
        printf("-");
    printf("\n");

    if (node == NULL || node->left == NULL && node->right == NULL)
        return IT_IS_OK;

    size_t len = strlen(*offset)+2;
    *offset = realloc(*offset, sizeof(size_t) * len);
    (*offset)[len-1] = '\0';
    (*offset)[len-2] = '|';

    KD_tree_recursion_print(node->left, offset);
    (*offset)[len-2] = ' ';
    KD_tree_recursion_print(node->right, offset);
    len--;
    *offset = realloc(*offset, sizeof(char) * len);
    (*offset)[len-1] = '\0';

    return IT_IS_OK;
}

Error KD_tree_print(const KD_tree * tree) {
    if (tree == NULL) {
        fprintf(stderr, "this tree ptr is NULL.\n");
        printf("this tree ptr is NULL.\n");
        return NULL_PTR_IN_UNEXCITED_PLACE;
    }
    if (tree->root == NULL) {
        printf("this tree is empty!\n");
        return IT_IS_OK;
    }

    char * offset = malloc(sizeof(char));
    offset[0] = '\0';
    KD_tree_recursion_print(tree->root, &offset);
    free(offset);

    return IT_IS_OK;
}
