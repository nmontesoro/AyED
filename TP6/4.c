#include <stdio.h>
#include <stdlib.h>
#include "libs/practice.h"

int main(int argc, char const *argv[])
{
    btn *root = create_tree();

    int is_sum = check_tree(root);

    printf("Cada nodo %stiene como valor la suma de sus descendientes.\n",
            is_sum ? "" : "NO ");

    btn_free(&root);
    return 0;
}
