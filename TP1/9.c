#include <stdio.h>
#include <stdlib.h>
#include "../libs/myComplex.h"

int main(int argc, char const *argv[])
{
    complex_t *a = complex_init(3, 4),
              *b = complex_init(1, 2),
              *c = NULL;
    char *a_str = complex_to_string(a),
         *b_str = complex_to_string(b),
         *c_str = NULL;

    fprintf(stdout, "Re(%s) = %5.4f\n", a_str, complex_real(a));
    fprintf(stdout, "Im(%s) = %5.4f\n", b_str, complex_imaginary(b));

    c = complex_sum(a, b);
    c_str = complex_to_string(c);
    fprintf(stdout, "(%s) + (%s) = %s\n", a_str, b_str, c_str);
    free((void *) c);
    free((void *) c_str);

    c = complex_subtract(a, b);
    c_str = complex_to_string(c);
    fprintf(stdout, "(%s) - (%s) = %s\n", a_str, b_str, c_str);
    free((void *) c);
    free((void *) c_str);

    c = complex_multiply(a, b);
    c_str = complex_to_string(c);
    fprintf(stdout, "(%s) * (%s) = %s\n", a_str, b_str, c_str);
    free((void *) c);
    free((void *) c_str);

    c = complex_divide(a, b);
    c_str = complex_to_string(c);
    fprintf(stdout, "(%s) / (%s) = %s\n", a_str, b_str, c_str);
    free((void *) c);
    free((void *) c_str);

    fprintf(stdout, "abs(%s) = %5.4f\n", a_str, complex_abs(a));

    fprintf(stdout, "theta(%s) = %5.4f\n", b_str, complex_angle(b));
    
    free((void *) a);
    free((void *) b);
    free((void *) a_str);
    free((void *) b_str);

    return 0;
}
