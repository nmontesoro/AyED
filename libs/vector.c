#include <stdlib.h>
#include <stdio.h>
#include "vector.h"

static void **vector_get_copy_from_index(vector *v, int index);

vector *vector_new(int ini_size)
{
    vector *v = NULL;

    if (ini_size > 0)
    {
        v = (vector *)malloc(sizeof(vector));
        v->a = (void **)malloc(sizeof(void *) * ini_size);
        v->size = 0;
        v->max_size = ini_size;
    }

    return v;
}

void vector_free(vector *v)
{
    if (v)
    {
        /* Free up memory taken by each item in v->a */
        for (int i = 0; i < v->size; i++)
        {
            free(v->a[i]);
        }

        /* Free up memory taken by v->a */
        free(v->a);

        /* Finally, free up memory taken by vector itself */
        free(v);
    }
}

int vector_size(vector *v)
{
    return v ? v->size : -1;
}

int vector_isfull(vector *v)
{
    return v ? (v->size == v->max_size) : -1;
}

int vector_isempty(vector *v)
{
    return v ? (!v->size) : -1;
}

void *vector_get(vector *v, int index)
{
    void *ptr = NULL;

    if (v && index >= 0 && index < v->size)
    {
        ptr = v->a[index];
    }

    return ptr;
}

int vector_add(vector *v, void *value)
{
    int success = 0;

    if (v && value)
    {
        if (v->size < v->max_size)
        {
            v->a[v->size++] = value;
        }
    }

    return success;
}

void *vector_set(vector *v, int index, void *value)
{
    void *result = NULL;

    if (v && index >= 0 && index < v->size && value)
    {
        v->a[index] = value;
        result = v->a[index];
    }

    return result;
}

int vector_insert(vector *v, int index, void *value)
{
    int success = 0;

    if (v && index >= 0 && index < v->size - 1 && value)
    {
        int copy_size = v->max_size - index;
        void **copy = vector_get_copy_from_index(v, index);

        v->a[index] = value;

        for (int i = 0; i < copy_size; i++)
        {
            v->a[index + i + 1] = copy[i];
        }

        free(copy);

        success = 1;
        v->size++;
    }

    return success;
}

void *vector_remove(vector *v, int index)
{
    void *removed_element_ptr = NULL;

    if (v && index >= 0 && index < v->size)
    {
        removed_element_ptr = v->a[index];

        if (index < v->size - 1)
        {
            int copy_size = v->max_size - index - 1;
            void **copy = vector_get_copy_from_index(v, index + 1);
            
            for (int i = 0; i < copy_size; i++)
            {
                v->a[index + i] = copy[i];
            }

            free(copy);
        }

        v->a[v->size--] = NULL;
    }

    return removed_element_ptr;
}

static void **vector_get_copy_from_index(vector *v, int index)
{
    int copy_size = v->max_size - index;
    void **copy = (void **)malloc(sizeof(void *) * copy_size);

    for (int i = index; i < v->max_size; i++)
    {
        copy[i - index] = v->a[i];
    }

    return copy;
}