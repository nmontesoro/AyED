#include <stdlib.h>
#include <string.h>

#include "libfiles.h"

/**
 * @brief Sets the time at which the file was modified
 * @param file The file
 * @return True or false, depending on the result of the operation
 */
bool _file_set_modified(file_t *file);

file_t *file_new(char *name, const uint8_t owner_id, const uint8_t group_id,
                 const uint16_t permissions, const uint32_t size,
                 const bool is_directory, void *contents)
{
    file_t *new_file = NULL;

    if ((new_file = malloc(sizeof(file_t))))
    {
        new_file->name = name;
        new_file->owner_id = owner_id;
        new_file->group_id = group_id;
        new_file->permissions = permissions;
        new_file->size = size;
        new_file->is_directory = is_directory;
        new_file->contents = contents;

        new_file->created_on = time(NULL);
        new_file->modified_on = new_file->created_on;
    }

    return new_file;
}

bool file_free(file_t *file)
{
    bool result = false;

    if (file)
    {
        if (file->is_directory)
        {
            result = file_list_free((list_t *)file->contents);
        }
        else
        {
            free(file->name);
            file->name = NULL;

            free(file->contents);
            file->contents = NULL;

            free(file);
            file = NULL;
            result = true;
        }
    }

    return result;
}

bool file_set_name(file_t *file, char *name)
{
    bool result = false;

    if (file)
    {
        free(file->name);
        file->name = name;
        _file_set_modified(file);
        result = true;
    }

    return result;
}

char *file_get_name(const file_t *file)
{
    char *name = NULL;
    size_t name_length = 0;

    if (file && file->name)
    {
        name_length = strlen(file->name) + 1; // +1 for '\0'

        if ((name = malloc(sizeof(char) * name_length)))
        {
            strncpy(name, file->name, name_length);
        }
    }

    return name;
}

bool file_set_owner_id(file_t *file, const uint8_t owner_id)
{
    bool result = false;

    if (file)
    {
        file->owner_id = owner_id;
        _file_set_modified(file);
        result = true;
    }

    return result;
}

uint8_t file_get_owner_id(const file_t *file)
{
    uint8_t owner_id = UINT8_MAX;

    if (file)
    {
        owner_id = file->owner_id;
    }

    return owner_id;
}

bool file_set_group_id(file_t *file, const uint8_t group_id)
{
    bool result = false;

    if (file)
    {
        file->group_id = group_id;
        _file_set_modified(file);
        result = true;
    }

    return result;
}

uint8_t file_get_group_id(const file_t *file)
{
    uint8_t group_id = UINT8_MAX;

    if (file)
    {
        group_id = file->group_id;
    }

    return group_id;
}

bool file_set_permissions(file_t *file, const uint16_t permissions)
{
    bool result = false;

    if (file)
    {
        file->permissions = permissions;
        _file_set_modified(file);
        result = true;
    }

    return result;
}

uint16_t file_get_permissions(const file_t *file)
{
    uint16_t permissions = UINT16_MAX;

    if (file)
    {
        permissions = file->permissions;
    }

    return permissions;
}

void *file_set_contents(file_t *file, const uint32_t size, void *contents)
{
    void *previous_contents = NULL;

    if (file && contents && !file->is_directory)
    {
        previous_contents = file->contents;
        file->contents = contents;
        file->size = size;
        _file_set_modified(file);
    }

    return previous_contents;
}

void *file_get_contents(const file_t *file)
{
    void *contents = NULL;

    if (file)
    {
        contents = file->contents;
    }

    return contents;
}

bool file_can(const file_t *file, const uint16_t permissions)
{
    bool result = false;

    if (file)
    {
        result = ((file->permissions & permissions) == permissions);
    }

    return result;
}

bool _file_set_modified(file_t *file)
{
    bool result = false;

    if (file)
    {
        file->modified_on = time(NULL);
        result = true;
    }

    return result;
}

file_t *file_copy(const file_t *file)
{
    file_t *new_file = NULL;
    char *name = NULL;
    void *contents = NULL;
    size_t name_length = 0;

    if (file)
    {
        name_length = strlen(file->name) + 1; // +1 for '\0'

        if ((name = malloc(sizeof(char) * name_length)))
        {
            strncpy(name, file->name, name_length);

            if (!file->is_directory)
            {
                if ((contents = malloc(file->size)))
                {
                    memcpy(contents, file->contents, file->size);

                    if (!(new_file = file_new(name, file->owner_id,
                                              file->group_id, file->permissions,
                                              file->size, file->is_directory,
                                              contents)))
                    {
                        free(contents);
                        free(name);
                    }
                }
                else
                {
                    free(name);
                }
            }
            else
            {
                if (!(new_file = file_new(name, file->owner_id, file->group_id,
                                          file->permissions, file->size,
                                          file->is_directory, contents)))
                {
                    free(name);
                }
            }
        }
    }

    return new_file;
}

bool _file_list_get_by_name_helper(void *file, void *ctx)
{
    return (strcmp((char *)ctx, ((file_t *)file)->name) == 0);
}

file_t *file_list_get_by_name(const list_t *list, const char *name)
{
    file_t *file = NULL;

    if (list && name)
    {
        file = (file_t *)list_find(list, (void *)name,
                                   _file_list_get_by_name_helper);
    }

    return file;
}

bool _file_list_get_by_owner_id_helper(void *file, void *id)
{
    return (*(uint8_t *)id == ((file_t *)file)->owner_id);
}

list_t *file_list_get_by_owner_id(const list_t *list, uint8_t owner_id)
{
    return list_find_all(list, &owner_id, _file_list_get_by_owner_id_helper);
}

bool _file_list_get_by_group_id_helper(void *file, void *id)
{
    return (*(uint8_t *)id == ((file_t *)file)->group_id);
}

list_t *file_list_get_by_group_id(const list_t *list, uint8_t group_id)
{
    return list_find_all(list, &group_id, _file_list_get_by_group_id_helper);
}

bool _file_list_get_directories_helper(void *file, void *ctx)
{
    return ((file_t *)file)->is_directory;
}

list_t *file_list_get_directories(const list_t *list)
{
    return list_find_all(list, NULL, _file_list_get_directories_helper);
}

bool _file_list_get_files_helper(void *file, void *ctx)
{
    return !((file_t *)file)->is_directory;
}

list_t *file_list_get_files(const list_t *list)
{
    return list_find_all(list, NULL, _file_list_get_files_helper);
}

void _file_list_free_helper(_list_node_t *node, void *ctx)
{
    file_t *file = (file_t *)node->value;
    bool *result = (bool *)ctx;

    *result &= file_free(file);
}

bool file_list_free(list_t *list)
{
    bool result = false;

    if (list)
    {
        result = true;
        list_traverse(list, &result, _file_list_free_helper);
        list_free(list);
        list = NULL;
    }

    return result;
}
