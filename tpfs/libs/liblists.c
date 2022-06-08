#include "liblists.h"

list_t *list_new(uint32_t max_size)
{
    list_t *new_list = (list_t *)NULL;

    if ((new_list = (list_t *)malloc(sizeof(list_t))))
    {
        new_list->head = (_list_node_t *)NULL;
        new_list->length = 0;
        new_list->max_size = max_size;
    }

    return new_list;
}

bool list_is_empty(list_t *list)
{
    return list && !(list->length);
}

bool list_is_full(list_t *list)
{
    return list && (list->length == list->max_size);
}

uint32_t list_get_length(list_t *list)
{
    uint32_t length = 0;

    if (list)
    {
        length = list->length;
    }

    return length;
}

void *list_get(list_t *list, uint32_t index)
{
    void *item = NULL;
    _list_node_t *current_node = NULL;

    if (list && index < list->length)
    {
        if ((current_node = list->head))
        {

            for (uint32_t i = 1; i <= index; i++)
            {
                current_node = current_node->next;
            }

            item = current_node->value;
        }
    }

    return item;
}

void list_traverse(list_t *list, void *context,
                   void callback(_list_node_t *current_node, void *context))
{
    _list_node_t *current_node = NULL;

    if (list)
    {
        current_node = list->head;

        for (uint32_t i = 0; i < list->length; i++)
        {
            callback(current_node, context);
            current_node = current_node->next;
        }
    }
}

uint32_t list_index_of(list_t *list, void *ctx, bool cmp(void *current_value, void *ctx))
{
    uint32_t index = UINT32_MAX,
             current_index = 0;

    _list_node_t *current_node = NULL;

    if (list && cmp)
    {
        current_node = list->head;

        while (current_node && index == UINT32_MAX)
        {
            if (cmp(current_node->value, ctx))
            {
                index = current_index;
            }
            else
            {
                current_node = current_node->next;
                current_index++;
            }
        }
    }

    return index;
}

_list_node_t *_list_node_new(void *value, _list_node_t *previous,
                             _list_node_t *next)
{
    _list_node_t *result = NULL;

    if ((result = (_list_node_t *)malloc(sizeof(_list_node_t))))
    {
        result->previous = previous;
        result->value = value;
        result->next = next;
    }

    return result;
}

bool list_insert(list_t *list, void *value, uint32_t index)
{
    bool result = false;
    _list_node_t *aux = NULL,
                 *new_node = NULL;
    uint32_t i = 1;

    if (list && value && !list_is_full(list) &&
        (new_node = _list_node_new(value, NULL, NULL)))
    {
        if (index > list->length)
        {
            index = list->length;
        }

        if (index == 0)
        {
            aux = list->head;
            list->head = new_node;
            new_node->next = aux;

            if (new_node->next)
            {
                new_node->next->previous = new_node;
            }
        }
        else
        {
            aux = list->head;

            while (i < index)
            {
                aux = aux->next;
                i++;
            }

            new_node->next = aux->next;
            new_node->previous = aux;
            aux->next = new_node;

            if (new_node->next)
            {
                new_node->next->previous = new_node;
            }
        }

        list->length++;
        result = true;
    }

    return result;
}

bool list_append(list_t *list, void *value)
{
    bool result = false;
    uint32_t index = 0;

    if (list)
    {
        index = list->length;
    }

    result = list_insert(list, value, index);

    return result;
}

void *list_remove_at(list_t *list, uint32_t index)
{
    void *removed_value = NULL;
    _list_node_t *removed_node = NULL;

    if (list && index < list->length)
    {
        removed_node = list->head;

        for (uint32_t i = 0; i < index; i++)
        {
            removed_node = removed_node->next;
        }

        if (index == 0)
        {
            list->head = removed_node->next;
        }

        if (removed_node->previous)
        {
            removed_node->previous->next = removed_node->next;
        }

        if (removed_node->next)
        {
            removed_node->next->previous = removed_node->previous;
        }
        removed_value = removed_node->value;

        list->length--;
        free(removed_node);
    }

    return removed_value;
}

uint32_t list_remove(list_t *list, void *ctx, bool cmp(void *val, void *ctx),
                     void _free(void *val))
{
    /* TODO: Rewrite to only go through the list once */

    uint32_t removed_count = 0,
             index = 0;
    _list_node_t *current_node = NULL;
    void *free_node = NULL,
         *value = NULL;

    if (list && cmp)
    {
        current_node = list->head;

        while (current_node)
        {
            value = current_node->value;
            current_node = current_node->next;

            if (cmp(value, ctx))
            {
                if ((free_node = list_remove_at(list, index)))
                {
                    _free(free_node);
                    removed_count++;
                }
            }
            
            index++;
        }
    }

    return removed_count;
}

bool list_free(list_t *list)
{
    bool result = false;
    _list_node_t *current_node = NULL,
                 *next_node = NULL;

    if (list)
    {
        next_node = list->head;

        while ((current_node = next_node))
        {
            next_node = current_node->next;
            free(current_node);
            current_node = NULL;
        }

        free(list);
        list = NULL;
        result = true;
    }

    return result;
}

void *list_find(const list_t *list, void *ctx, bool cmp(void *val, void *ctx))
{
    void *item = NULL;
    _list_node_t *current_node = NULL;

    if (list && cmp)
    {
        current_node = list->head;

        while (current_node)
        {
            if (cmp(current_node->value, ctx))
            {
                item = current_node->value;
                current_node = NULL;
            }
            else
            {
                current_node = current_node->next;
            }
        }
    }

    return item;
}

list_t *list_find_all(const list_t *list, void *ctx,
                      bool cmp(void *val, void *ctx))
{
    list_t *new_list = NULL;
    _list_node_t *current_node = NULL;

    if (list && cmp)
    {
        if ((new_list = list_new(list->max_size)))
        {
            current_node = list->head;

            while (current_node)
            {
                if (cmp(current_node->value, ctx))
                {
                    list_append(new_list, current_node->value);
                }

                current_node = current_node->next;
            }
        }
    }

    return new_list;
}

void _nullf(void *var) {}

bool _ptrs_match(void *val, void *ctx)
{
    return (val == ctx);
}