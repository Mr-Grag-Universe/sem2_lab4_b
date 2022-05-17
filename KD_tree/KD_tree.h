//
// Created by Stephan on 06.05.2022.
//

#ifndef LAB4B_PROT_KD_TREE_H
#define LAB4B_PROT_KD_TREE_H
#include "stdlib.h"
#include "stdio.h"
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
KD_key * KD_key_enter(size_t dimension);
Error KD_key_print(const KD_key * key);
int KD_key_bigger(KD_key * key1, KD_key * key2);
KD_key * KD_key_random(size_t number_of_dimensions, unsigned int min, unsigned int max);

//============INFO==============

typedef struct KD_info {
    char * str_info;
    long long_int_info;
} KD_info;
KD_info * KD_info_init(char * str_info);
Error KD_info_free(KD_info *);
KD_info * KD_info_enter();
Error KD_info_print(KD_info *);
KD_info * KD_info_random(size_t len);

//=============ITEM===============
struct KD_node;
typedef struct KD_item {
    size_t number_of_dimensions;
    KD_key * key;
    KD_info * info;
    struct KD_node * node;
} KD_item;
KD_item * KD_item_init(size_t number_of_dimension, KD_key*, KD_info*);
Error KD_item_free(KD_item *);
long KD_item_cmp(void *, void *, size_t dim);
Error KD_item_print(KD_item * item);
KD_item ** KD_item_random_array(size_t number_of_dimensions, unsigned int min, unsigned int max, size_t number, size_t len);
void KD_item_sort(KD_item ** items, size_t number, size_t dim);
unsigned int KD_item_arithmetic_mean(KD_item ** items, size_t number_of_items, size_t current_node_dimension_index);

//=============NODE=============

typedef struct KD_node {
    size_t current_node_dimension_index;

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
Error KD_node_free_items(KD_node * node);
KD_item * KD_node_get_item(const KD_node * node, unsigned int x);
Error KD_node_delete(KD_node * node, KD_key * key);
KD_node * get_max_node(const KD_node * node);
KD_node * get_min_node(const KD_node * node);

//===========TREE===========

struct KD_node_iterator_container;
struct KD_item_iterator_container;
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
KD_tree * KD_tree_init(size_t number_of_dimension, size_t dots_in_dimension, struct KD_item_iterator_container *);
Error KD_tree_free(KD_tree *);
Error KD_tree_add(KD_tree *, KD_item *);
Error KD_tree_delete(KD_tree *, KD_key *, size_t index_of_item);
size_t KD_BS(const KD_item ** items, size_t dim, size_t number_of_items, unsigned int x);
struct KD_node_iterator_container * KD_tree_get_node(const KD_tree *, KD_key *);
struct KD_item_iterator_container * KD_tree_get_items(const KD_tree * tree, KD_key * key);
Error KD_tree_print(const KD_tree *);
KD_item * KD_tree_closest_neighbour(const KD_tree * tree, KD_key * key);
KD_tree * read_tree(char * name_of_file);
Error update_graph(KD_tree * tree);
KD_tree * find_all_numbers(FILE * file);

//=============ITERATORS============

typedef struct KD_node_iterator_container {
    size_t number_of_elements;
    KD_node ** iterator;
} KD_node_iterator_container;
KD_node_iterator_container * KD_node_iterator_create(const KD_node * node);
Error KD_node_container_free(KD_node_iterator_container * container);

typedef struct KD_item_iterator_container {
    size_t number_of_elements;
    KD_item ** iterator;
} KD_item_iterator_container;
KD_item_iterator_container * KD_IIC_init(size_t number, KD_item ** items);
KD_item_iterator_container * KD_item_iterator_create(const KD_node * node);
Error KD_item_container_free(KD_item_iterator_container * container);

KD_item_iterator_container * KD_tree_create_SIIC(const KD_tree * tree, KD_key * key);


#endif //LAB4B_PROT_KD_TREE_H


