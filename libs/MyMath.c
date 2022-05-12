//
// Created by Stephan on 10.05.2022.
//
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "MyMath.h"

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

double distance(void * c1, void * c2, size_t d, Type type) {
    if (c1 == NULL || c2 == NULL) {
        return -1;
    }

    double distance = 0;
    switch (type) {
        case INT: {
            for (size_t i = 0; i < d; ++i)
                distance += pow(((int *) c1)[i] - ((int *) c2)[i], 2);
            return sqrt(distance);
        }
        case U_INT: {
            for (size_t i = 0; i < d; ++i)
                distance += pow((double) (((unsigned int *) c1)[i]) - (double) (((unsigned int *) c2)[i]), 2);
            return sqrt(distance);
        }
        case LONG: {
            for (size_t i = 0; i < d; ++i)
                distance += pow(((long int *) c1)[i] - ((long int *) c2)[i], 2);
            return sqrt(distance);
        }
        case U_LONG: {
            for (size_t i = 0; i < d; ++i)
                distance += pow(((unsigned long int *) c1)[i] - ((unsigned long int *) c2)[i], 2);
            return sqrt(distance);
        }
        default:
            return -1;
    }
}