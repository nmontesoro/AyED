#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "btree_search.h"

t_elem_btree _max_elem(t_elem_btree a, t_elem_btree b,
                       int cmp(t_elem_btree, t_elem_btree))
{
    return (cmp(a, b) > 0) ? a : b;
}

int sbt_insert_node(btn **node, btn *newNode,
                    int cmp(t_elem_btree, t_elem_btree))
{
    if (node == NULL)
        return 0;
    if (newNode == NULL)
        return 1;

    int result = 0;

    if (!*node)
    {
        *node = newNode;
        result = 1;
    }
    else
    {
        int comparison = cmp((*node)->value, newNode->value);

        if (comparison > 0)
        {
            result = sbt_insert_node(&((*node)->left), newNode, cmp);
        }
        else if (comparison < 0)
        {
            result = sbt_insert_node(&((*node)->right), newNode, cmp);
        }
    }

    return result;
}

int sbt_insert_value(btn **node, t_elem_btree value,
                     int cmp(t_elem_btree, t_elem_btree))
{
    int result;

    btn *new_node = btn_new(value);

    result = sbt_insert_node(node, new_node, cmp);

    return result;
}

btn **sbt_get_min_node(btn **node)
{
    btn **result = NULL;

    if (node && *node)
    {
        if ((*node)->left)
        {
            result = sbt_get_min_node(&((*node)->left));
        }
        else
        {
            result = node;
        }
    }

    return result;
}

t_elem_btree sbt_get_min_value(btn *node)
{
    if (node == NULL)
        return -1;
    btn **r = sbt_get_min_node(&node);
    return (*r)->value;
}

btn **sbt_get_max_node(btn **node)
{
    btn **result = NULL;

    if (node && *node)
    {
        if ((*node)->right)
        {
            result = sbt_get_max_node(&((*node)->right));
        }
        else
        {
            result = node;
        }
    }

    return result;
}

t_elem_btree sbt_get_max_value(btn *node)
{
    if (node == NULL)
        return -1;
    btn **r = sbt_get_max_node(&node);
    return (*r)->value;
}

btn *sbt_findr(btn *node, t_elem_btree value,
               int cmp(t_elem_btree, t_elem_btree))
{
    return (btn *)NULL;
}

btn *sbt_findi(btn *node, t_elem_btree value,
               int cmp(t_elem_btree, t_elem_btree))
{
    return (btn *)NULL;
}

btn **sbt_find_node(btn **node, t_elem_btree value,
                    int cmp(t_elem_btree, t_elem_btree))
{
    btn **result = NULL;

    // TODO: Implement a proper binary search
    result = btn_find(node, value, cmp);

    return result;
}

btn **sbt_findii(btn **node, t_elem_btree value,
                 int cmp(t_elem_btree, t_elem_btree))
{
    return (btn **)NULL;
}

int sbt_in(btn *node, t_elem_btree value, int cmp(t_elem_btree, t_elem_btree))
{
    return 0;
}

btn *sbt_remove_node(btn **node, int cmp(t_elem_btree, t_elem_btree))
{
    btn *aux = NULL;
    int result = 0;

    if (node && *node)
    {
        aux = *node;
        result = sbt_insert_node(&((*node)->right),
                                 (*node)->left, cmp);

        if (result)
        {
            *node = (*node)->right;

            aux->left = NULL;
            aux->right = NULL;
        }
        else
        {
            aux = NULL;
        }
    }

    return aux;
}

btn *sbt_remove_node2(btn **node)
{
    btn *result = NULL;
    btn **found = NULL;

    if (node && *node)
    {
        result = *node;

        if (!(found = sbt_get_max_node(node)))
        {
            found = sbt_get_min_node(node);
        }

        if (found)
        {
            (*found)->left = (*node)->left;
            (*found)->right = (*node)->right;
            *node = *found;
            *found = NULL;
        }

        result->left = NULL;
        result->right = NULL;
    }

    return result;
}

btn *sbt_remove_node_smart(btn **node)
{
    btn *removed_node = NULL;
    btn **found = NULL;

    if (node && *node)
    {
        removed_node = *node;

        if (btn_height((*node)->left) >= btn_height((*node)->right))
        {
            found = sbt_get_max_node(&((*node)->left));
        }
        else
        {
            found = sbt_get_min_node(&((*node)->right));
        }

        if (found)
        {
            (*found)->left = (*node)->left;
            (*found)->right = (*node)->right;
            *node = *found;
            *found = NULL;
        }

        removed_node->left = NULL;
        removed_node->right = NULL;
    }

    return removed_node;
}

int sbt_remove_value(btn **node, t_elem_btree value,
                     int cmp(t_elem_btree, t_elem_btree))
{
    int result = 0;

    btn **node_found = sbt_find_node(node, value, cmp);

    btn *test = sbt_remove_node(node_found, cmp);

    if (test)
    {
        free(test);
        result = 1;
    }
    else
    {
        printf("Something went wrong!\n");
    }

    return result;
}

int sbt_remove_value2(btn **node, t_elem_btree value,
                      int cmp(t_elem_btree, t_elem_btree))
{
    int result = 0;

    btn **node_found = sbt_find_node(node, value, cmp);
    btn *test = sbt_remove_node2(node_found);

    if (test)
    {
        free(test);
        result = 1;
    }
    else
    {
        printf("Something went wrong!\n");
    }

    return result;
}

int sbt_remove_value_smart(btn **node, int value,
                           int cmp(t_elem_btree, t_elem_btree))
{
    int result = 0;

    btn **node_found = sbt_find_node(node, value, cmp);
    btn *node_removed = sbt_remove_node_smart(node_found);

    if (node_removed)
    {
        free(node_removed);
        result = 1;
    }

    return result;
}