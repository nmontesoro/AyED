#ifndef PRACTICE_GUIA_6
#define PRACTICE_GUIA_6

#include "btree_base.h"

btn *create_tree();
void tree_sum_helper(btn *node, void *ctx);
int tree_sum(btn *root);

#endif