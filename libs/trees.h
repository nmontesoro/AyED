typedef struct _btnode
{
    void *data;
    struct _btnode *left;
    struct _btnode *right;
} btnode;

/**
 * @brief Finds a value stored in a binary tree
 * @param node Root of the tree
 * @param cmpfunc A function that takes the current node as a parameter
 * and returns 0 if its value is equal to the one sought, -1 if it's
 * less, or 1 if it's greater.
 * @return A pointer to the node, or NULL if it could not be found.
 */
btnode *bt_find(btnode *node, int (*cmpfunc)(btnode *node));
