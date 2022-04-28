#ifndef _VECTOR
#define _VECTOR

typedef struct
{
    void **a;
    int size;
    int max_size;
} vector;

/**
 * @brief Initializes a vector
 * @param ini_size Maximum size of the vector (must be >= 0)
 * @return Pointer to the vector
 */
vector *vector_new(int ini_size);

/**
 * @brief Frees the memory taken up by a vector (also frees what's taken
 * up by v->a !)
 * @param v Vector
 * @return (void)
 */
void vector_free(vector *v);

/**
 * @brief Returns the current size of the vector
 * @param v Vector
 * @return Size of the vector, or -1 if v is NULL
 */
int vector_size(const vector *v);

/**
 * @brief Used to check if a vector is full (duh!)
 * @param v Vector
 * @return 1 if vector is full; -1 if v is NULL; 0 otherwise
 */
int vector_isfull(vector *v);

/**
 * @brief Used to check if a vector is empty (duh!)
 * @param v Vector
 * @return 1 if vector is empty; -1 if v is NULL; 0 otherwise
 */
int vector_isempty(vector *v);

/**
 * @brief Returns the pointer at the specified index
 * @param v Vector
 * @param index Index of v->a
 * @return Pointer to data at specified index, or NULL if index is
 * invalid
 */
void *vector_get(const vector *v, int index);

/**
 * @brief Sets the value of vector at a specified index
 * @param v Vector
 * @param index Index (must be between 0 and v->size - 1)
 * @param value Value
 * @return Pointer to value, or NULL if something failed
 */
void *vector_set(vector *v, int index, void *value);

/**
 * @brief Adds a value to the last element of a vector
 * @param v Vector
 * @param value Pointer to the value
 * @return 1 if succeeded, 0 otherwise
 */
int vector_add(vector *v, void *value);

/**
 * @brief Inserts a value at the specified index
 * @param v Vector
 * @param index Index
 * @param value Value
 * @return 1 if successful, 0 otherwise
 */
int vector_insert(vector *v, int index, void *value);

/**
 * @brief Removes an element from the vector at the specified index
 * @param v Vector
 * @param index Index
 * @return Pointer to the removed element, so that it can be freed, or
 * NULL if something failed.
 */
void *vector_remove(vector *v, int index);

/**
 * @brief Calls a function for each element in the vector
 * @param v Vector
 * @param func Pointer to function. Must take one argument, a pointer to
 * the element.
 * @return (void)
 */
void vector_print(vector *v, void (*func)(void *));

/**
 * @brief Returns the maximum size of a given vector
 * @param v Vector
 * @return Maximum size of the vector
 */
int vector_max_size(const vector *v);

/**
 * @brief Sorts a vector using the insertion algorithm
 * @param v Vector
 * @param data_size Size (in bytes) of each item in the vector
 * @param cmp_func A function to use as comparison. Must take two
 * parameters, a and b, and return 0 if a == b, -1 if a < b or 1
 * if a > b
 * @return A new, sorted vector
 */
vector *vector_sort_insertion(const vector *v, int data_size, int (*cmp_func)(void *, void *));

/**
 * @brief Returns a copy of a vector, at a diferent memory address
 * @param v Vector
 * @param data_size Size (in bytes) of each item in the vector
 * @param li Index from which to start copying.
 * @param ui Index at which to stop copying. Set to -1 to copy until the
 * last item.
 * @return A copy of the vector
 */
vector *vector_copy(const vector *v, int data_size, int li, int ui);

/**
 * @brief Sorts a vector using the bubble sort algorithm
 * @param v Vector
 * @param data_size Size (in bytes) of each item in the vector
 * @param cmp_func A function to use as comparison. Must take two
 * parameters, a and b, and return 0 if a == b, -1 if a < b or 1
 * if a > b
 * @return A new, sorted vector
 */
vector *vector_sort_bubble(const vector *v, int data_size, int (*cmp_func)(void *, void *));

/**
 * @brief Sorts a vector using the selection sort algorithm
 * @param v Vector
 * @param data_size Size (in bytes) of each item in the vector
 * @param cmp_func A function to use as comparison. Must take two
 * parameters: list (a list of the remaining items) and length (the
 * number of remaining items). Must return the index of the item of
 * interest.
 * For instance, if you want to sort a vector in ascending order, this
 * function would return the index of the minimum value it finds every
 * time it's called.
 * @return A new, sorted vector
 */
vector *vector_sort_selection(const vector *v, int data_size, int (*cmp_func)(void **, int));

/**
 * @brief Sorts a vector using the shell sort algorithm
 * @param v Vector
 * @param data_size Size (in bytes) of each item in the vector
 * @param cmp_func A function to use as comparison. Must take two
 * parameters, a and b, and return 0 if a == b, -1 if a < b or 1
 * if a > b
 * @return A new, sorted vector
 */
vector *vector_sort_shell(const vector *v, int data_size, int (*cmp_func)(void *, void *));

#endif