//
// Created by Stephan on 18.04.2022.
//

#ifndef LAB4_PROT_TREE_DIALOG_H
#define LAB4_PROT_TREE_DIALOG_H
#include "Errors.h"
#include "KD_tree.h"

Error add_tree_dialog(KD_tree * tree);
Error print_tree_dialog(const KD_tree * tree);
Error delete_tree_dialog(KD_tree * tree);
Error get_tree_dialog(const KD_tree * tree);
Error traversal_tree_dialog(const KD_tree* tree);
Error nearest_item_dialog(const KD_tree * tree);
//Error timing_dialog();
//Error random_dialog(KD_tree * tree);
//Error read_tree_dialog(KD_tree ** tree);
//Error number_of_words_in_file();
//Error dialog_update_graphviz(KD_tree * tree);
#endif //LAB4_PROT_TREE_DIALOG_H
