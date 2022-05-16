project(KD_tree)

add_library(KD_tree KD_tree.h Tree.c Key.c Node.c Info.c Item.c iterator.c file_input.c graphviz_modul.c file_work.c)
target_link_libraries(KD_tree -g)