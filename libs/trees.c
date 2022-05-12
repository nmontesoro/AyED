#include <stdlib.h>
#include "trees.h"

btnode *bt_find(btnode *node, int (*cmpfunc)(btnode *node))
{
    btnode *result = NULL;

    if (node)
    {
        switch ((*cmpfunc)(node))
        {
        case 0:
            /* Both are equal */
            result = node;
            break;

        case 1:
            /* Current node is greater than the one sought */
            result = bt_find(node->left, cmpfunc);
            break;

        case -1:
            /* Current node is less than the one sought */
            result = bt_find(node->right, cmpfunc);
            break;

        default:
            /* Comparison function returned something unexpected.
             * Do not assign anything to result, simply return NULL.
             */
            break;
        }
    }

    return result;
}

btnode *bt_node_new(void *data)
{
    btnode *new_node = (btnode *)malloc(sizeof(btnode));

    if (new_node)
    {
        new_node->data = data;
        new_node->left = NULL;
        new_node->right = NULL;
    }

    return new_node;
}
