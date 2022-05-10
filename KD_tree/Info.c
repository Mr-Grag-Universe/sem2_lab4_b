//
// Created by Stephan on 08.05.2022.
//
#include "stdlib.h"
#include "stdio.h"
#include "Errors.h"
#include "KD_tree.h"
#include "KGetLine.h"

KD_info * KD_info_init(char * str_info) {
    KD_info * info = malloc(sizeof(KD_info));
    info->str_info = str_info;

    return info;
}

Error KD_info_free(KD_info * info) {
    if (info == NULL) {
        fprintf(stderr, "NULL-ptr info freeing.\n");
        return FREEING_OF_NULL_PTR;
    }

    if (info->str_info == NULL) {
        fprintf(stderr, "NULL-prt str-info in info freeing.\n");
        free(info);
        return FREEING_OF_NULL_PTR;
    }

    free(info->str_info);
    free(info);

    return IT_IS_OK;
}

KD_info * KD_info_enter() {
    KD_info * info = KD_info_init(get_line());

    return info;
}

Error KD_info_print(KD_info * info) {
    if (info == NULL) {
        fprintf(stderr, "NULL info in printing.\n");
        return NULL_PTR_IN_UNEXCITED_PLACE;
    }

    printf("%s\n", info->str_info);

    return IT_IS_OK;
}