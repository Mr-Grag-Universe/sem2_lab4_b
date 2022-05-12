//
// Created by Stephan on 10.05.2022.
//

#ifndef LAB4B_PROT_MYMATH_H
#define LAB4B_PROT_MYMATH_H
typedef struct Coordinates {
    void * coords;
    size_t number_of_coords;
} Coordinates;
typedef enum Type {
   INT,
   U_INT,
   LONG,
   U_LONG,
   CHAR,
   U_CHAR,
} Type;

long long arithmetic_mean(void * array, size_t number_of_el);
/// in struct: ptr to coords and number of coords - size_t
double distance(void * c1, void * c2, size_t d, Type type);
#endif //LAB4B_PROT_MYMATH_H
