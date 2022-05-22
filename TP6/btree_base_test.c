#include "libs/btree_base.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***/
// toStr devuelve el contenido de un nodo como string
// asume que t_elem_tree es un número entero
void btn_intToStr(btn *node, char *str);

// compara 2 elementos del árbol cuando son enteros
int btn_cmp_int(t_elem_btree a, t_elem_btree b);

// Imprime el nivel de un nodo del árbol.
// Asumiendo que los valores de cada nodo son únicos.
void btn_level_test(btn *node, void *ctx);

// Imprime los niveles de todos los nodos recorriendo inorder.
void btn_level_each(btn *root);

// Prueba todos los nodos si son hojas e imprime el resultado.
void btn_test_leaf(btn *node, void *ctx);

// toStr devuelve el contenido de un nodo como string
// Y le agrega la altura del nodo.
void btn_intToStr_h(btn *node, char *str);

// Imprime un nodo
void btn_do_print(btn *node);

// Imprime un nodo con contexto (para utilizar en recorridos)
void btn_do_print_ctx(btn *node, void *ctx);

void btn_intToStr(btn *node, char *str)
{
    if (!node)
        return;
    sprintf(str, "(%03d)", node->value);
}

int btn_cmp_int(t_elem_btree a, t_elem_btree b)
{
    return a - b;
}

void btn_level_test(btn *node, void *ctx)
{
    btn *root = (btn *)ctx;
    int level = btn_level(root, node->value, btn_cmp_int);
    char str[100];
    btn_intToStr(node, str);
    printf("%10s: level %d\n", str, level);
}

void btn_level_each(btn *root)
{
    btn_inorder(root, btn_level_test, root);
}

void btn_test_leaf(btn *node, void *ctx)
{
    char str[100];
    btn_intToStr(node, str);
    if (btn_isLeaf(node))
    {
        printf("%s es hoja\n", str);
    }
    else
    {
        printf("%s no es hoja\n", str);
    }
}

void btn_intToStr_h(btn *node, char *str)
{
    if (!node)
        return;
    sprintf(str, "(%03d) h=%d", node->value, btn_height(node));
}

void btn_do_print(btn *node)
{
    if (!node)
        return;
    char str[100];
    btn_intToStr(node, str);
    printf("%s ", str);
}

void btn_do_print_ctx(btn *node, void *ctx)
{
    if (!node)
        return;
    char str[100];
    btn_intToStr(node, str);
    printf("%s ", str);
}

int main(int argc, char const *argv[])
{
    btn *root = NULL;
    btn_insert_value(&root, 5);
    btn_insert_value(&root, 6);
    btn_insert_value(&root, 8);
    btn_insert_value(&root, 10);
    btn_insert_value(&root, 11);
    btn_insert_value(&root, 123);
    btn_insert_value(&root, 124);
    btn_insert_value(&root, 225);
    btn_insert_value(&root, 20);

    printf("\n%10s: \n", "Árbol");
    btn_print(root, btn_intToStr);

    printf("\nEl árbol cuenta con %d nodos.\n", btn_count(root));

    printf("\n%10s: ", "Inorder");
    btn_inorder(root, btn_do_print_ctx, NULL);
    printf("\n%10s: ", "Preorder");
    btn_preorder(root, btn_do_print_ctx, NULL);
    printf("\n%10s: ", "Postorder");
    btn_postorder(root, btn_do_print_ctx, NULL);
    printf("\n");

    printf("\n%10s: \n", "Niveles de cada nodo");
    btn_level_each(root);

    printf("\n%10s: \n", "Prueba de Hojas");
    btn_preorder(root, btn_test_leaf, NULL);

    printf("\n%10s: \n", "Arbol con alturas");
    btn_print(root, btn_intToStr_h);

    btn **twenty = btn_find(&root, 20, btn_cmp_int);
    btn_free(twenty);
    printf("\n%10s: \n", "Eliminado nodo 20");
    btn_print(root, btn_intToStr);

    btn **six = btn_find(&root, 6, btn_cmp_int);
    btn_free(six);
    printf("\n%10s: \n", "Eliminado nodo 6");
    btn_print(root, btn_intToStr);

    btn_free(&root);

    return 0;
}
