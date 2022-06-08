#include <stdlib.h>
#include <string.h>

#include "libusers.h"

user_t *user_new(uint8_t user_id, char *name, char *password)
{
    user_t *new_user = NULL;

    if (name && password && (new_user = malloc(sizeof(user_t))))
    {
        new_user->user_id = user_id;
        new_user->name = name;
        new_user->password = password;
    }

    return new_user;
}

bool user_free(user_t *user)
{
    bool result = false;

    if (user)
    {
        free(user->password);
        user->password = NULL;

        free(user->name);
        user->name = NULL;

        free(user);
        user = NULL;

        result = true;
    }

    return result;
}

uint8_t user_get_id(user_t *user)
{
    uint8_t user_id = UINT8_MAX;

    if (user)
    {
        user_id = user->user_id;
    }

    return user_id;
}

bool user_set_id(user_t *user, uint8_t user_id)
{
    bool result = false;

    if (user)
    {
        user->user_id = user_id;
        result = true;
    }

    return result;
}

char *user_get_name(user_t *user)
{
    char *name = NULL;
    size_t name_length;

    if (user && user->name)
    {
        name_length = strlen(user->name) + 1; // +1 for '\0'

        if ((name = malloc(sizeof(char) * name_length)))
        {
            strncpy(name, user->name, name_length);
        }
    }

    return name;
}

bool user_set_name(user_t *user, char *name)
{
    bool result = false;

    if (user && name)
    {
        free(user->name);
        user->name = name;
        result = true;
    }

    return result;
}

char *user_get_password(user_t *user)
{
    char *password = NULL;
    size_t pwd_length = 0;

    if (user)
    {
        pwd_length = strlen(user->password) + 1; // +1 for '\0'

        if ((password = malloc(sizeof(char) * pwd_length)))
        {
            strncpy(password, user->password, pwd_length);
        }
    }

    return password;
}

bool user_set_password(user_t *user, char *password)
{
    bool result = false;

    if (user && password)
    {
        free(user->password);
        user->password = password;
        result = true;
    }

    return result;
}

bool _user_list_get_by_name_helper(void *user, void *ctx)
{
    return (strcmp((char *)ctx, ((user_t *)user)->name) == 0);
}

user_t *user_list_get_by_name(const list_t *list, const char *name)
{
    user_t *user = NULL;

    if (list && name)
    {
        user = (user_t *)list_find(list, (void *)name,
                                   _user_list_get_by_name_helper);
    }

    return user;
}

bool _user_list_get_by_id_helper(void *user, void *id)
{
    return (*(uint8_t *)id == ((user_t *)user)->user_id);
}

user_t *user_list_get_by_id(const list_t *list, uint8_t id)
{
    user_t *user = NULL;

    if (list)
    {
        user = (user_t *)list_find(list, (void *)&id,
                                   _user_list_get_by_id_helper);
    }

    return user;
}

void _user_list_free_helper(void *user, void *ctx)
{
    *(bool *)ctx &= user_free((user_t *)user);
}

bool user_list_free(list_t *list)
{
    bool result = true;

    list_traverse(list, &result, _user_list_free_helper);

    if (result)
    {
        result &= list_free(list);
    }

    return result;
}
