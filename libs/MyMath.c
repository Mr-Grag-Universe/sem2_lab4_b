//
// Created by Stephan on 10.05.2022.
//
#include "stdlib.h"
#include "stdio.h"

long long arithmetic_mean(void * array, size_t number_of_el) {
    if (number_of_el == 0)
        return 0;

    if (sizeof(array) / sizeof(int) != number_of_el) {
        fprintf(stderr, "not int-convertible type in finding of AM.\n");
        return 0;
    }

    long long res = 0;
    for (size_t i = 0; i < number_of_el; ++i) {
        res += (long long) ((int *) array)[number_of_el];
    }

    return res / (long long) number_of_el;
}