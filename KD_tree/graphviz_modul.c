//
// Created by Stephan on 29.04.2022.
//
#include <stdlib.h>
#include "stdio.h"
#include "string.h"
#include "Errors.h"
#include "FGetLine.h"
#include "tree_dialog.h"
#include "SortsSearch.h"

const char l1[] = "digraph {\n";
const char l2[] = "\tnode [style=\"filled\", color=\"black\", fillcolor=\"skyblue\"];\n";
const char l3[] = " [label = \"";

Error init_graph() {
    FILE * file = fopen("graph.gv", "w");
    fwrite(l1, 1, strlen(l1), file);
    fwrite(l2, 1, strlen(l2), file);
    fwrite("}", 1, 1, file);

    fclose(file);

    return IT_IS_OK;
}

char * create_name(int ind) {
    char * name = calloc(1, sizeof(char) * 10);
    // itoa(ind, name+1, 10);
    sprintf(name+2, "%u", ind);
    name[0] = '\t';
    name[1] = 'a';
    return name;
}

//int comp(const void * _node1, const void * _node2) {
//    KeyType * key1 = (*((Node**) _node1))->key;
//    KeyType * key2 = (*((Node**) _node2))->key;
//    return key_cmp(key1, key2);
//}
void bst_print_dot(KD_node * tree, FILE* stream);
Error update_graph(KD_tree * tree) {
    if (tree == NULL)
        return NULL_PTR_IN_UNEXCITED_PLACE;

    /*
    FILE * file = fopen("graph.gv", "r+");
    if (file == NULL) {
        init_graph();
        file = fopen("graph.gv", "r+");
    }

    /////
    char ** lines = f_get_lines("test_g.gv");
    if (lines == NULL)
        return NULL_PTR_IN_UNEXCITED_PLACE;

    int i = 0;
    while (lines[i] && strcmp(lines[i], "}") != 0) { i++; }
    int len = 0;
    while (lines[len]) { len++; }
    if (lines[i] == NULL) {
        fprintf(stderr, "there is no \'}\' in .gv file\n");
        exit(NULL_PTR_IN_UNEXCITED_PLACE);
    }

    i--;
    lines = realloc(lines, sizeof(char *) * (len+2));
    memmove(lines+i+1, lines+i, sizeof(char *) * 2);

    char name[10];
    itoa(len/2, name, 10);
    size_t name_len = strlen(name);

    lines[i] = malloc(sizeof(char) * ( + 1));
     /////

    TreeIteratorContainer * container = create_iterator(tree);
    size_t number = container->number_of_elements;

    fwrite(l1, 1, strlen(l1), file);
    fwrite(l2, 1, strlen(l2), file);

    for (int i = 0; i < number; ++i) {
        char * name = create_name(i);
        size_t name_len = strlen(name);

        fwrite(name, 1, name_len, file);
        fwrite(l3, 1, strlen(l3), file);
        char data[50];
        sprintf(data, "%u", container->iterator[i]->info->val);
        // itoa((int) container->iterator[i]->info->val, data, 10);
        fwrite(data, 1, strlen(data), file);
        fwrite("\"];\n", 4, sizeof(char), file);
        free(name);
    }

    fwrite("\n", 1, 1, file);

    for (int i = 0; i < number; ++i) {
        char * name = create_name(i);
        size_t name_len = strlen(name);

        Node * node = container->iterator[i];
        size_t ind1 = -1;
        size_t ind2 = -1;
        if (node->left)
            ind1 = binarySearch(container->iterator, &(node->left), number, sizeof(Node*), comp);
        if (node->right)
            ind2 = binarySearch(container->iterator, &(node->right), number, sizeof(Node*), comp);

        if (ind1 > number || ind2 >= number)
            fprintf(stderr, "there is no some nodes in iter\n");

        if (ind1 != -1) {
            char * name1 = create_name((int) ind1);
            size_t name_len1 = strlen(name1);

            fprintf(file, "%s -> %s;\n", name, name1);
            free(name1);
        } else {
            fprintf(file, "\tnull%d%d [shape=point];\n", i, 1);
            fprintf(file, "%s -> null%d%d;\n", name,  i, 1);
        }

        if (ind2 != -1) {
            char * name2 = create_name((int) ind2);
            //size_t name_len2 = strlen(name2);

            fprintf(file, "%s -> %s;\n", name, name2);
            free(name2);
        } else {
            fprintf(file, "\tnull%d%d [shape=point];\n", i, 2);
            fprintf(file, "%s -> null%d%d;\n", name,  i, 2);
        }
        free(name);
    }

    fwrite("}", 1, 1, file);

    fclose(file);

    free_container(container);
    */

    FILE * stream = fopen("graph.gv", "w+");
    bst_print_dot(tree->root, stream);
    fclose(stream);

    return IT_IS_OK;
}

