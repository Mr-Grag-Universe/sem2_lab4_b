//
// Created by Stephan on 18.04.2022.
//
#include <time.h>
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/time.h"
#include "Errors.h"
#include "KD_tree.h"
#include "KGetLine.h"
#include "FGetLine.h"
#include "MyString.h"

Error add_tree_dialog(KD_tree * tree) {
    printf("Enter your key: ");
    KD_key * key = KD_key_enter(tree->number_of_dimensions);
    if (key == NULL) {
        fprintf(stderr, "Entered key is null!\n");
        return NULL_PTR_IN_UNEXCITED_PLACE;
    }

    printf("Enter your info: ");
    KD_info * info = KD_info_enter();
    if (info == NULL) {
        fprintf(stderr, "Entered info is null!\n");
        return NULL_PTR_IN_UNEXCITED_PLACE;
    }

    KD_item * item = KD_item_init(tree->number_of_dimensions, key, info);

    return KD_tree_add(tree, item);
}

Error delete_tree_dialog(KD_tree * tree) {
    printf("Enter your key: ");
    KD_key * key = KD_key_enter(tree->number_of_dimensions);
    if (key == NULL) {
        fprintf(stderr, "Entered key is null!\n");
        return NULL_PTR_IN_UNEXCITED_PLACE;
    }

    KD_node_iterator_container * container = KD_tree_get_node(tree, key);
    if (container == NULL) {
        KD_key_free(key);
        return NULL_PTR_IN_UNEXCITED_PLACE;
    }

    if (container->number_of_elements == 0) {
        printf("there is not elements with such keys.\n");
        KD_key_free(key);
        KD_node_container_free(container);
        return IT_IS_OK;
    }
    Error report = 0;
    if (1) {
        printf("There are several dots with the same keys. Please, chose only one of them.\n");
        size_t ind = container->number_of_elements;
        while (ind >= container->number_of_elements) {
            ind = get_int();
            if (ind >= container->number_of_elements) {
                printf("too big index. do you want to continue? (y/n): ");
                char * answer = get_line();
                if (answer == NULL) {
                    KD_node_container_free(container);
                    KD_key_free(key);
                    return IT_IS_OK;
                }
                else if (strcmp("y", answer) != 0) {
                    KD_node_container_free(container);
                    KD_key_free(key);
                    return IT_IS_OK;
                }
                else {
                    continue;
                }
            }
        }
        report = KD_tree_delete(tree, key, ind);
    }
    KD_node_container_free(container);
    KD_key_free(key);

    return report;
}

Error print_tree_dialog(const KD_tree * tree) {
    return KD_tree_print(tree);//print_BT(tree);
}

Error get_tree_dialog(const KD_tree * tree) {
    printf("Enter your key: ");
    KD_key * key = KD_key_enter(tree->number_of_dimensions);
    if (key == NULL) {
        fprintf(stderr, "Entered key is null!\n");
        return NULL_PTR_IN_UNEXCITED_PLACE;
    }

//    KD_node * node = KD_tree_get_node(tree, key);
//    if (node == NULL) {
//        fprintf(stderr, "there is no such element in this tree.\n");
//        return NULL_PTR_IN_UNEXCITED_PLACE;
//    }
//    KD_info * info = node->items[KD_BS(node, key->keys[node->current_node_dimension_index])]->info;
//    KD_key_free(key);
//
//    if (!KD_info_print(info) || !KD_key_print(key))
//        return WRONG_INPUT;

    KD_item_iterator_container * container = KD_tree_get_items(tree, key);
    if (container == NULL || container->number_of_elements == 0 && container->iterator == NULL) {
        printf("there is no such element in this tree.\n");
        if (container)
            KD_item_container_free(container);
        KD_key_free(key);
        return IT_IS_OK;
    }

    size_t number_of_items = container->number_of_elements;
    for (size_t i = 0; i < number_of_items; ++i) {
        KD_item_print(container->iterator[i]);
    }
    KD_item_container_free(container);
    KD_key_free(key);

    return IT_IS_OK;
}

