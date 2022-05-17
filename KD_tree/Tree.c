//
// Created by Stephan on 06.05.2022.
//
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "Errors.h"
#include "KD_tree.h"
#include "SortsSearch.h"
#include "MyMath.h"

size_t KD_BS(const KD_item ** items, size_t dim, size_t number_of_items, unsigned int x) {
    size_t left = 0, right = number_of_items;
    size_t size = sizeof(KD_item);
    void * array = items;
    while (left != right) {
        size_t m = (left+right)/2;
        void * p = (void*) ((size_t) array + size*m);
        if (x > items[m]->key->keys[dim]) left = m + 1;
        else if (x < items[m]->key->keys[dim]) right = m;
        else return m;
    }
    return left;
}

void recursion_init_add(KD_tree * tree, KD_item ** items, size_t number, size_t current_dim_ind) {
    if (number == 0)
        return;

    KD_item_sort(items, number, current_dim_ind);

    //printf("NEW ITERATION.\n");
    //for (size_t i = 0; i < number; ++i) {
    //    KD_item_print(items[i]);
    //}

    unsigned int x = KD_item_arithmetic_mean(items, number, current_dim_ind);
    size_t ind = KD_BS(items, current_dim_ind, number, x);
    if (ind >= number)
        ind = number-1;

    size_t n = tree->dots_in_node / 2;

    size_t start_ind = ind;
    size_t end_ind = ind;
    size_t store = tree->dots_in_node;
    for (size_t i = 0; i < n+1; ++i) {
        if (store == 0)
            break;
        if (start_ind) {
            start_ind--;
            store--;
        }
        if (store == 0)
            break;
        if (end_ind != number-1) {
            end_ind++;
            store--;
        }
    }

    for (size_t i = start_ind; i < end_ind || i == number-1; ++i) {
        tree->add(tree, items[i]);
        //KD_tree_print(tree);
    }

    if (number < 4) {
        return;
    }
    size_t new_dim = (current_dim_ind+1)%tree->number_of_dimensions;
    if (start_ind)
        recursion_init_add(tree, items, start_ind, new_dim);
    if (end_ind != number-1)
        recursion_init_add(tree, items+end_ind, number-end_ind, new_dim);
}

