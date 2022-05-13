project(KD_tree)

add_library(KD_tree KD_tree.h Tree.c Key.c Node.c Info.c Item.c iterator.c)
target_link_libraries(KD_tree -g)