Error traversal_tree_dialog(const KD_tree * tree) {
    printf("Do you wanna get all items or items, which keys bigger then entered.\n1) all\t2) some\n");
    int chose_all_or_bigger_then_key = 0;
    while (chose_all_or_bigger_then_key <= 0 || chose_all_or_bigger_then_key > 2) {
        chose_all_or_bigger_then_key = get_int();
        if (chose_all_or_bigger_then_key <= 0 || chose_all_or_bigger_then_key > 2) {
            printf("your answer is wrong. do you want to continue? (y/n): ");
            char * answer = get_line();
            if (answer == NULL || strcmp("y", answer) != 0) {
                return IT_IS_OK;
            }
        }
    }

    switch (chose_all_or_bigger_then_key) {
        case 1: {
            KD_item_iterator_container * container = KD_item_iterator_create(tree->root);
            for (size_t i = 0; i < container->number_of_elements; ++i) {
                KD_item_print(container->iterator[i]);
            }
            KD_item_container_free(container);
            break;
        }
        case 2: {
            KD_key * key = KD_key_enter(tree->number_of_dimensions);

            KD_item_iterator_container * container = KD_tree_create_SIIC(tree, key);
            if (container == NULL || container->number_of_elements == 0) {
                printf("there is not dots with such key.\n");
                if (container)
                    KD_item_container_free(container);
                return IT_IS_OK;
            }
            for (size_t i = 0; i < container->number_of_elements; ++i) {
                KD_item_print(container->iterator[i]);
            }

            KD_item_container_free(container);
            KD_key_free(key);

            break;
        }
        default: {
            fprintf(stderr, "input Error in traversal.\n");
            return WRONG_INPUT;
        }
    }


    return IT_IS_OK;
}

Error nearest_item_dialog(const KD_tree * tree) {
    if (tree == NULL) {
        fprintf(stderr, "NULL tree in nearest item.\n");
        return NULL_PTR_IN_UNEXCITED_PLACE;
    }

    printf("Enter your key.\n(Number of dimensions = %ld)\n", tree->number_of_dimensions);
    KD_key * key = KD_key_init(0, NULL);
    while (key->keys == NULL) {
        KD_key_free(key);
        key = KD_key_enter(tree->number_of_dimensions);
    }

    KD_item * item = KD_tree_closest_neighbour(tree, key);

    KD_key_free(key);

    if (item == NULL) {
        printf("empty tree.\n");
        return IT_IS_OK;
    }

    KD_item_print(item);

    return IT_IS_OK;
}

//Error timing_dialog() {
//    srand(time(NULL));
//
//    long double * table_of_results = malloc(sizeof(long double) * 100);
//
//    for (int i = 0; i < 100; ++i) {
//        size_t number_of_elements = 10000*(i+1);
//        printf("number: %ld\n", number_of_elements);
//
//
//        Node * node = NULL;
//        Tree *tree = init_tree();
//        if (tree == NULL) {
//            fprintf(stderr, "timing allocing wrong!\n");
//            return MEMORY_OVERFLOW;
//        }
//        printf("start filling\n");
//        fill_tree_with_random_data(tree, number_of_elements, 0, 100, 5);
//        printf("finish filling\n");
//
//        /*
//        // traversal time
//        start = clock();
//        TreeIteratorContainer * container = create_iterator(tree);
//        finish = clock();
//        traversal_time = (traversal_time + (double) (finish-start) / CLOCKS_PER_SEC) / 2;
//        free_container(container);
//         */
//
//        long double search_time = 0;
//        size_t number_of_checking = 10000;
//        size_t * results = malloc(sizeof(size_t) * number_of_checking);
//
//        for (int j = 0; j < number_of_checking; ++j) {
//            // searching time
//            KeyType *key = generate_key(5);
//
//            struct timeval start;
//            struct timeval finish;
//            //printf("start find\n");
//            gettimeofday(&start, NULL);
//            node = get_node_from_BT(tree, key);
//            gettimeofday(&finish, NULL);
//            //print_node(node);
//
//            results[j] = ((finish.tv_sec-start.tv_sec)*1000000 + (finish.tv_usec-start.tv_usec) );
//            if (results[j] > 100000) {
//                printf("throughout\n");
//            }
//            //search_time = (search_time + (finish.tv_sec-start.tv_sec)*1000 + (finish.tv_usec-start.tv_usec) )/ 2;
//            //printf("%ld\n", search_time);
//            //printf("finish find\n");
//            free_key(key);
//        }
//        free_BT(tree);
//        for (size_t j = 0; j < number_of_checking; ++j) {
//            search_time = search_time + (double)results[j]/1000;// / 2;
//        }
//        search_time /= number_of_checking;
//        free(results);
//        table_of_results[i] = search_time;
//
//        // printf("traversal time: %.20Lf\n", traversal_time);
//        printf("search time %Lf\n", search_time);
//    }
//
//    for (size_t i = 0; i < 100; ++i) {
//        printf("%ld: %Lf\n", (i+1)*10000, table_of_results[i]);
//    }
//    free(table_of_results);
//
//    return IT_IS_OK;
//}

