cmake_minimum_required(VERSION 3.16)
project(lab4b_prot C)

set(CMAKE_C_STANDARD 99)

include_directories(segmental_tree libs libs/headers KD_tree tree_dialog)

add_subdirectory(segmental_tree)
add_subdirectory(KD_tree)
add_subdirectory(libs)
add_subdirectory(tree_dialog)

add_executable(lab4b_prot main.c)
target_link_libraries(lab4b_prot segmental_tree)
target_link_libraries(KD_tree mylib)
target_link_libraries(lab4b_prot mylib)
target_link_libraries(lab4b_prot KD_tree)
target_link_libraries(dialog KD_tree)
target_link_libraries(dialog mylib)
target_link_libraries(lab4b_prot dialog)
