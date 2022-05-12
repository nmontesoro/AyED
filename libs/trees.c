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

int bt_insert(btnode **node, btnode *new, int (*cmpfunc)(btnode *node))
{
    int result = 0;

    if (node && new)
    {
        if (!*node)
        {
            *node = new;
        }
        else
        {
            switch ((*cmpfunc)(*node))
            {
            case 0:
                /* Both are equal */
                result = 2;
                break;

            case 1:
                /* Current node is greater than the one sought */
                result = bt_insert(&((*node)->right), new, cmpfunc);
                break;

            case -1:
                /* Current node is less than the one sought */
                result = bt_insert(&((*node)->left), new, cmpfunc);
                break;

            default:
                /* Comparison function returned something unexpected.
                 * Do not assign anything to result, simply return 0.
                 */
                break;
            }
        }
    }

    return result;
}

int bt_node_is_empty(btnode *node)
{
    return !(node->left || node->right);
}

btnode *bt_delete_node(btnode *node, int (*cmpfunc)(btnode *node))
{
    btnode *aux = node;
    bt_insert(&(aux->right), aux->right, cmpfunc);
    node = aux->right;
    aux->right = NULL;
    aux->left = NULL;
    return aux;
}

void bt_traverse(btnode *root, void (*callback)(btnode *node), int order)
{
    if (root)
    {
        switch (order)
        {
        case TRAVERSE_PREORDER:
            (*callback)(root);
            bt_traverse(root->left, callback, order);
            bt_traverse(root->right, callback, order);
            break;

        case TRAVERSE_INORDER:
            bt_traverse(root->left, callback, order);
            (*callback)(root);
            bt_traverse(root->right, callback, order);
            break;

        case TRAVERSE_POSTORDER:
            bt_traverse(root->left, callback, order);
            bt_traverse(root->right, callback, order);
            (*callback)(root);
            break;

        default:
            break;
        }
    }
}