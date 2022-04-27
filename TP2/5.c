#include <stdio.h>
#include <stdlib.h>
#include "../libs/guia_2.h"
#include "../libs/vector.h"
#include "../libs/myFractions.h"

int compare_fractions(void *a, void *b)
{
    fraction_t *frac_a = (fraction_t *)a,
               *frac_b = (fraction_t *)b;
    float dec_a = fraction_to_float(frac_a),
          dec_b = fraction_to_float(frac_b);
    int result = 0;

    if (dec_a > dec_b)
    {
        result = 1;
    }
    else if (dec_a < dec_b)
    {
        result = -1;
    }

    return result;
}

void print_fractions(void *datum)
{
    fraction_t *frac = (fraction_t *)datum;
    int num = fraction_get_numerator(frac),
        den = fraction_get_denominator(frac);
    char str[32] = "";

    sprintf(str, "%i/%i", num, den);
    printf("%15s", str);
}

int main(int argc, char const *argv[])
{
    vector *v = vector_new(5),
           *v_sorted = (vector *)NULL;

    fill_vector_with_random_fractions(v, 5);
    v_sorted = vector_sort_insertion(v, sizeof(fraction_t), &compare_fractions);

    vector_print(v, &print_fractions);
    printf("\n");
    vector_print(v_sorted, &print_fractions);
    printf("\n");

    vector_free(v);
    vector_free(v_sorted);

    return 0;
}
