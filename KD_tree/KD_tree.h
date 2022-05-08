//
// Created by Stephan on 06.05.2022.
//

#ifndef LAB4B_PROT_KD_TREE_H
#define LAB4B_PROT_KD_TREE_H
#include "stdlib.h"
#include "stdbool.h"
#include "Errors.h"

//============KEY==============

/// key in kd-try
/// default - unsigned int
typedef struct KD_key {
    size_t number_of_dimensions;
    unsigned int * keys;
} KD_key;
KD_key * KD_key_init(size_t number_of_dimensions, unsigned int * keys);
Error KD_key_free(KD_key * key);
long KD_key_cmp(KD_key *, KD_key *, bool each_x , size_t index_of_dimension);

//============INFO==============

typedef struct KD_info {
    char * str_key;
} KD_info;
KD_info * KD_info_init();
Error KD_info_free(KD_info *);

//=============ITEM===============

typedef struct KD_item {
    size_t number_of_dimensions;
    KD_key * key;
    KD_info * info;
} KD_item;
KD_item * KD_item_init(size_t number_of_dimension, KD_key*, KD_info*);
long KD_item_cmp(void *, void *, size_t dim);

//=============NODE=============

typedef struct KD_node {
    size_t dimension;

    size_t max_number_of_items;
    size_t number_of_items;
    KD_item ** items;

    struct KD_node * left;
    struct KD_node * right;

    unsigned int face;
} KD_node;
KD_node * KD_node_init(size_t number_of_items, size_t dimension);
///free node with it's items pointers array, but does not free items with themself
Error KD_node_free(KD_node *);
Error KD_node_add(KD_node * node, KD_item * item);

//===========TREE===========

typedef struct KD_tree {
    size_t depth;
    size_t dots_in_node;
    size_t number_of_dimensions;
    KD_node * root;

    long (**cmp_funcs)(void *, void *);

    // functions
    Error (* free)(struct KD_tree *);
    Error (* add)(struct KD_tree *, KD_item *);
} KD_tree;
KD_tree * KD_tree_init(size_t number_of_dimension, size_t dots_in_dimension, long (**cmp_funcs)(void*, void*));
Error KD_tree_free(KD_tree *);
Error KD_tree_add(KD_tree *, KD_item *);
size_t KD_BS(KD_node * node, unsigned int x, size_t dim);
KD_node * KD_tree_get_node(KD_tree *, KD_key *);
Error KD_tree_print(const KD_tree *);
#endif //LAB4B_PROT_KD_TREE_H
