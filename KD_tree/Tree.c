//
// Created by Stephan on 06.05.2022.
//
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "Errors.h"
#include "KD_tree.h"
#include "SortsSearch.h"

size_t KD_BS(KD_node * node, unsigned int x, size_t dim) {
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
    tree->cmp_funcs = malloc(sizeof(void*) * number_of_dimension);
    for (size_t i = 0; i < number_of_dimension; ++i) {
        tree->cmp_funcs[i] = cmp_funcs[i];
    }

    tree->free = KD_tree_free;
    tree->add = KD_tree_add;

    return tree;
}

void recursion_free(KD_node * node) {
    if (node == NULL)
        return;

    recursion_free(node->left);
    recursion_free(node->right);

    KD_node_free(node);
}

Error KD_tree_free(KD_tree * tree) {
    if (tree == NULL) {
        fprintf(stderr, "NULL-pointer tree was tried to get free.\n");
        return FREEING_OF_NULL_PTR;
    }

    recursion_free(tree->root);

    return IT_IS_OK;
}

KD_node * KD_tree_get_node(KD_tree * tree, KD_key * key) {
    if (tree == NULL) {
        fprintf(stderr, "tree ptr is NULL in adding.\n");
        return NULL;
    }

    KD_node * node = tree->root;
    if (node == NULL) {
        return NULL;
    }

    size_t ind = 0;
    while (node) {
        size_t d = node->dimension;
        ind = KD_BS(node, key->keys[d], d);
        if (ind < node->number_of_items) {
            if (!KD_key_cmp(node->items[ind]->key, key, 1, 0)) {
                return node;
            }
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

    if (ind > node->number_of_items)
        fprintf(stderr, "strange thing in getting of node.\n");

    return NULL;
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
        tree->root = KD_node_init(tree->dots_in_node, tree->number_of_dimensions);
        KD_node_add(tree->root, item);
    }

    KD_node * node = KD_tree_get_node(tree, item->key);
    if (node) {
        return IT_IS_OK;
    }

    node = tree->root;
    size_t ind = 0;
    while (node) {
        size_t d = node->dimension;

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
            printf(": %s; ", item->info->str_key);
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