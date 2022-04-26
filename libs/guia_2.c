#include <time.h>
#include <stdlib.h>
#include "guia_2.h"
#include "vector.h"
#include "myFractions.h"
#include "myComplex.h"

vector *fill_vector_with_random_fractions(vector *v, int n)
{
    vector *result = NULL;
    int num = 0,
        den = 0;
    fraction_t *frac_original = NULL,
               *frac_simplified = NULL;

    srand(time(NULL));

    if (v && n > 0 && n <= v->max_size)
    {
        for (int i = 0; i < n; i++)
        {
            num = rand();
            den = rand();
            frac_original = fraction_init(num, den);
            frac_simplified = fraction_simplify(frac_original);

            free(frac_original);
            vector_add(v, (void *)frac_simplified);
        }
    }

    return result;
}

vector *fill_vector_with_random_cpx_numbers(vector *v, int n)
{
    vector *result = NULL;
    int real = 0,
        img = 0;
    complex_t *cpx = NULL;

    srand(time(NULL));

    if (v && n > 0 && n <= v->max_size)
    {
        for (int i = 0; i < n; i++)
        {
            real = rand();
            img = rand();
            cpx = complex_init((double)real, (double)img);

            vector_add(v, (void *)cpx);
        }
    }

    return result;
}
