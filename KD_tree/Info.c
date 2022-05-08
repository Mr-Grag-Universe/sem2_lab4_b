//
// Created by Stephan on 08.05.2022.
//
#include "stdlib.h"
#include "stdio.h"
#include "Errors.h"
#include "KD_tree.h"

KD_info * KD_info_init(char * str_info) {
    KD_info * info = malloc(sizeof(KD_info));
    info->str_key = str_info;

    return info;
}

Error KD_info_free(KD_info * info) {
    if (info == NULL) {
        fprintf(stderr, "NULL-ptr info freeing.\n");
        return FREEING_OF_NULL_PTR;
    }

    if (info->str_key == NULL) {
        fprintf(stderr, "NULL-prt str-info in info freeing.\n");
        free(info);
        return FREEING_OF_NULL_PTR;
    }

    free(info->str_key);
    free(info);

    return IT_IS_OK;
}