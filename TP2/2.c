#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../libs/myFractions.h"
#include "../libs/myComplex.h"
#include "../libs/vector.h"

void print_fraction(int num, int den);
void print_complex(double real, double imaginary);
void print_vector(void *datum);

int main(int argc, char const *argv[])
{
    fraction_t *frac = fraction_init(-3, 4);
    fraction_print(frac, &print_fraction);
    free(frac);

    complex_t *cpx = complex_init(3.14159225, -11.3);
    complex_print(cpx, &print_complex);
    free(cpx);

    vector *v = vector_new(100);

    for (int i = -20; i < 20; i++)
    {
        double *val = (double *)malloc(sizeof(double));
        *val = 3*pow(i, 3) - 1.5*pow(i, 2) + i - 3;
        vector_add(v, (void *)val);
    }

    vector_print(v, &print_vector);

    vector_free(v);

    return 0;
}

void print_fraction(int num, int den)
{
    printf("%i/%i\n", num, den);
}

void print_complex(double real, double imaginary)
{
    printf("%5.4f %c %5.4fi\n", real, imaginary >= 0 ? '+' : '-', fabs(imaginary));
}

void print_vector(void *datum)
{
    printf(" %5.4f ", *(double *) datum);
}