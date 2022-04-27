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

spec("vector_sort")
{
    vector *v = NULL;

    describe("vector_sort_insertion")
    {

        context("when v is NULL")
        {
            xit("should return NULL")
            {
                check(!vector_sort_insertion(NULL, 4, &cmp_func));
            }
        }

        context("when comparison function pointer is NULL")
        {
            xit("should return NULL")
            {
                check(!vector_sort_insertion(v, 4, NULL));
            }
        }

        context("when data_size is invalid")
        {
            xit("should return NULL")
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
}