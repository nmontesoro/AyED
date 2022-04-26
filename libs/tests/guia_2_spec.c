#include "../vector.h"
#include "../guia_2.h"
#include "../../bdd-for-c.h"

spec("guia_2")
{
    static vector *v = NULL;

    before_each()
    {
        v = vector_new(100);
    }

    after_each()
    {
        vector_free(v);
    }

    describe("fill_vector_with_random_fractions")
    {
        context("when v is NULL")
        {
            it("should return NULL")
            {
                check(!fill_vector_with_random_fractions(NULL, 100));
            }
        }

        context("when n is negative")
        {
            it("should return NULL")
            {
                check(!fill_vector_with_random_fractions(v, -1));
            }
        }

        context("when n > v->max_size")
        {
            it("should return NULL")
            {
                check(!fill_vector_with_random_fractions(v, 150));
            }
        }

        context("when both v and n are valid")
        {
            it("should create n fractions")
            {
                int n = 10;
                fill_vector_with_random_fractions(v, n);
                check(vector_size(v) == n, "n != v->size");
            }
        }
    }

    describe("fill_vector_with_random_cpx_numbers")
    {
        context("when v is NULL")
        {
            it("should return NULL")
            {
                check(!fill_vector_with_random_cpx_numbers(NULL, 100));
            }
        }

        context("when n is negative")
        {
            it("should return NULL")
            {
                check(!fill_vector_with_random_cpx_numbers(v, -1));
            }
        }

        context("when n > v->max_size")
        {
            it("should return NULL")
            {
                check(!fill_vector_with_random_cpx_numbers(v, 150));
            }
        }

        context("when both v and n are valid")
        {
            it("should create n complex numbers")
            {
                int n = 10;
                fill_vector_with_random_cpx_numbers(v, n);
                check(vector_size(v) == n, "n != v->size");
            }
        }
    }
}