Error random_dialog(KD_tree ** tree) {
    int answer = y_n_question_answer("do you really wanna erase current tree?\n");
    if (answer) {
        size_t number_of_dimensions = (*tree)->number_of_dimensions;
        size_t dots_in_node = (*tree)->dots_in_node;
        KD_tree_free(*tree);
        KD_item ** items = KD_item_random_array(number_of_dimensions, 0, 100, 30, 5);
        KD_item_iterator_container * container = KD_IIC_init(30, items);
        *tree = KD_tree_init(number_of_dimensions, dots_in_node, container);
        KD_item_container_free(container);
        // free(items);
    }
    return IT_IS_OK;
}

Error read_tree_dialog(KD_tree ** tree) {
    printf("Do you really want to delete your current tree?\n");
    char * answer = get_line();
    if (!(!strcmp(answer, "y") || !strcmp(answer, "yes"))) {
        if (answer)
            free(answer);
        return IT_IS_OK;
    }
    free(answer);

    KD_tree_free(*tree);

    *tree = read_tree("tree.txt");
    if (*tree == NULL)
        return NULL_PTR_IN_UNEXCITED_PLACE;

    return IT_IS_OK;
}

//Error number_of_words_in_file() {
//    printf("enter the name of your txt file: \n");
//    char * file_name = get_line();
//    if (file_name == NULL || file_name[0] == '\0') {
//        if (file_name)
//            free(file_name);
//        return WRONG_INPUT;
//    }
//
//    FILE * file = fopen(file_name, "r");
//    if (file == NULL) {
//        free(file_name);
//        return WRONG_INPUT;
//    }
//
//    char ** lines = f_get_lines(file_name);
//    fclose(file);
//    free(file_name);
//    if (lines == NULL) {
//        return NULL_PTR_IN_UNEXCITED_PLACE;
//    }
//
//    int i = 0;
//    while (lines[i++]) {}
//        //printf("%s\n", lines[i-1]);
//
//    Tree * tree = init_tree();
//    for (int j = 0; j < i; ++j) {
//        char * line = lines[j];
//        if (line) {
//            char ** words = split(line, " \t\n.,;\")(");
//            if (words == NULL) {
//                fprintf(stderr, "NULL in the splitting of a not NULL line\n");
//                free(line);
//                continue;
//                //exit(NULL_PTR_IN_UNEXCITED_PLACE);
//            }
//            int k = 0;
//            while (words[k++]) {
//                //printf("word%d: %s\n", k-1, words[k-1]);
//                KeyType * key = init_key();
//                key->strKey = words[k-1];
//                line_to_lower(key->strKey);
//                print_key(key);
//                getchar();
//                key->keySize = strlen(key->strKey)+1;
//                Node * node = get_node_from_BT(tree, key);
//                if (node) {
//                    free_key(key);
//                    node->info->val++;
//                } else {
//                    InfoType * info = init_info();
//                    info->val = 1;
//                    add_el_into_BT(tree, key, info);
//                }
//            }
//            free(words);
//        }
//        free(line);
//    }
//    free(lines);
//
//    TreeIteratorContainer * container = create_iterator(tree);
//    for (int j = 0; j < container->number_of_elements; ++j) {
//        print_node(container->iterator[j]);
//    }
//    free_container(container);
//    free_BT(tree);
//
//    return IT_IS_OK;
//}
//
//Error dialog_update_graphviz(KD_tree * tree) {
//    printf("enter your output file's name: ");
//    char * out = get_line();
//    if (out == NULL)
//        return WRONG_INPUT;
//    Error report = 0; // update_graph(tree);
//
//    size_t len = strlen(out);
//
//    char * c1 = "dot -v -Tpng -o ";
//    char * c2 = " graph.gv";
//    size_t c1_len = strlen(c1);
//    size_t c2_len = strlen(c2);
//
//    char * command = calloc(len + c1_len + c2_len + 1, sizeof(char));
//    memmove(command, c1, c1_len + 1);
//    memmove(command + c1_len, out, len + 1);
//    memmove(command + c1_len + len, c2, c2_len + 1);
//
//    system(command);
//    //system("dot -v -Tpng -o png1.png test_g.gv");
//    if (report == IT_IS_OK)
//        remove("graph.gv");
//
//    free(command);
//    free(out);
//
//    return report;
//}