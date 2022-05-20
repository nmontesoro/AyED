#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "btree_base.h"

btn *btn_new(t_elem_btree value)
{
    btn *node = malloc(sizeof(btn));

    if (node)
    {
        node->value = value;
        node->left = NULL;
        node->right = NULL;
    }

    return node;
}

void btn_free(btn **node)
{
    if (node)
    {
        if (*node)
        {
            btn_free(&((*node)->left));
            btn_free(&((*node)->right));
            free(*node);
            *node = NULL;
        }
    }
}

void btn_free_helper(btn *node, void *ctx)
{
}

btn **btn_find(btn **node, t_elem_btree value, int cmp(t_elem_btree, t_elem_btree))
{
    btn **result = NULL;

    if (node && *node)
    {
        if (!cmp((*node)->value, value))
        {
            result = node;
        }
        else
        {
            result = btn_find(&((*node)->left), value, cmp);
            if (!result)
            {
                result = btn_find(&((*node)->right), value, cmp);
            }
        }
    }

    return result;
}

int btn_isLeaf(btn *node)
{
    int result = 0;

    if (node)
    {
        result = !(node->left || node->right);
    }

    return result;
}

int btn_count(btn *node)
{
    int result = 0;
    if (node != NULL)
    {
        result = 1 + btn_count(node->left) + btn_count(node->right);
    }
    return result;
}

int btn_insert(btn **node, btn *newNode)
{
    int result = 0,
        lcnt = 0,
        rcnt = 0;

    if (node)
    {
        if (*node)
        {
            if (!((*node)->left))
            {
                (*node)->left = newNode;
            }
            else if (!((*node)->right))
            {
                (*node)->right = newNode;
            }
            else
            {
                lcnt = btn_count((*node)->left);
                rcnt = btn_count((*node)->right);

                if (lcnt <= rcnt)
                {
                    btn_insert(&((*node)->left), newNode);
                }
                else if (rcnt < lcnt)
                {
                    btn_insert(&((*node)->right), newNode);
                }
            }
        }
        else
        {
            *node = newNode;
        }
    }

    return result;
}

int btn_insert_value(btn **node, t_elem_btree value)
{
    int result = 0;
    btn *new_node = NULL;

    if (node)
    {
        new_node = btn_new(value);

        result = btn_insert(node, new_node);
    }

    return result;
}

int _btn_level(btn *node, t_elem_btree value, int level, int cmp(t_elem_btree, t_elem_btree))
{
    int result = 0;

    if (node)
    {
        if (!cmp(node->value, value))
        {
            result = level;
        }
        else
        {
            result = _btn_level(node->left, value, level + 1, cmp);

            if (!result)
            {
                result = _btn_level(node->right, value, level + 1, cmp);
            }
        }
    }

    return result;
}

int btn_level(btn *root, t_elem_btree value, int cmp(t_elem_btree, t_elem_btree))
{
    return _btn_level(root, value, 1, cmp);
}

int _max(int a, int b) { return (a > b) ? a : b; }

int _btn_height(btn *node, int height)
{
    int result = height;

    if (node)
    {
        if (!btn_isLeaf(node))
        {
            result = _max(_btn_height(node->left, height + 1),
                          _btn_height(node->right, height + 1));
        }
    }
    else
    {
        result = -1;
    }

    return result;
}

int btn_height(btn *node)
{
    return _btn_height(node, 0);
}

void btn_inorder(btn *node, void btn_do(btn *, void *), void *ctx)
{
    if (!node)
        return;

    btn_inorder(node->left, btn_do, ctx);
    btn_do(node, ctx);
    btn_inorder(node->right, btn_do, ctx);
}

void btn_postorder(btn *node, void btn_do(btn *, void *), void *ctx)
{
    if (!node)
        return;

    btn_postorder(node->left, btn_do, ctx);
    btn_postorder(node->right, btn_do, ctx);
    btn_do(node, ctx);
}

void btn_preorder(btn *node, void btn_do(btn *, void *), void *ctx)
{
    if (!node)
        return;

    btn_do(node, ctx);
    btn_preorder(node->left, btn_do, ctx);
    btn_preorder(node->right, btn_do, ctx);
}

int _btn_print(btn *tree, int is_left, int offset, int depth, char s[20][255], void toStr(btn *, char *))
{
    char b[200];
    int width = 5;

    if (!tree)
        return 0;
    toStr(tree, b);
    // sprintf(b, "%s", str);

    int left = _btn_print(tree->left, 1, offset, depth + 1, s, toStr);
    int right = _btn_print(tree->right, 0, offset + left + width, depth + 1, s, toStr);

    // for (int i = 0; i < width; i++) s[2 * depth][offset + left + i] = b[i];
    for (int i = 0; i < strlen(b); i++)
        s[2 * depth][offset + left + i] = b[i];

    if (depth && is_left)
    {
        for (int i = 0; i < width + right; i++)
            s[2 * depth - 1][offset + left + width / 2 + i] = '-';

        s[2 * depth - 1][offset + left + width / 2] = '+';
        s[2 * depth - 1][offset + left + width + right + width / 2] = '+';
    }
    else if (depth && !is_left)
    {
        for (int i = 0; i < left + width; i++)
            s[2 * depth - 1][offset - width / 2 + i] = '-';

        s[2 * depth - 1][offset + left + width / 2] = '+';
        s[2 * depth - 1][offset - width / 2 - 1] = '+';
    }

    return left + width + right;
}

void btn_print(btn *tree, void toStr(btn *, char *))
{
    char s[20][255];
    for (int i = 0; i < 20; i++)
        sprintf(s[i], "%80s", " ");

    _btn_print(tree, 0, 0, 0, s, toStr);

    for (int i = 0; i < 20; i++)
    {
        int j = 0;
        while (s[i][j] != '\0' && s[i][j] == ' ')
        {
            j++;
        }
        if (s[i][j] != '\0')
        {
            printf("%s\n", s[i]);
        }
    }
}
