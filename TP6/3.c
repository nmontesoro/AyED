#include <stdio.h>
#include <stdlib.h>
#include "libs/btree_base.h"

/*

     15
 5        3
6 9     7  10

 */

btn *create_tree();
void tree_sum_helper(btn *node, void *ctx);
int tree_sum(btn *root);

btn *create_tree()
{
    int arr[] = {5, 3, 6, 7, 9, 10};
    btn *root = btn_new(15);

    for (int i = 0; i < 6; i++)
    {
        btn_insert_value(&root, arr[i]);
    }

    return root;
}

void tree_sum_helper(btn *node, void *ctx)
{
    /* En este caso ctx es un puntero a int, declarado en la función
     * tree_sum, que inicialmente tiene valor 0.
     */

    *(int *)ctx += node->value;
}

int tree_sum(btn *root)
{
    int result = 0;

    /* Lo hago con postorder, pero podría hacerlo con preorder o inorder
     * sin problemas.
     */
    btn_postorder(root, tree_sum_helper, (void *)&result);

    return result;
}

int main(int argc, char const *argv[])
{
    int sum = 0;
    const int expected_sum = 15 + 5 + 3 + 6 + 7 + 9 + 10;

    btn *root = create_tree();

    sum = tree_sum(root);

    printf("La suma fue %scorrecta.\n", sum == expected_sum ? "" : "in");

    btn_free(&root);

    return 0;
}
