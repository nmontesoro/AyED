#include <stdlib.h>
#include <string.h>

#include "libgroups.h"

group_t *group_new(uint8_t group_id, char *name)
{
    group_t *new_group = NULL;

    if (name && (new_group = malloc(sizeof(group_t))))
    {
        new_group->group_id = group_id;
        new_group->name = name;

        if (!(new_group->users = list_new(UINT8_MAX)))
        {
            free(new_group);
            new_group = NULL;
        }
    }

    return new_group;
}

bool group_free(group_t *group)
{
    bool result = false;

    if (group && list_free(group->users))
    {
        group->users = NULL;
        free(group->name);
        group->name = NULL;
        free(group);
        group = NULL;
        result = true;
    }

    return result;
}

uint8_t group_get_id(const group_t *group)
{
    uint8_t group_id = UINT8_MAX;

    if (group)
    {
        group_id = group->group_id;
    }

    return group_id;
}

bool group_set_id(group_t *group, uint8_t group_id)
{
    bool result = false;

    if (group)
    {
        group->group_id = group_id;
        result = true;
    }

    return result;
}

char *group_get_name(group_t *group)
{
    char *name = NULL;
    size_t name_length = 0;

    if (group)
    {
        name_length = strlen(group->name) + 1; // +1 for '\0'

        if ((name = malloc(sizeof(char) * name_length)))
        {
            strncpy(name, group->name, name_length);
        }
    }

    return name;
}

bool group_set_name(group_t *group, char *name)
{
    bool result = false;

    if (group && name)
    {
        free(group->name);
        group->name = name;
        result = true;
    }

    return result;
}

bool group_add_user(group_t *group, user_t *user)
{
    bool result = false;

    if (group && user)
    {
        result = list_append(group->users, (void *)user);
    }

    return result;
}

bool _group_index_of_user_helper(void *val, void *ctx)
{
    return val == ctx;
}

bool group_remove_user(group_t *group, user_t *user)
{
    bool result = false;
    uint32_t index = 0;

    if (group && user)
    {
        if (((index = list_index_of(group->users, (void *)user, _group_index_of_user_helper)) != UINT32_MAX))
        {
            result = list_remove_at(group->users, index);
        }
    }

    return result;
}

bool _user_in_group_helper(void *val, void *ctx)
{
    return val == ctx;
}

bool user_in_group(group_t *group, user_t *user)
{
    bool result = false;

    if (group && user)
    {
        result = list_index_of(group->users, user, _user_in_group_helper) != UINT32_MAX;
    }

    return result;
}

group_t *group_list_get_by_name(const list_t *list, const char *name)
{
    group_t *group = NULL;
    _list_node_t *current_node = NULL;

    if (list && name)
    {
        current_node = list->head;

        while (current_node)
        {
            if (strcmp(name, ((group_t *)current_node->value)->name) == 0)
            {
                group = (group_t *)current_node->value;
                current_node = NULL;
            }
            else
            {
                current_node = current_node->next;
            }
        }
    }

    return group;
}

group_t *group_list_get_by_id(const list_t *list, uint8_t id)
{
    group_t *group = NULL;
    _list_node_t *current_node = NULL;

    if (list)
    {
        current_node = list->head;

        while (current_node)
        {
            if (id == ((group_t *)current_node->value)->group_id)
            {
                group = (group_t *)current_node->value;
                current_node = NULL;
            }
            else
            {
                current_node = current_node->next;
            }
        }
    }

    return group;
}

user_t *group_get_member_by_id(const group_t *group, uint8_t id)
{
    user_t *user = NULL;

    if (group)
    {
        user = user_list_get_by_id(group->users, id);
    }

    return user;
}

user_t *group_get_member_by_name(const group_t *group, const char *name)
{
    user_t *user = NULL;

    if (group)
    {
        user = user_list_get_by_name(group->users, name);
    }

    return user;
}

void group_traverse(const group_t *group, void *ctx,
                    void callback(_list_node_t *user, void *ctx))
{
    if (group)
    {
        list_traverse(group->users, ctx, callback);
    }
}

void _groups_list_free_helper(_list_node_t *node, void *ctx)
{
    *(bool *)ctx &= group_free((group_t *)node->value);
    node->value = NULL;
}

bool groups_list_free(list_t *list)
{
    bool result = true;

    if (list)
    {
        list_traverse(list, &result, _groups_list_free_helper);
    }

    return result;
}

list_t *group_get_members(const group_t *group)
{
    list_t *list = NULL;

    if (group)
    {
        list = group->users;
    }

    return list;
}
