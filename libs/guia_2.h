#ifndef _GUIA_2
#define _GUIA_2
#include "vector.h"

/**
 * @brief Fills a given vector with random rational numbers
 * @param v Vector
 * @param n How many numbers to generate
 * @return Pointer to vector, or NULL if unsuccessful
 */
vector *fill_vector_with_random_fractions(vector *v, int n);

/**
 * @brief Fills a given vector with random complex numbers
 * @param v Vector
 * @param n How many numbers to generate
 * @return Pointer to vector, or NULL if unsuccessful
 */
vector *fill_vector_with_random_cpx_numbers(vector *v, int n);

#endif