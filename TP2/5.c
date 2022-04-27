#include <stdio.h>
#include <stdlib.h>
#include "../libs/guia_2.h"
#include "../libs/vector.h"
#include "../libs/myFractions.h"
#include "../libs/myComplex.h"

int compare_fractions(void *a, void *b);
void print_fractions(void *datum);
void test_fractions(void);
int compare_cpx_real(void *a, void *b);
void print_complex_numbers(void *datum);
void test_complex_real(void);
void test_complex_img(void);
int compare_cpx_img(void *a, void *b);

int compare_cpx_real(void *a, void *b)
{
    double real_a = complex_real((complex_t *)a),
           real_b = complex_real((complex_t *)b);
    int result = 0;

    if (real_a > real_b)
    {
        result = 1;
    }
    else if (real_a < real_b)
    {
        result = -1;
    }

    return result;
}

int compare_cpx_img(void *a, void *b)
{
    double img_a = complex_imaginary((complex_t *)a),
           img_b = complex_imaginary((complex_t *)b);
    int result = 0;

    if (img_a > img_b)
    {
        result = 1;
    }
    else if (img_a < img_b)
    {
        result = -1;
    }

    return result;
}

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
    printf("%20s", str);
}

void print_complex_numbers(void *datum)
{
    char str[32] = "";
    complex_t *cpx = (complex_t *)datum;

    sprintf(str, "%3.2f%+3.2fi", complex_real(cpx), complex_imaginary(cpx));
    printf("%20s", str);
}



int main(int argc, char const *argv[])
{
    test_fractions();
    test_complex_real();
    test_complex_img();

    return 0;
}

void test_fractions(void)
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
}

void test_complex_real(void)
{
    vector *v = vector_new(5),
           *v_sorted = (vector *)NULL;

    fill_vector_with_random_cpx_numbers(v, 5);
    v_sorted = vector_sort_insertion(v, sizeof(complex_t), &compare_cpx_real);

    vector_print(v, &print_complex_numbers);
    printf("\n");
    vector_print(v_sorted, &print_complex_numbers);
    printf("\n");

    vector_free(v);
    vector_free(v_sorted);
}

void test_complex_img(void)
{
    vector *v = vector_new(5),
           *v_sorted = (vector *)NULL;

    fill_vector_with_random_cpx_numbers(v, 5);
    v_sorted = vector_sort_insertion(v, sizeof(complex_t), &compare_cpx_img);

    vector_print(v, &print_complex_numbers);
    printf("\n");
    vector_print(v_sorted, &print_complex_numbers);
    printf("\n");

    vector_free(v);
    vector_free(v_sorted);
}