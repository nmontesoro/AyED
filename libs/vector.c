#include <stdlib.h>
#include <stdio.h>
#include <math.h>   // for floor
#include <string.h> // for memcpy
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

int vector_size(const vector *v)
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

void *vector_get(const vector *v, int index)
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

    if (v && index >= 0 && index < v->size && value)
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

void vector_print(vector *v, void (*func)(void *))
{
    for (int i = 0; i < v->size; i++)
    {
        (*func)(v->a[i]);
    }
}

int vector_max_size(const vector *v)
{
    int max_size = 0;

    if (v)
    {
        max_size = v->max_size;
    }

    return max_size;
}

vector *vector_copy(const vector *v, int data_size, int li, int ui)
{
    int max_size = 0;
    vector *copy = NULL;

    if (ui == -1)
    {
        ui = vector_size(v) - 1;
    }

    if (v && data_size > 0 && li >= 0 && (ui > li && ui < vector_size(v)))
    {

        max_size = vector_max_size(v);
        copy = vector_new(max_size);

        for (int i = li; i <= ui; i++)
        {
            /* Create a copy of the value in memory */
            void *val = malloc(data_size);
            memcpy(val, vector_get(v, i), data_size);

            /* Add said copy to the new vector */
            vector_add(copy, val);
        }
    }

    return copy;
}

vector *vector_sort_insertion(const vector *v, int data_size, int (*cmp_func)(void *, void *))
{
    vector *copy = NULL;
    void **data = NULL;
    int size = 0,
        i = 1,
        j = 0;

    if (v && data_size > 0 && cmp_func)
    {
        copy = vector_copy(v, data_size, 0, -1);
        size = vector_size(v);

        while (i < size)
        {
            j = i;

            while (j > 0 && (*cmp_func)(vector_get(copy, j), vector_get(copy, j - 1)) == -1)
            {
                data = vector_remove(copy, j);
                vector_insert(copy, j - 1, data);
                j--;
            }
            i++;
        }
    }

    return copy;
}

vector *vector_sort_bubble(const vector *v, int data_size, int (*cmp_func)(void *, void *))
{
    vector *copy = NULL;
    void **data = NULL;
    int size = 0,
        swapped = 0;

    if (v && data_size > 0 && cmp_func)
    {
        copy = vector_copy(v, data_size, 0, -1);
        size = vector_size(v);

        do
        {
            swapped = 0;

            for (int i = 1; i < size; i++)
            {
                if ((*cmp_func)(vector_get(copy, i), vector_get(copy, i - 1)) == -1)
                {
                    data = vector_remove(copy, i);
                    vector_insert(copy, i - 1, data);
                    swapped = 1;
                }
            }
        } while (swapped);
    }

    return copy;
}

vector *vector_sort_selection(const vector *v, int data_size,
                              int (*cmp_func)(void **, int))
{
    vector *copy = NULL,
           *subset_vector = NULL;
    void *data = NULL;
    int size = 0,
        index = 0;

    if (v && data_size > 0 && cmp_func)
    {
        copy = vector_copy(v, data_size, 0, -1);
        size = vector_size(copy);

        for (int i = 0; i < size - 1; i++)
        {
            subset_vector = vector_copy(copy, data_size, i, -1);
            index = (*cmp_func)(subset_vector->a, size - i);
            data = vector_remove(copy, i + index);
            vector_insert(copy, i, data);
            vector_free(subset_vector);
        }
    }

    return copy;
}

vector *vector_sort_shell(const vector *v, int data_size, int (*cmp_func)(void *, void *))
{
    /* See https://startutorial.com/view/data-structure-and-algorithm-shell-sort */

    vector *copy = NULL;
    void *data = NULL;
    int size = 0,
        gap = 0,
        comparison = 0,
        i = 0,
        iteration = 1;

    if (v && data_size > 0 && cmp_func)
    {
        copy = vector_copy(v, data_size, 0, -1);
        size = vector_size(v);
        gap = (int)floor(size / (2 * iteration));

        while (gap > 0)
        {
            i = 0;
            while (i < size - gap)
            {
                comparison = (*cmp_func)(vector_get(copy, i),
                                         vector_get(copy, i + gap));

                if (comparison == 1)
                {
                    data = copy->a[i];
                    copy->a[i] = copy->a[i + gap];
                    copy->a[i + gap] = data;
                }

                i++;
            }
            gap = (int)floor(size / (2 * ++iteration));
        }
    }

    return copy;
}

void merge(const vector *v, int data_size, int (*cmp_func)(void *, void *),
           int start_index, int mid_index, int last_index)
{
    int i = 0,
        j = 0,
        k = start_index,
        comparison = 0,
        size_L = mid_index - start_index + 1,
        size_R = last_index - mid_index;

    void *L[size_L],
        *R[size_R];

    for (i = start_index; i <= mid_index; i++)
    {
        L[i - start_index] = v->a[i];
    }
    for (j = mid_index + 1; j <= last_index; j++)
    {
        R[j - mid_index - 1] = v->a[j];
    }

    i = 0;
    j = 0;
    while (i < size_L && j < size_R)
    {
        comparison = (*cmp_func)(L[i], R[j]);

        if (comparison == -1 || comparison == 0)
        {
            v->a[k++] = L[i++];
        }
        else
        {
            v->a[k++] = R[j++];
        }
    }
    while (i < size_L)
    {
        v->a[k++] = L[i++];
    }
    while (j < size_R)
    {
        v->a[k++] = R[j++];
    }
}

void merge_helper(const vector *v, int data_size,
                  int (*cmp_func)(void *, void *), int start_index, int last_index)
{
    int mid_index = 0;

    if (start_index < last_index)
    {
        mid_index = (int)floor((start_index + last_index) / 2);
        merge_helper(v, data_size, cmp_func, start_index, mid_index);
        merge_helper(v, data_size, cmp_func, mid_index + 1, last_index);
        merge(v, data_size, cmp_func, start_index, mid_index, last_index);
    }
}

vector *vector_sort_merge(const vector *v, int data_size,
                          int (*cmp_func)(void *, void *))
{
    vector *copy = (vector *)NULL;
    int size = 0;

    if (v && data_size > 0 && cmp_func)
    {
        copy = vector_copy(v, data_size, 0, -1);
        size = vector_size(copy);
        merge_helper(copy, data_size, cmp_func, 0, size - 1);
    }

    return copy;
}
