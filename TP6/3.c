#include <stdio.h>
#include <stdlib.h>
#include "libs/practice.h"

/*

     15
 5        3
6 9     7  10

 */

int main(int argc, char const *argv[])
{
    int sum = 0;
    const int expected_sum = 64 + 15 + 17 + 6 + 7 + 9 + 10;

    btn *root = create_tree();

    sum = tree_sum(root);

    printf("La suma fue %scorrecta.\n", sum == expected_sum ? "" : "in");

    btn_free(&root);

    return 0;
}
