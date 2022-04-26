#include "../vector.h"
#include "../../bdd-for-c.h"

spec("vector")
{
    vector *v = NULL;

    it("vector_new should return NULL if ini_size <= 0")
    {
        check(!vector_new(0));
        check(!vector_new(-1));
    }

    it("vector_new should return an empty vector pointer with correct max_size")
    {
        v = vector_new(5);
        check(v->a);
        check(v->size == 0);
        check(v->max_size == 5)
            free(v->a);
        free(v);
        v = NULL;
    }

    it("vector_free should free the memory taken up by all items in v->a")
    {
        v = vector_new(5);

        /* Add some items to v->a */
        int **a = malloc(sizeof(int *) * 5);
        for (int i = 0; i < 5; i++)
        {
            a[i] = malloc(sizeof(int));
            *(a[i]) = i;
        }

        /* Free the memory taken up by the current v->a */
        free(v->a);

        /* Add the data */
        v->a = (void **)a;

        /* Now call vector_free and check that everything works as
        expected */
        vector_free(v);
    }

    it("vector_size should return -1 if v is NULL")
    {
        check(vector_size(NULL) == -1);
    }

    it("vector_size should return the size of the vector")
    {
        v = vector_new(5);

        /* Manually set the size */
        v->size = 42;

        /* Check that function returns the correct size */
        check(vector_size(v) == 42);

        /* Set the size back to 0 so as to not cause problems because we
        didn't use vector_add */
        v->size = 0;

        vector_free(v);
    }

    it("vector_isfull should return -1 if v is NULL")
    {
        check(vector_isfull(NULL) == -1);
    }

    it("vector_isfull should return 1 only when vector is full")
    {
        v = vector_new(10);

        /* Manually set size to a new value, check that isfull is only true when
        size == 10 */
        for (int i = 1; i <= 10; i++)
        {
            v->size = i;

            if (i == 10)
            {
                check(vector_isfull(v));
            }
            else
            {
                check(!vector_isfull(v));
            }
        }

        /* Set size back to 0 because we didn't use vector_add */
        v->size = 0;

        vector_free(v);
    }

    it("vector_isempty should return -1 if v is NULL")
    {
        check(vector_isempty(NULL) == -1);
    }

    it("vector_isempty should return 1 only when vector is empty")
    {
        v = vector_new(10);

        /* Manually set size to a new value, check that isfull is only true when
        size == 10 */
        for (int i = 0; i <= 10; i++)
        {
            v->size = i;

            if (i == 0)
            {
                check(vector_isempty(v));
            }
            else
            {
                check(!vector_isempty(v));
            }
        }

        /* Set size back to 0 because we didn't use vector_add */
        v->size = 0;

        vector_free(v);
    }

    it("vector_get should return NULL if index is invalid or v is NULL")
    {
        v = vector_new(1);

        /* Index < 0 */
        check(!vector_get(v, -1));

        /* Index above current size of the vector */
        check(!vector_get(v, 2));

        /* v is NULL */
        check(!vector_get(NULL, 0));

        vector_free(v);
    }

    it("vector_get should return the right pointers")
    {
        v = vector_new(5);

        /* Add some items to v->a */
        int **a = malloc(sizeof(int *) * 5);
        for (int i = 0; i < 5; i++)
        {
            a[i] = malloc(sizeof(int));
            *(a[i]) = i;
        }
        free(v->a);
        v->a = (void **)a;
        v->size = 5;

        /* Make sure that the pointers returned by vector_get are
        correct */
        for (int i = 0; i < 5; i++)
        {
            check(vector_get(v, i) == a[i]);
        }

        vector_free(v);
    }

    it("vector_add should return 0 if v is NULL or value is NULL")
    {
        v = vector_new(1);
        void *p = malloc(sizeof(int));

        /* v is NULL */
        check(!vector_add(NULL, p));

        /* value is NULL */
        check(!vector_add(v, NULL));

        free(p);
        vector_free(v);
    }

    it("vector_add should add a value")
    {
        v = vector_new(2);
        int *a = (int *)malloc(sizeof(int)),
            *b = (int *)malloc(sizeof(int));
        void *pa = NULL,
             *pb = NULL;

        *a = 5;
        *b = 3;

        /* Add the values */
        vector_add(v, (void *)a);
        vector_add(v, (void *)b);

        /* Check that the addresses are OK */
        pa = vector_get(v, 0);
        pb = vector_get(v, 1);
        check(pa == (void *)a, "Address of a has been modified");
        check(pb == (void *)b, "Address of b has been modified");

        /* Check that the values have not been altered */
        check(*(int *)pa == 5, "Value of a has been modified");
        check(*(int *)pb == 3, "Value of b has been modified");

        vector_free(v);
    }

    it("vector_add should return 0 if vector is full")
    {
        v = vector_new(2);
        int *a = (int *)malloc(sizeof(int)),
            *b = (int *)malloc(sizeof(int)),
            *c = (int *)malloc(sizeof(int));

        *a = 4;
        *b = 5;
        *c = 9;

        /* Add the first two values */
        vector_add(v, (void *)a);
        vector_add(v, (void *)b);

        /* Check that adding a new value returns NULL */
        check(!vector_add(v, (void *)c), "vector_add did not return NULL");

        /* Check that the first two values have not been altered */
        check(*(int *)vector_get(v, 0) == 4, "Value of a has been altered");
        check(*(int *)vector_get(v, 1) == 5, "Value of b has been altered");

        vector_free(v);
    }

    it("vector_set should return NULL if v is NULL, if index is invalid, or if value is NULL")
    {
        v = vector_new(2);
        void *val = malloc(sizeof(int));

        /* v is NULL */
        check(!vector_set(v, 0, val), "when v is NULL");

        /* Index is invalid */
        check(!vector_set(v, -1, val), "when index is negative");
        check(!vector_set(v, 15, val), "when index is greater than max_size");

        /* Value is NULL */
        check(!vector_set(v, 0, NULL));

        free(val);
        vector_free(v);
    }

    it("vector_set should modify a value without altering the others")
    {
        v = vector_new(3);
        int **vals = (int **)malloc(sizeof(int) * 3),
            *val = (int *)malloc(sizeof(int));
        void *ptr = NULL;

        for (int i = 0; i < 3; i++)
        {
            vals[i] = (int *)malloc(sizeof(int));
            **(vals + i) = 2 * i;
            vector_add(v, (void *)vals[i]);
        }

        /* Check that all the values were loaded succesfully */
        check(*(int *)vector_get(v, 0) == 0, "Incorrect values loaded");
        check(*(int *)vector_get(v, 1) == 2, "Incorrect values loaded");
        check(*(int *)vector_get(v, 2) == 4, "Incorrect values loaded");

        /* Modify the one at index 1 */
        *val = 6;
        ptr = vector_set(v, 1, (void *)val);

        /* Check that this value was modified correctly, and the others
        were untouched */
        check(*(int *)vector_get(v, 0) == 0, "Incorrect values after vector_set");
        check(*(int *)vector_get(v, 1) == 6, "Incorrect values after vector_set");
        check(*(int *)vector_get(v, 2) == 4, "Incorrect values after vector_set");

        /* Check that the addresses match */
        check(ptr == (void *)val, "Adresses do not match");

        free(vals[1]);
        vector_free(v);
    }

    it("vector_insert should return 0 if v is NULL, index is invalid, or value is NULL")
    {
        v = vector_new(2);
        void *val = malloc(sizeof(int));

        /* v is NULL */
        check(!vector_insert(NULL, 0, val), "when v is NULL");

        /* index is invalid */
        check(!vector_insert(v, -1, val), "when index is invalid");
        check(!vector_insert(v, 15, val), "when index is invalid");

        /* value is NULL */
        check(!vector_insert(v, 0, NULL), "when value is NULL");

        vector_free(v);
    }

    it("vector_insert should insert the value at the specified index, without modifying the other values")
    {
        v = vector_new(5);
        int **vals = (int **)malloc(sizeof(int) * 4),
            *val = (int *)malloc(sizeof(int)),
            v_size = 0;

        /* Adding values 4, 8, 12, 16 */
        for (int i = 0; i < 4; i++)
        {
            vals[i] = (int *)malloc(sizeof(int));
            **(vals + i) = 4 * (i + 1);
            vector_add(v, (void *)vals[i]);
        }
        v_size = vector_size(v); // Should be 4

        /* Insert value at index 2 */
        *val = 1;
        vector_insert(v, 2, val);

        /* Check that all values are there */
        check(*(int *)vector_get(v, 0) == 4, "wrong values after vector_insert");
        check(*(int *)vector_get(v, 1) == 8, "wrong values after vector_insert");
        check(*(int *)vector_get(v, 2) == 1, "wrong values after vector_insert");
        check(*(int *)vector_get(v, 3) == 12, "wrong values after vector_insert");
        check(*(int *)vector_get(v, 4) == 16, "wrong values after vector_insert");

        /* Check that v->size has increased */
        check(vector_size(v) == v_size + 1, "v->size is wrong");

        vector_free(v);
        free(vals);
    }

    it("vector_remove should return NULL if v is NULL or index is invalid")
    {
        v = vector_new(2);

        check(!vector_remove(NULL, 0), "when v is NULL");
        check(!vector_remove(v, -1), "when index is negative");
        check(!vector_remove(v, 15), "when index is greater than size");

        vector_free(v);
    }

    it("vector_remove should remove the specified item without modifying the rest")
    {
        v = vector_new(4);
        int **vals = (int **)malloc(sizeof(int) * 4),
            v_size = 0;

        /* Adding values 4, 8, 12, 16 */
        for (int i = 0; i < 4; i++)
        {
            vals[i] = (int *)malloc(sizeof(int));
            **(vals + i) = 4 * (i + 1);
            vector_add(v, (void *)vals[i]);
        }
        v_size = vector_size(v); // Should be 4

        /* Remove value at index 1 */
        vector_remove(v, 1);

        /* Check that the other values are there */
        check(*(int *)vector_get(v, 0) == 4, "wrong values after vector_remove");
        check(*(int *)vector_get(v, 1) == 12, "wrong values after vector_remove");
        check(*(int *)vector_get(v, 2) == 16, "wrong values after vector_remove");

        /* Check that v->size has decreased */
        check(vector_size(v) == v_size - 1, "v->size is wrong");

        vector_free(v);
        free(vals);
    }

    it("vector_remove should remove the last item if index == size - 1")
    {
        v = vector_new(4);
        int **vals = (int **)malloc(sizeof(int) * 4),
            v_size = 0;

        /* Adding values 4, 8, 12, 16 */
        for (int i = 0; i < 4; i++)
        {
            vals[i] = (int *)malloc(sizeof(int));
            **(vals + i) = 4 * (i + 1);
            vector_add(v, (void *)vals[i]);
        }
        v_size = vector_size(v); // Should be 4

        /* Remove value at index 3 */
        vector_remove(v, 3);

        /* Check that the other values are there */
        check(*(int *)vector_get(v, 0) == 4, "wrong values after vector_remove");
        check(*(int *)vector_get(v, 1) == 8, "wrong values after vector_remove");
        check(*(int *)vector_get(v, 2) == 12, "wrong values after vector_remove");

        /* Check that v->size has decreased */
        check(vector_size(v) == v_size - 1, "v->size is wrong");

        vector_free(v);
        free(vals);
    }
}