void concat(char ** str, char * x) {
    size_t len1 = strlen(*str);
    size_t len2 = strlen(x);

    *str = realloc(*str, sizeof(char) * (len1 + len2 + 1));
    memmove(*str + len1, x, (1+len2) * (sizeof(char)));
}

void print_node(KD_node * node, FILE * stream, char * subname) {
    fprintf(stream, "\tstruct%s [shape=none, margin=0, label=<\n<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\">\n", subname);
    for (size_t i = 0; i < node->number_of_items; ++i) {
        fprintf(stream, "\t<TR>");
        fprintf(stream, "<TD>Key: ");
        for (size_t j = 0; j < node->items[i]->number_of_dimensions; ++j) {
            fprintf(stream, "%u ", node->items[i]->key->keys[j]);
        }
        fprintf(stream, "</TD><TD> Info: %s, %ld</TD>", node->items[i]->info->str_info, node->items[i]->info->long_int_info);
        fprintf(stream, "</TR>\n");
    }
    if (node->number_of_items == 0) {
        fprintf(stream, "\t<TR>");
        fprintf(stream, "<TD>empty</TD>");
        fprintf(stream, "</TR>\n");
    }
    fprintf(stream, "\t</TABLE>>]\n");
}

void bst_print_dot_null(int nullcount, FILE* stream, char * subname) {
    fprintf(stream, "    struct%s -> null%d;\n", subname, nullcount);
}
void bst_print_dot_aux(KD_node * node, FILE* stream, char ** subname) {
    static int nullcount = 0;

    print_node(node, stream, *subname);
    if (node->left) {
        size_t old_len = strlen(*subname);

        concat(subname, "1");
        print_node(node, stream, *subname);
        *subname = realloc(*subname,  (old_len + 1) * sizeof(char));
        (*subname)[old_len] = '\0';

        fprintf(stream, "    struct%s -> struct%s1;\n", *subname, *subname);

        concat(subname, "1");
        bst_print_dot_aux(node->left, stream, subname);
        *subname = realloc(*subname,  (old_len + 1) * sizeof(char));
        (*subname)[old_len] = '\0';
    }
    else
        bst_print_dot_null(nullcount++, stream, *subname);

    if (node->right) {
        size_t old_len = strlen(*subname);

        //print_node(node, stream, subname);
        concat(subname, "2");
        print_node(node, stream, *subname);
        *subname = realloc(*subname,  (old_len + 1) * sizeof(char));
        (*subname)[old_len] = '\0';

        fprintf(stream, "    struct%s -> struct%s2;\n", *subname, *subname);

        concat(subname, "2");
        bst_print_dot_aux(node->right, stream, subname);
        *subname = realloc(*subname,  (old_len + 1) * sizeof(char));
        (*subname)[old_len] = '\0';
    }
    else
        bst_print_dot_null(nullcount++, stream, *subname);
}
void bst_print_dot(KD_node * tree, FILE* stream) {
    fprintf(stream, "digraph BST {\n");
    //fprintf(stream, "    node [shape=record fontname=\"Arial\"];\n");

    char * subname = calloc(1, sizeof(char));
    if (!tree)
        fprintf(stream, "\n");
    else if (!tree->right && !tree->left) {
        print_node(tree, stream, subname);
    }
    else
        bst_print_dot_aux(tree, stream, &subname);

    free(subname);

    fprintf(stream, "}\n");
}
