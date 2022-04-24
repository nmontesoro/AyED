#include <stdio.h>
#include <stdlib.h>
#include "../libs/myFractions.h"

int main(int argc, char const *argv[])
{
    fraction_t *a = fraction_init(3, 4),
               *b = fraction_init(1, 8);
    
    fraction_t *res = fraction_divide(a, b);
    char *str = fraction_to_string(res);

    fprintf(stdout, "%s\n", str);

    free((void *) a);
    free((void *) b);
    free((void *) res);
    free((void *) str);

    return 0;
}
