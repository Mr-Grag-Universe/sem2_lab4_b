#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdbool.h"
#include "KGetLine.h"
#include "time.h"
#include "MyString.h"
#include "commands.h"
#include "Errors.h"
#include "KD_tree.h"
#include "tree_dialog.h"

void clrscr(){
    system("@cls||clear");
}

char ** init_menu_points() {
    char ** menu_points = malloc(sizeof(char*) * (UNKNOWN_COMMAND));
    menu_points[EXIT] = "exit";
    menu_points[ADD_EL] = "add a new element";
    menu_points[DELETE_EL] = "delete an element";
    menu_points[FIND_EL] = "find an element";
    menu_points[PRINT_TREE] = "print tree";
    menu_points[DELETE_ALl] = "delete all";
    menu_points[TRAVERSAL] = "tree traversal";
    menu_points[NEAREST] = "find the nearest element";
    menu_points[TIMING] = "check out time";// of traversal and searching in BT";
    menu_points[RANDOM_GENERATE] = "generate random tree data";
    menu_points[READ_TREE] = "read tree from file";
    menu_points[COUNT_WORDS_IN_FILE] = "calculate how many each word is in file";
    menu_points[GRAPHVIZ] = "update graphviz png";
    return menu_points;
}

void print_menu(char ** menu) {
    printf("This is menu:\n");
    for (int i = 0; i < UNKNOWN_COMMAND; ++i) {
        printf("%d) %s\n", i, menu[i]);
    }
}

bool check_command(char * command) {

    return true;
}

bool execute_command(KD_tree ** tree, Command command) {
    switch (command) {
        case EXIT: {
            printf("OK. Goodbye!\n");
            return true;
        }
        case ADD_EL: {
            add_tree_dialog(*tree);
            return false;
        }
        case FIND_EL: {
            if (get_tree_dialog(*tree) != IT_IS_OK)
                printf("Wrong input or something else.\n");
            return false;
        }
        case DELETE_EL: {
            delete_tree_dialog(*tree);
            return false;
        }
        case PRINT_TREE: {
            print_tree_dialog(*tree);
            return false;
        }
        case DELETE_ALl: {
            KD_tree_free(*tree);
            *tree = KD_tree_init(2, 3, NULL);
            return false;
        }
        case TRAVERSAL: {
            traversal_tree_dialog(*tree);
            char * answer = get_line();
            free(answer);
            return false;
        }
        case NEAREST: {
            nearest_item_dialog(*tree);
            return false;
        }
        case TIMING: {
            //timing_dialog();
            return false;
        }
        case RANDOM_GENERATE: {
            random_dialog(tree);
            return false;
        }
        case READ_TREE: {
            read_tree_dialog(tree);
            return false;
        }
        case COUNT_WORDS_IN_FILE: {
            //number_of_words_in_file();
            return false;
        }
        case GRAPHVIZ: {
            //dialog_update_graphviz(*tree);
            return false;
        }
        default: {
            printf("Your command is wrong!\n");
            return false;
        }
    }
    return false;
}

Command get_command_code(char * command) {
    if (is_digit(command)) {
        long int x = strtol(command, NULL, 10);
        if (x < 0 || x > UNKNOWN_COMMAND)
            return UNKNOWN_COMMAND;
        return x;
    }

    if (!strcmp(command, "exit"))
        return EXIT;
    else if (!strcmp(command, "get"))
        return FIND_EL;
    else if (!strcmp(command, "add"))
        return ADD_EL;
    else if (!strcmp(command, "delete"))
        return DELETE_EL;
    else if (!strcmp(command, "print"))
        return PRINT_TREE;
    else if (!strcmp(command, "delete all"))
        return DELETE_ALl;
    else if (!strcmp(command, "traversal"))
        return TRAVERSAL;
    else if (!strcmp(command, "nearest"))
        return NEAREST;
    else if (!strcmp(command, "time"))
        return TIMING;
    else if (!strcmp(command, "read"))
        return READ_TREE;
    else if (!strcmp(command, "counter"))
        return COUNT_WORDS_IN_FILE;
    else if (!strcmp(command, "graphviz"))
        return GRAPHVIZ;
    else return UNKNOWN_COMMAND;
}

KD_tree * start_session() {
    KD_tree * tree = KD_tree_init(2, 3, NULL);
    return tree;
}

int main() {
    printf("Info: %ld, Node: %ld\n", sizeof(KD_info), sizeof(KD_node));
    printf("Tree: %ld\n", sizeof(KD_tree));
    srand(time(NULL));
    bool finish = false;
    char ** menu = init_menu_points();

    KD_tree * tree = start_session();
    if (tree == NULL) {
        fprintf(stderr, "tree is NULL.\nSomething came wrong.\nCheck creation or initial data\n");
        exit(NULL_PTR_IN_UNEXCITED_PLACE);
    }

    while (!finish) {
        /// clrscr();
        print_menu(menu);

        char * command = get_line();
        if (command[0] == '\0') {
            free(command);
            continue;
        }
        //delete_double_spaces(&command);

        if (check_command(command) == false) {
            printf("Your command is wrong.");
            free(command);
            continue;
        }

        Command command_code = get_command_code(command);

        finish = execute_command(&tree, command_code);

        free(command);
    }
    KD_tree_free(tree);
    free(menu);

    return 0;
}