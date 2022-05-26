#include "practice.h"

btn *create_tree()
{
    int arr[] = {15, 17, 6, 7, 9, 10};
    btn *root = btn_new(64);

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

void check_tree_helper(btn *node, void *ctx)
{
    if (!btn_isLeaf(node))
    {
        int sum = tree_sum(node);
        *(int *)ctx = *(int *)ctx && (sum == node->value << 1);
    }
}

int check_tree(btn *root)
{
    int result = 1;

    btn_postorder(root, check_tree_helper, (void *)&result);

    return result;
}