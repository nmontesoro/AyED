#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "myComplex.h"

complex_t *complex_init(double real, double imaginary)
{
    complex_t *cp = malloc(sizeof(complex_t));
    cp->real = real;
    cp->im = imaginary;
    return cp;
}

double complex_real(complex_t *num)
{
    return num->real;
}

double complex_imaginary(complex_t *num)
{
    return num->im;
}

complex_t *complex_sum(complex_t *a, complex_t *b)
{
    return complex_init(a->real + b->real, a->im + b->im);
}

complex_t *complex_subtract(complex_t *a, complex_t *b)
{
    return complex_init(a->real - b->real, a->im - b->im);
}

complex_t *complex_multiply(complex_t *a, complex_t *b)
{
    double real = 0,
          im = 0;
    
    real = (a->real * b->real - a->im * b->im);
    im = (a->real * b->im + a->im * b->real);

    return complex_init(real, im);
}

complex_t *complex_divide(complex_t *a, complex_t *b)
{
    double quotient = complex_abs(a) / complex_abs(b),
           phase_dif = complex_angle(a) - complex_angle(b);
    
    return complex_init(quotient * cos(phase_dif), quotient * sin(phase_dif));
}

int complex_compare(complex_t *a, complex_t *b)
{
    return (a->real == b->real && a->im == b->im);
}

double complex_abs(complex_t *a)
{
    return sqrt(pow(a->real, 2) + pow(a->im, 2));
}

double complex_angle(complex_t *a)
{
    return atan(a->im / a->real);
}

char *complex_to_string(complex_t *a)
{
    char *str = malloc(sizeof(char) * 255);
    sprintf(str, "%5.4f %c %5.4fi", a->real, a->im < 0 ? '-' : '+', fabs(a->im));
    
    return str;
}
