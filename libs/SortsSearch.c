//
// Created by Stephan on 30.04.2022.
//
#include "stdlib.h"

size_t binarySearch(void * cars, void * el, size_t number, size_t size, int (*comparator) (const void*, const void*)) {
    size_t left = 0, right = number;
    while (left != right) {
        size_t m = (left+right)/2;
        void * p = (void*) ((size_t) cars + size*m);
        if (comparator(el, p) > 0) left = m + 1;
        else if (comparator(el, p) < 0) right = m;
        else return m;
    }
    return left;
}