#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>
#include "myMath.h"
#include "myFractions.h"

fraction_t *fraction_init(int numerator, int denominator)
{
    fraction_t *frac = NULL;

    if (!denominator)
    {
        fprintf(stderr, "ERROR: Denominator of a fraction cannot be zero!");
    }
    else
    {
        frac = malloc(sizeof(fraction_t));

        frac->numerator = numerator;
        frac->denominator = denominator;
    }

    return frac;
}

int fraction_get_denominator(fraction_t *frac)
{
    return frac->denominator;
}

int fraction_get_numerator(fraction_t *frac)
{
    return frac->numerator;
}

fraction_t *fraction_simplify(fraction_t *frac)
{
    int gcd = myGCD(frac->numerator, frac->denominator);
    return fraction_init((frac->numerator) / gcd, (frac->denominator) / gcd);
}

fraction_t *fraction_add(fraction_t *frac1, fraction_t *frac2)
{
    int result_num = 0,
        result_den = 0,
        gcd = 0;

    result_num = frac1->numerator * frac2->denominator + frac1->denominator * frac2->numerator;

    result_den = frac1->denominator * frac2->denominator;

    gcd = myGCD(result_num, result_den);

    return fraction_init(result_num / gcd, result_den / gcd);
}

fraction_t *fraction_subtract(fraction_t *frac1, fraction_t *frac2)
{
    int result_num = 0,
        result_den = 0,
        gcd = 0;

    result_num = frac1->numerator * frac2->denominator - frac1->denominator * frac2->numerator;

    result_den = frac1->denominator * frac2->denominator;

    gcd = myGCD(result_num, result_den);

    return fraction_init(result_num / gcd, result_den / gcd);
}

fraction_t *fraction_multiply(fraction_t *frac1, fraction_t *frac2)
{
    int result_num = frac1->numerator * frac2->numerator,
        result_den = frac1->denominator * frac2->denominator,
        gcd = 0;

    gcd = myGCD(result_num, result_den);

    return fraction_init(result_num / gcd, result_den / gcd);
}

fraction_t *fraction_divide(fraction_t *frac1, fraction_t *frac2)
{
    int result_num = frac1->numerator * frac2->denominator,
        result_den = frac1->denominator * frac2->numerator,
        gcd = 0;

    gcd = myGCD(result_num, result_den);

    return fraction_init(result_num / gcd, result_den / gcd);
}

char *fraction_to_string(fraction_t *frac)
{
    char *str = (char *)malloc(sizeof(char) * 255);
    sprintf(str,
            "%c%i/%i",
            (frac->numerator * frac->denominator < 0) ? '-' : ' ',
            abs(frac->numerator),
            abs(frac->denominator));
    return str;
}

float fraction_to_float(fraction_t *frac)
{
    return (frac->numerator / frac->denominator);
}

int fraction_compare(fraction_t *frac1, fraction_t *frac2)
{
    fraction_t *a = NULL,
               *b = NULL;
    int result = 0;

    a = fraction_simplify(frac1);
    b = fraction_simplify(frac2);

    result = (a->numerator == b->numerator && a->denominator == b->denominator);

    free((void *)a);
    free((void *)b);

    return result;
}

void fraction_print(fraction_t *frac, void (*printfunct)(int, int))
{
    (*printfunct)(fraction_get_numerator(frac), fraction_get_denominator(frac));
}
