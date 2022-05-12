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

/**
 * @brief Creates a new node (duh!)
 * @param data A pointer to the data that needs to be stored in the
 * node.
 * @return A pointer to the new node.
 */
btnode *bt_node_new(void *data);

/**
 * @brief Inserts a node in a tree
 * @param node Root of the tree
 * @param new New node
 * @param cmpfunc A function that takes the current node as a parameter
 * and returns 0 if its value is equal to the one sought, -1 if it's
 * less, or 1 if it's greater.
 * @return 0 if something failed, 1 if the insertion was successful, or
 * 2 if the value was already in the tree.
 */
int bt_insert(btnode **node, btnode *new, int (*cmpfunc)(btnode *node));

/**
 * @brief Checks if a given node has no children.
 * @param node The node.
 * @return 0 if the node is NOT empty, 1 if it is.
 */
int bt_node_is_empty(btnode *node);