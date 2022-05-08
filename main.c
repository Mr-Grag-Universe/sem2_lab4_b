#include <stdio.h>
#include "KD_tree.h"

long cmp_u_i(void * x1, void * x2) {
    if (x1 == NULL || x2 == NULL) {
        fprintf(stderr, "NULL keys were compared.\n");
        if (x1 == NULL && x2)
            return -1;
        else if (x1)
            return 1;
        return 0;
    }
    return *((long long*) x1) - *((long long*) x2);
}

typedef long (*CMP)(void *, void *);

int main() {
    CMP cmp_funcs[] = {cmp_u_i, cmp_u_i};
    KD_tree * tree = KD_tree_init(2, 3, cmp_funcs);
    unsigned int key[] = {10, 1};
    KD_info * info = KD_info_init();
    info->str_key = "haha";
    KD_item * item = KD_item_init(2, KD_key_init(2, key), info);
    KD_tree_add(tree, item);
    KD_tree_print(tree);
    printf("Hello, World!\n");
    return 0;
}
