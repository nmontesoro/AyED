#ifndef LIB_LISTS
#define LIB_LISTS

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct __list_node_t
{
    struct __list_node_t *previous;
    void *value;
    struct __list_node_t *next;
} _list_node_t;

typedef struct _list
{
    _list_node_t *head;
    uint32_t max_size;
    uint32_t length;
} list_t;

/**
 * @brief Creates a new list
 * @param max_size Maximum number of items
 * @return A pointer to the list, or NULL if something failed
 */
list_t *list_new(uint32_t max_size);

/**
 * @brief Checks if a list is empty
 * @param list The list
 * @return True if the list is empty; false otherwise
 */
bool list_is_empty(list_t *list);

/**
 * @brief Checks if a list is full
 * @param list The list
 * @return True if the list is full; false otherwise
 */
bool list_is_full(list_t *list);

/**
 * @brief Returns the length of the list
 * @param list The list
 * @return A number
 */
uint32_t list_get_length(list_t *list);

/**
 * @brief Returns a pointer to the value at the specified index
 * @param list The list
 * @param index The index
 * @return Pointer to value at specified index, or NULL if something
 * failed
 */
void *list_get(list_t *list, uint32_t index);

/**
 * @brief Returns the index at which a given element can be found in the
 * list
 * @param list The list
 * @param value A pointer to the value
 * @return The index, or UINT32_MAX if the item could not be found
 */
/* uint32_t list_index_of(list_t *list, void *value); */
uint32_t list_index_of(list_t *list, void *ctx, bool cmp(void *current_value, void *ctx));

/**
 * @brief Inserts a value into a list
 * @param list The list
 * @param value The value
 * @param index The index at which to insert the value. If
 * index > list.length, then the value is inserted at the end of the
 * list
 * @return True if the operation succeeded; false otherwise
 */
bool list_insert(list_t *list, void *value, uint32_t index);

/**
 * @brief Inserts a value at the end of a list
 * @param list The list
 * @param value The value
 * @return True if the operation succeeded; false otherwise
 */
bool list_append(list_t *list, void *value);

/**
 * @brief Removes the item at the specified index
 * @param list The list
 * @param index The index
 * @return A pointer to the value, so it can be freed
 */
void *list_remove_at(list_t *list, uint32_t index);

/**
 * @brief Removes the items that match a specified criteria
 * @param list The list
 * @param ctx Optional pointer to context
 * @param cmp A function that returns true when a match is found
 * @param _free A function to free the memory taken up by the value
 * @return The number of removals made
 */
uint32_t list_remove(list_t *list, void *ctx, bool cmp(void *val, void *ctx),
                      void _free(void *val));

/**
 * @brief Calls a function for each item in the list
 * @param list The list
 * @param context An optional pointer to a context object
 * @param callback The callback function. Takes 2 parameters: the
 * current list node, and an optional pointer to context
 * @return (void)
 */
void list_traverse(list_t *list, void *context,
                   void callback(_list_node_t *current_node, void *context));

/**
 * @brief Creates a new list node
 * @param value The value
 * @param previous A pointer to the previous node (can be NULL)
 * @param next A pointer to the next node (can be NULL)
 * @return A pointer to the node, or NULL if something failed
 */
_list_node_t *_list_node_new(void *value, _list_node_t *previous,
                             _list_node_t *next);

/**
 * @brief Frees a list. User should call list_traverse to free each
 * element individually, if needed
 * @param list The list
 * @param callback A free-like function to call for each value in the
 * list
 * @return True if the operation succeeded; false otherwise
 */
bool list_free(list_t *list);

#endif