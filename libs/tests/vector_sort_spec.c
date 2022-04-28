#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "../vector.h"
#include "../../bdd-for-c.h"

int cmp_func(void *a_ptr, void *b_ptr);
int check_vector_result(vector *v);

int cmp_func(void *a_ptr, void *b_ptr)
{
    int a = *(int *)a_ptr,
        b = *(int *)b_ptr,
        r = 0;

    if (a > b)
    {
        r = 1;
    }
    else if (a < b)
    {
        r = -1;
    }
    return r;
}

int cmp_func_sel(void **list, int length)
{
    int min = *(int *)list[0];
    int index = 0;

    for (int i = 1; i < length; i++)
    {
        int val = *(int *)list[i];
        if (val < min)
        {
            min = val;
            index = i;
        }
    }

    return index;
}

spec("vector_sort")
{
    vector *v = NULL;

    describe("vector_sort_insertion")
    {

        context("when v is NULL")
        {
            it("should return NULL")
            {
                check(!vector_sort_insertion(NULL, 4, &cmp_func));
            }
        }

        context("when comparison function pointer is NULL")
        {
            it("should return NULL")
            {
                check(!vector_sort_insertion(v, 4, NULL));
            }
        }

        context("when data_size is invalid")
        {
            it("should return NULL")
            {
                check(!vector_sort_insertion(v, -1, &cmp_func));
                check(!vector_sort_insertion(v, 0, &cmp_func));
            }
        }

        context("when everything is right")
        {
            v = vector_new(10);
            int stored_val = 0;

            /* [0, -1, 2, -3, 4, -5, 6, -7, 8, -9] */
            for (int i = 0; i < 10; i++)
            {
                int *val = (int *)malloc(sizeof(int));
                *val = pow(-1, i) * i;
                vector_add(v, (void *)val);
            }

            it("should have sorted the values")
            {
                vector *v_sorted = vector_sort_insertion(v, sizeof(int), &cmp_func);

                vector_free(v);
                v = v_sorted;

                int correct_vals[] = {-9, -7, -5, -3, -1, 0, 2, 4, 6, 8};

                for (int i = 0; i < 10; i++)
                {
                    stored_val = *(int *)vector_get(v, i);

                    check(stored_val == correct_vals[i],
                          "Incorrect value at index %i", i);
                }

                vector_free(v_sorted);
            }
        }
    }

    describe("vector_sort_bubble")
    {

        context("when v is NULL")
        {
            it("should return NULL")
            {
                check(!vector_sort_bubble(NULL, 4, &cmp_func));
            }
        }

        context("when comparison function pointer is NULL")
        {
            it("should return NULL")
            {
                check(!vector_sort_bubble(v, 4, NULL));
            }
        }

        context("when data_size is invalid")
        {
            it("should return NULL")
            {
                check(!vector_sort_bubble(v, -1, &cmp_func));
                check(!vector_sort_bubble(v, 0, &cmp_func));
            }
        }

        context("when everything is right")
        {
            v = vector_new(10);
            int stored_val = 0;

            /* [0, -1, 2, -3, 4, -5, 6, -7, 8, -9] */
            for (int i = 0; i < 10; i++)
            {
                int *val = (int *)malloc(sizeof(int));
                *val = pow(-1, i) * i;
                vector_add(v, (void *)val);
            }

            it("should have sorted the values")
            {
                vector *v_sorted = vector_sort_bubble(v, sizeof(int), &cmp_func);

                vector_free(v);
                v = v_sorted;

                int correct_vals[] = {-9, -7, -5, -3, -1, 0, 2, 4, 6, 8};

                for (int i = 0; i < 10; i++)
                {
                    stored_val = *(int *)vector_get(v, i);

                    check(stored_val == correct_vals[i],
                          "Incorrect value at index %i", i);
                }

                vector_free(v_sorted);
            }
        }
    }

    describe("vector_sort_selection")
    {
        context("when v is NULL")
        {
            it("should return NULL")
            {
                check(!vector_sort_selection(NULL, 4, &cmp_func_sel));
            }
        }

        context("when comparison function pointer is NULL")
        {
            it("should return NULL")
            {
                check(!vector_sort_selection(v, 4, NULL));
            }
        }

        context("when data_size is invalid")
        {
            it("should return NULL")
            {
                check(!vector_sort_selection(v, -1, &cmp_func_sel));
                check(!vector_sort_selection(v, 0, &cmp_func_sel));
            }
        }

        context("when everything is right")
        {
            v = vector_new(10);
            int stored_val = 0;

            /* [0, -1, 2, -3, 4, -5, 6, -7, 8, -9] */
            for (int i = 0; i < 10; i++)
            {
                int *val = (int *)malloc(sizeof(int));
                *val = pow(-1, i) * i;
                vector_add(v, (void *)val);
            }

            it("should have sorted the values")
            {
                vector *v_sorted = vector_sort_selection(v, sizeof(int), &cmp_func_sel);

                vector_free(v);
                v = v_sorted;

                int correct_vals[] = {-9, -7, -5, -3, -1, 0, 2, 4, 6, 8};

                for (int i = 0; i < 10; i++)
                {
                    stored_val = *(int *)vector_get(v, i);

                    check(stored_val == correct_vals[i],
                          "Incorrect value at index %i", i);
                }

                vector_free(v_sorted);
            }
        }
    }

    describe("vector_sort_shell")
    {
        context("when v is NULL")
        {
            it("should return NULL")
            {
                check(!vector_sort_shell(NULL, 4, &cmp_func));
            }
        }

        context("when comparison function pointer is NULL")
        {
            it("should return NULL")
            {
                check(!vector_sort_shell(v, 4, NULL));
            }
        }

        context("when data_size is invalid")
        {
            it("should return NULL")
            {
                check(!vector_sort_shell(v, -1, &cmp_func));
                check(!vector_sort_shell(v, 0, &cmp_func));
            }
        }

        context("when everything is right")
        {
            v = vector_new(10);
            int stored_val = 0;

            /* [0, -1, 2, -3, 4, -5, 6, -7, 8, -9] */
            for (int i = 0; i < 10; i++)
            {
                int *val = (int *)malloc(sizeof(int));
                *val = pow(-1, i) * i;
                vector_add(v, (void *)val);
            }

            it("should have sorted the values")
            {
                vector *v_sorted = vector_sort_shell(v, sizeof(int), &cmp_func);

                vector_free(v);
                v = v_sorted;

                int correct_vals[] = {-9, -7, -5, -3, -1, 0, 2, 4, 6, 8};

                for (int i = 0; i < 10; i++)
                {
                    stored_val = *(int *)vector_get(v, i);

                    check(stored_val == correct_vals[i],
                          "Incorrect value at index %i", i);
                }

                vector_free(v_sorted);
            }
        }
    }
}