KD_tree * KD_tree_init(size_t number_of_dimension, size_t dots_in_dimension, KD_item_iterator_container * container) {
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

    if (container) {
        //for (size_t i = 0; i < container->number_of_elements; ++i) {
        //    KD_item_print(container->iterator[i]);
        //}
        recursion_init_add(tree, container->iterator, container->number_of_elements, 0);
    }

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

KD_node_iterator_container * KD_tree_get_node(const KD_tree * tree, KD_key * key) {
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
            if (!KD_key_cmp(node->items[i]->key, key, 1, 0)) {
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

    KD_node_iterator_container * container = malloc(sizeof(KD_node_iterator_container));
    container->number_of_elements = number_of_nodes;
    container->iterator = nodes;

    return container;
}

KD_item_iterator_container * KD_tree_get_items(const KD_tree * tree, KD_key * key) {
    if (key == NULL) {
        fprintf(stderr, "Entered key is null!\n");
        return NULL;
    }
    KD_node_iterator_container * container = KD_tree_get_node(tree, key);
    if (container == NULL || container->number_of_elements == 0 && container->iterator == NULL) {
        //printf("there is no such element in this tree.\n");
        if (container)
            KD_node_container_free(container);
        //KD_key_free(key);
        return NULL;
    }

    KD_item ** items = NULL;
    KD_node ** nodes = container->iterator;
    size_t number_of_nodes = container->number_of_elements;
    size_t number_of_eq = 0;
    for (size_t i = 0; i < number_of_nodes; ++i) {
        // можно улучшить ассимптотику через бинарный поиск
        for (size_t j = 0; j < nodes[i]->number_of_items; ++j) {
            if (!KD_key_cmp(nodes[i]->items[j]->key, key, 1, 0)) {
                items = realloc(items, sizeof(KD_item*) * (number_of_eq+1));
                items[number_of_eq] = nodes[i]->items[j];
                number_of_eq++;
            }
        }
    }
    KD_node_container_free(container);

    if (number_of_eq == 0) {
        fprintf(stderr, "item was found with get_node from tree but it didn't with get_item from node.\n");
        return NULL;
    }
    KD_item_iterator_container * res = malloc(sizeof(KD_item_iterator_container));
    res->iterator = items;
    res->number_of_elements = number_of_eq;

    return res;
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

    // инициализация - добавление первого элемента
    if (tree->root == NULL) {
        tree->root = KD_node_init(tree->dots_in_node, 0);
        KD_node_add(tree->root, item);
        return IT_IS_OK;
    }

//    KD_node_iterator_container * container = KD_tree_get_node(tree, item->key);
//    if (container->number_of_elements) {
//        KD_node_container_free(container);
//        return IT_IS_OK;
//    }
//    KD_node * node = container->iterator[container->number_of_elements-1];
//    KD_node_container_free(container);

    KD_node * node = tree->root;
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

    KD_node_iterator_container * container = KD_tree_get_node(tree, key);
    if (container->iterator == NULL || container->number_of_elements == 0) {
        KD_node_container_free(container);
        return NULL_PTR_IN_UNEXCITED_PLACE;
    }

    // можно добавить функцию распаковщик контейнеров
    KD_node ** nodes = container->iterator;
    size_t number_of_nodes = container->number_of_elements;

    size_t * indexes = NULL;
    KD_item ** items = NULL;
    size_t number_of_eq = 0;
    for (size_t i = 0; i < number_of_nodes; ++i) {
        // можно улучшить ассимптотику через бинарный поиск
        for (size_t j = 0; j < nodes[i]->number_of_items; ++j) {
            if (!KD_key_cmp(nodes[i]->items[j]->key, key, 1, 0)) {
                indexes = realloc(indexes, sizeof(size_t) * (number_of_eq+1));
                items = realloc(items, sizeof(KD_item*) * (number_of_eq+1));
                indexes[number_of_eq] = j;
                items[number_of_eq] = nodes[i]->items[j];
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

        KD_item_free(items[ind]);
        KD_node * node = items[ind]->node;
        memmove(node->items + ind, node->items + ind + 1, sizeof(KD_item*) * (node->number_of_items-ind-1));
        node->items[node->number_of_items-1] = NULL;
        node->number_of_items--;
    }

    KD_node_container_free(container);
    free(indexes);
    free(items);

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
            printf(": %s, %ld; ", item->info->str_info, item->info->long_int_info);
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

KD_item * recursion_nearest_neighbour(KD_node * node, KD_key * original_key, double * d) {
    if (node == NULL)
        return NULL;

    size_t dim = original_key->number_of_dimensions;
    KD_item * item1 = NULL;
    double min_d1 = 0;
    if (node->number_of_items != 0) {
        min_d1 = distance(node->items[0]->key->keys, original_key->keys, dim, U_INT);
    }
    for (size_t i = 0; i < node->number_of_items; ++i) {
        double new_d = distance(node->items[i]->key->keys, original_key->keys, dim, U_INT);
        if (min_d1 > new_d) {
            item1 = node->items[i];
            min_d1 = new_d;
        }
    }

    KD_item * item = NULL;
    double min_d2 = 0;
    if (node->left && node->right) {
        //unsigned int or_k = original_key->keys[node->current_node_dimension_index];
        //unsigned int t_k = temp_key->keys[node->current_node_dimension_index];

        KD_item * item_l = recursion_nearest_neighbour(node->left, original_key, d);
        KD_item * item_r = recursion_nearest_neighbour(node->right, original_key, d);

        double left_d = (item_l) ? distance(item_l->key->keys, original_key->keys, dim, U_INT) : -1;
        double right_d = (item_r) ? distance(item_r->key->keys, original_key->keys, dim, U_INT): -1;

        if (left_d && right_d && left_d < right_d && left_d < *d || left_d < *d && left_d != -1 && right_d == -1) {
            item = item_l;
            min_d2 = left_d;
        }
        else if (left_d && right_d && right_d < left_d && right_d < *d || right_d < *d && right_d != -1 && left_d == -1) {
            item = item_r;
            min_d2 = right_d;
        }
    }
    else if (node->left == NULL && node->right || node->right == NULL && node->left) {
        fprintf(stderr, "THERE IS ONLY 1 CHILD!!!\n");
        return NULL;
    }

    if (item == NULL && item1 == NULL)
        return NULL;
    if (item == NULL && item1) {
        if (min_d1 < *d) {
            *d = min_d1;
            return item1;
        }
        return NULL;
    }
    if (item1 == NULL && item) {
        if (min_d2 < *d) {
            *d = min_d2;
            return item;
        }
        return NULL;
    }
    if (min_d1 < min_d2) {
        *d = min_d1;
        return item1;
    }

    *d = min_d2;
    return item;
}

KD_item * KD_tree_closest_neighbour(const KD_tree * tree, KD_key * key) {
    if (tree == NULL || tree->root == NULL || tree->root->number_of_items == 0)
        return NULL;

    KD_item * item_start = tree->root->items[0];
    double r = distance(key->keys, item_start->key->keys, key->number_of_dimensions, U_INT);

    KD_item * item = recursion_nearest_neighbour(tree->root, key, &r);
    if (item == NULL)
        return item_start;

    return item;
}

KD_tree * KD_tree_random(size_t number_of_dimensions, size_t dots_in_dimension, size_t number, unsigned int min, unsigned int max, size_t len_of_data) {
    if (min > max) {
        return NULL;
    }

    KD_item ** items = KD_item_random_array(number, min, max, number, len_of_data);
    // KD_tree * tree = KD_tree_init(number_of_dimensions, dots_in_dimension, items);

    return NULL; //tree;
}