#ifndef MSG_MAX_LENGTH
#define MSG_MAX_LENGTH 81
#endif

#ifndef LIST_DIR_BUFFER_SIZE
#define LIST_DIR_BUFFER_SIZE 40
#endif

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "libfilesystem.h"

/**
 * @brief Moves a string from the stack to the heap and returns a
 * pointer to the string.
 *
 * This avoids the need to manually allocate memory when initializing
 * objects.
 * @param str The NULL-terminated string to move to the heap
 * @return A pointer to the new string, or NULL if something failed.
 */
char *_str_to_heap(const char *str);

/**
 * @brief Creates the root user and adds it to the filesystem
 * @return The list of users
 */
list_t *_init_users();

/**
 * @brief Creates the admin group and adds the root user to it
 * @param root_usr The root user
 * @return The list of groups
 */
list_t *_init_groups(user_t *root_usr);

/**
 * @brief Initializes the root directory
 * @return A pointer to the root directory
 */
file_t *_init_root_dir(void);

/**
 * @brief Initializes the message object
 * @return A pointer to the message object
 */
fs_msg_t *_init_last_message(void);

/**
 * @brief Frees the memory allocated to the last_message object
 * @param msg A pointer to the last_message object
 * @return True or false, depending on the result of the operation
 */
bool _free_last_message(fs_msg_t *msg);

/**
 * @brief Writes a message to the last_message object
 * @param fs A pointer to the filesystem
 * @param code The message code
 * @param message The message (max. MSG_MAX_LENGTH - 1 characters)
 * @return (void)
 */
void _write_message(filesystem_t *fs, uint8_t code, char *message);

char *_str_to_heap(const char *str)
{
    char *heap_str = NULL;
    const size_t length = strlen(str) + 1; // +1 for '\0'

    if (length > 0 && (heap_str = malloc(sizeof(char) * length)))
    {
        strncpy(heap_str, str, length);
    }

    return heap_str;
}

list_t *_init_users()
{
    user_t *root_usr = NULL;
    list_t *users = NULL;

    /* The addresses MUST be different, even though the contents are
    the same */
    char *username = _str_to_heap("root"),
         *password = _str_to_heap("root");

    if ((root_usr = user_new(0, username, password)))
    {
        if ((users = list_new(UINT8_MAX)))
        {
            if (!list_append(users, (void *)root_usr))
            {
                list_free(users);
                free(users);
                users = NULL;
                user_free(root_usr);
                root_usr = NULL;
            }
        }
        else
        {
            user_free(root_usr);
            root_usr = NULL;
        }
    }
    else
    {
        free(username);
        username = NULL;
        free(password);
        password = NULL;
    }

    return users;
}

list_t *_init_groups(user_t *root_usr)
{
    group_t *root_group = NULL;
    list_t *groups = NULL;
    char *group_name = _str_to_heap("root");

    if ((root_group = group_new(0, group_name)))
    {
        group_add_user(root_group, root_usr);

        if ((groups = list_new(UINT8_MAX)))
        {
            if (!list_append(groups, (void *)root_group))
            {
                group_free(root_group);
                root_group = NULL;
                list_free(groups);
                free(groups);
                groups = NULL;
            }
        }
        else
        {
            group_free(root_group);
            root_group = NULL;
        }
    }

    return groups;
}

file_t *_init_root_dir()
{
    file_t *root_dir = NULL;
    char *name = _str_to_heap("");
    list_t *root_contents = NULL;

    if ((root_contents = list_new(UINT32_MAX)))
    {
        if ((root_dir = file_new(name, 0, 0, USER_ALL + GROUP_ALL, 0, true,
                                 (void *)root_contents)) == NULL)
        {
            free(name);
            name = NULL;
            list_free(root_contents);
            root_contents = NULL;
        }
    }
    else
    {
        free(name);
        name = NULL;
    }

    return root_dir;
}

fs_msg_t *_init_last_message(void)
{
    fs_msg_t *msg = NULL;

    if ((msg = malloc(sizeof(fs_msg_t))))
    {
        msg->code = 0;

        if ((msg->message = malloc(sizeof(char) * MSG_MAX_LENGTH)))
        {
            strncpy(msg->message, "", MSG_MAX_LENGTH);
        }
        else
        {
            free(msg);
            msg = NULL;
        }
    }

    return msg;
}

bool _free_last_message(fs_msg_t *msg)
{
    bool result = false;

    if (msg)
    {
        free(msg->message);
        msg->message = NULL;
        free(msg);
        msg = NULL;
        result = true;
    }

    return result;
}

void _write_message(filesystem_t *fs, uint8_t code, char *message)
{
    if (fs && message)
    {
        fs->last_message->code = code;
        strncpy(fs->last_message->message, message, MSG_MAX_LENGTH);
    }
}

filesystem_t *fs_new()
{
    filesystem_t *new_fs = NULL;
    list_t *users = NULL;
    list_t *groups = NULL;
    file_t *root_dir = NULL;
    fs_msg_t *last_message = NULL;

    new_fs = malloc(sizeof(filesystem_t));
    users = _init_users();
    groups = _init_groups(list_get(users, 0));
    root_dir = _init_root_dir();
    last_message = _init_last_message();

    if (new_fs && users && groups && root_dir && last_message)
    {
        new_fs->current_group = (group_t *)list_get(groups, 0);
        new_fs->current_user = (user_t *)list_get(users, 0);
        new_fs->cwd = root_dir;
        new_fs->groups = groups;
        new_fs->last_group_id = 0;
        new_fs->last_message = last_message;
        new_fs->last_user_id = 0;
        new_fs->root_dir = root_dir;
        new_fs->users = users;
    }
    else
    {
        free(new_fs);
        new_fs = NULL;
        user_list_free(users);
        users = NULL;
        groups_list_free(groups);
        free(groups);
        groups = NULL;
        file_free(root_dir);
        root_dir = NULL;
        _free_last_message(last_message);
    }

    return new_fs;
}

bool fs_free(filesystem_t *fs)
{
    bool result = true;

    if (fs)
    {
        result &= user_list_free(fs->users) &&
                  groups_list_free(fs->groups) &&
                  file_free(fs->root_dir) &&
                  _free_last_message(fs->last_message);

        if (result)
        {
            fs->users = NULL;
            fs->groups = NULL;
            fs->root_dir = NULL;
            fs->last_message = NULL;
            free(fs);
            fs = NULL;
        }
    }

    return result;
}

bool fs_add_user(filesystem_t *fs, char *username, char *password)
{
    bool result = false;
    user_t *new_user = NULL;

    if (fs && username && password)
    {
        if (!fs_user_exists(fs, username))
        {
            if ((new_user = user_new(fs->last_user_id + 1, username, password)))
            {
                if (list_append(fs->users, (void *)new_user))
                {
                    result = true;
                    fs->last_user_id++;
                }
                else
                {
                    _write_message(fs, 4, "Could not add user to list");
                    user_free(new_user);
                    new_user = NULL;
                }
            }
            else
            {
                _write_message(fs, 3, "Could not create new user");
            }
        }
        else
        {
            _write_message(fs, 2, "User already exists");
        }
    }
    else
    {
        _write_message(fs, 1, "Invalid parameters");
    }

    return result;
}

bool fs_user_exists(filesystem_t *fs, char *name)
{
    return (user_list_get_by_name(fs->users, name) != NULL);
}

void _fs_print_users_helper(_list_node_t *node, void *ctx)
{
    fprintf((FILE *)ctx, "%s (%i)%s", ((user_t *)node->value)->name,
            ((user_t *)node->value)->user_id,
            node->next ? ", " : "\n");
}

void fs_print_users(filesystem_t *fs, FILE *fp)
{
    if (fp)
    {
        list_traverse(fs->users, (void *)fp, _fs_print_users_helper);
    }
}

bool fs_remove_user(filesystem_t *fs, char *name)
{
    bool result = false;
    user_t *user = NULL;

    if (fs && name)
    {
        if (fs_current_user_is_admin(fs))
        {
            user = user_list_get_by_name(fs->users, name);

            if (user)
            {
                /* Remove user from every group it may belong to */
                group_list_remove_member(fs->groups, user);

                /* Remove the user from the list of users, and free */
                list_remove(fs->users, user, POINTERS_MATCH, NULLF);

                /* Free the memory allocated to the user */
                user_free(user);
                user = NULL;
                result = true;
            }
            else
            {
                _write_message(fs, 3, "User not found");
            }
        }
        else
        {
            _write_message(fs, 2, "User is not in root group");
        }
    }
    else
    {
        _write_message(fs, 1, "Invalid parameters");
    }

    return result;
}

bool fs_user_is_admin(filesystem_t *fs, user_t *user)
{
    bool result = false;
    group_t *admin_group = NULL;

    if (fs)
    {
        admin_group = (group_t *)list_get(fs->groups, 0);
        result = user_in_group(admin_group, user);
    }

    return result;
}

bool fs_current_user_is_admin(filesystem_t *fs)
{
    bool result = false;

    if (fs)
    {
        result = fs_user_is_admin(fs, fs->current_user);
    }

    return result;
}

bool fs_group_exists(filesystem_t *fs, char *name)
{
    bool result = false;

    if (fs && name)
    {
        result = (group_list_get_by_name(fs->groups, name) != NULL);
    }

    return result;
}

bool fs_add_group(filesystem_t *fs, char *name)
{
    bool result = false;
    group_t *group = NULL;

    if (fs && name)
    {
        if (!fs_group_exists(fs, name))
        {
            group = group_new(fs->last_group_id + 1, name);

            if (group)
            {
                if (list_append(fs->groups, (void *)group))
                {
                    fs->last_group_id++;
                    result = true;
                }
                else
                {
                    _write_message(fs, 4, "Could not add group to list");
                    group_free(group);
                }
            }
            else
            {
                _write_message(fs, 3, "Could not create group");
            }
        }
        else
        {
            _write_message(fs, 2, "Group already exists");
        }
    }
    else
    {
        _write_message(fs, 1, "Invalid parameters");
    }

    return result;
}

void _fs_print_groups_helper(_list_node_t *node, void *ctx)
{
    const group_t *group = ((group_t *)node->value);
    fprintf((FILE *)ctx, "%s: ", group->name);
    list_traverse(group->users, ctx, _fs_print_users_helper);
    fprintf((FILE *)ctx, "\n");
}

void fs_print_groups(filesystem_t *fs, FILE *fp)
{
    if (fs && fp)
    {
        list_traverse(fs->groups, fp, _fs_print_groups_helper);
    }
}

bool fs_add_user_to_group(filesystem_t *fs, user_t *user, group_t *group)
{
    bool result = false;

    if (fs && user && group)
    {
        if (fs_current_user_is_admin(fs))
        {
            if (!user_in_group(group, user))
            {
                if (group_add_user(group, user))
                {
                    result = true;
                }
                else
                {
                    _write_message(fs, 3, "Could not add user to group");
                }
            }
            else
            {
                _write_message(fs, 2, "User already belongs to group");
            }
        }
        else
        {
            _write_message(fs, 4, "Not enough permissions");
        }
    }
    else
    {
        _write_message(fs, 1, "Invalid parameters");
    }

    return result;
}

bool fs_login(filesystem_t *fs, char *username, char *password)
{
    bool result = false;
    user_t *user = NULL;

    if (fs && username && password)
    {
        user = user_list_get_by_name(fs->users, username);

        if (user)
        {
            if (strcmp(user->password, password) == 0)
            {
                fs->current_user = user;
                result = true;
            }
            else
            {
                _write_message(fs, 3, "Invalid password");
            }
        }
        else
        {
            _write_message(fs, 2, "User does not exist");
        }
    }
    else
    {
        _write_message(fs, 1, "Invalid parameters");
    }

    return result;
}

bool fs_current_user_can_modify(filesystem_t *fs, file_t *file)
{
    return fs_user_can(fs, file, fs->current_user, USER_WRITE);
}

bool fs_create_file(filesystem_t *fs, char *name, const uint16_t permissions,
                    const uint32_t size, void *contents, file_t *dir)
{
    bool result = false;
    file_t *file = NULL;

    if (fs && name && contents && dir && dir->is_directory &&
        fs->current_user && fs->current_group)
    {
        if (fs_current_user_can_modify(fs, dir))
        {
            file = file_new(name,
                            fs->current_user->user_id,
                            fs->current_group->group_id,
                            permissions, size, false, contents);

            if (file)
            {
                if (list_append((list_t *)dir->contents, file))
                {
                    result = true;
                }
                else
                {
                    _write_message(fs, 4, "Could not add file to directory");
                    file_free(file);
                    free(file);
                    file = NULL;
                }
            }
            else
            {
                _write_message(fs, 3, "Could not create file");
            }
        }
        else
        {
            _write_message(fs, 2, "Current user cannot modify this directory");
        }
    }
    else
    {
        _write_message(fs, 1, "Invalid parameters");
    }

    return result;
}

bool fs_current_user_can_read(filesystem_t *fs, file_t *file)
{
    return fs_user_can(fs, file, fs->current_user, USER_READ);
}

bool fs_current_user_can_execute(filesystem_t *fs, file_t *file)
{
    return fs_user_can(fs, file, fs->current_user, USER_EXEC);
}

void _fs_list_dir_helper(_list_node_t *node, void *ctx)
{
    file_t *file = (file_t *)node->value;
    char str[LIST_DIR_BUFFER_SIZE] = "\0";

    strftime(str, 40, "%Y-%m-%dT%H:%M:%S-03:00",
             localtime(&(file->modified_on)));

    printf("%4" PRIu16 " %3" PRIu8 " %3" PRIu8 " %10" PRIu32 " %s %s%s\n",
           file->permissions,
           file->owner_id,
           file->group_id,
           file->size,
           str,
           file->name,
           file->is_directory ? " (d)" : "");
}

void fs_list_dir(filesystem_t *fs, file_t *dir)
{
    list_t *list = NULL;
    if (dir)
    {
        if (dir->is_directory)
        {
            if (fs_current_user_can_read(fs, dir))
            {
                list = (list_t *)dir->contents;

                if (list_get_length(list) > 0)
                {
                    list_traverse(list, NULL, _fs_list_dir_helper);
                }
                else
                {
                    printf("Directory is empty\n");
                }
            }
            else
            {
                printf("Access denied\n");
            }
        }
        else
        {
            printf("Not a directory\n");
        }
    }
}

bool fs_create_dir(filesystem_t *fs, char *name, const uint16_t permissions,
                   file_t *dir)
{
    bool result = false;
    file_t *file = NULL;
    list_t *contents = NULL;

    if (fs && name && dir && dir->is_directory && fs->current_user &&
        fs->current_group)
    {
        if (fs_current_user_can_modify(fs, dir))
        {
            contents = list_new(UINT8_MAX);

            if (contents)
            {
                file = file_new(name,
                                fs->current_user->user_id,
                                fs->current_group->group_id,
                                permissions, 0, true, contents);

                if (file)
                {
                    if (list_append((list_t *)dir->contents, file))
                    {
                        result = true;
                    }
                    else
                    {
                        _write_message(fs, 4, "Could not add file to directory");
                        file_free(file);
                        free(file);
                        file = NULL;
                    }
                }
                else
                {
                    _write_message(fs, 5, "Could not create file object");
                    list_free(contents);
                    free(contents);
                    contents = NULL;
                }
            }
            else
            {
                _write_message(fs, 3, "Could not create directory");
            }
        }
        else
        {
            _write_message(fs, 2, "Current user cannot modify this directory");
        }
    }
    else
    {
        _write_message(fs, 1, "Invalid parameters");
    }

    return result;
}

bool fs_change_user_password(filesystem_t *fs, user_t *user, char *password)
{
    bool result = false;

    if (fs && user && password)
    {
        if (fs->current_user == user || fs_current_user_is_admin(fs))
        {
            result = user_set_password(user, password);

            if (!result)
            {
                _write_message(fs, 3, "Could not set password");
            }
        }
        else
        {
            _write_message(fs, 2, "Not enough permissions");
        }
    }
    else
    {
        _write_message(fs, 1, "Invalid parameters");
    }

    return result;
}

bool fs_remove_user_from_group(filesystem_t *fs, user_t *user, group_t *group)
{
    bool result = false;

    if (fs && user && group)
    {
        if (fs_current_user_is_admin(fs))
        {
            result = group_remove_user(group, user);

            if (!result)
            {
                _write_message(fs, 3, "User does not belong to group");
            }
        }
        else
        {
            _write_message(fs, 2, "Not enough permissions");
        }
    }
    else
    {
        _write_message(fs, 1, "Invalid parameters");
    }

    return result;
}

bool fs_change_directory(filesystem_t *fs, file_t *dir)
{
    bool result = false;

    if (fs && dir && dir->is_directory)
    {
        if (fs_current_user_can_execute(fs, dir))
        {
            fs->cwd = dir;
            result = true;
        }
        else
        {
            _write_message(fs, 2, "Access denied");
        }
    }
    else
    {
        _write_message(fs, 1, "Invalid parameters");
    }

    return result;
}

bool fs_copy_file(filesystem_t *fs, file_t *source, file_t *dest_dir)
{
    bool result = false;
    file_t *new_file = NULL;

    if (fs && source && dest_dir && !source->is_directory &&
        dest_dir->is_directory)
    {
        if (fs_current_user_can_read(fs, source) && fs_current_user_can_modify(fs, dest_dir))
        {
            new_file = file_copy(source);

            if (new_file)
            {
                new_file->owner_id = fs->current_user->user_id;
                new_file->group_id = fs->current_group->group_id;
                new_file->permissions = USER_ALL + GROUP_READ + GROUP_EXEC +
                                        OTHERS_READ + OTHERS_EXEC;

                if (list_append((list_t *)dest_dir->contents, new_file))
                {
                    result = true;
                }
                else
                {
                    _write_message(fs, 4, "Could not add file to directory");
                    file_free(new_file);
                    free(new_file);
                    new_file = NULL;
                }
            }
            else
            {
                _write_message(fs, 3, "Could not create new file");
            }
        }
        else
        {
            _write_message(fs, 2, "Access denied");
        }
    }
    else
    {
        _write_message(fs, 1, "Invalid parameters");
    }

    return result;
}

bool fs_remove_file(filesystem_t *fs, file_t *parent_dir, file_t *file)
{
    bool result = false;

    if (fs && parent_dir && file && parent_dir->is_directory &&
        !file->is_directory)
    {
        if (fs_current_user_can_modify(fs, parent_dir) && fs_current_user_can_modify(fs, file))
        {
            result = list_remove((list_t *)parent_dir->contents,
                                 (void *)file, POINTERS_MATCH, NULLF);

            if (!result)
            {
                _write_message(fs, 3, "Could not remove file from directory");
            }
        }
        else
        {
            _write_message(fs, 2, "Access denied");
        }
    }
    else
    {
        _write_message(fs, 1, "Invalid parameters");
    }

    return result;
}

bool fs_move_file(filesystem_t *fs, file_t *source_dir, file_t *dest_dir,
                  file_t *source)
{
    bool result = false;

    if (fs && source_dir && dest_dir && source && source_dir->is_directory &&
        dest_dir->is_directory && !source->is_directory)
    {
        if (fs_current_user_can_modify(fs, source_dir) &&
            fs_current_user_can_modify(fs, dest_dir))
        {
            result = fs_remove_file(fs, source_dir, source);

            if (result)
            {
                result = list_append((list_t *)dest_dir->contents,
                                     (void *)source);

                if (!result)
                {
                    /* Attempt to put the file back in its original location */
                    list_append((list_t *)source_dir->contents, (void *)source);
                    _write_message(fs, 3, "Could not add file to directory");
                }
            }
        }
        else
        {
            _write_message(fs, 2, "Access denied");
        }
    }
    else
    {
        _write_message(fs, 1, "Invalid parameters");
    }

    return result;
}

bool fs_file_change_permissions(filesystem_t *fs, file_t *file,
                                uint16_t permissions)
{
    bool result = false;

    if (fs && file)
    {
        if (fs_current_user_can_modify(fs, file))
        {
            result = file_set_permissions(file, permissions);
        }
        else
        {
            _write_message(fs, 2, "Access denied");
        }
    }
    else
    {
        _write_message(fs, 1, "Invalid parameters");
    }

    return result;
}

bool fs_dir_change_permissions(filesystem_t *fs, file_t *dir,
                               uint16_t permissions)
{
    return fs_file_change_permissions(fs, dir, permissions);
}

bool fs_modify_file(filesystem_t *fs, file_t *file, void *new_contents,
                    uint32_t new_size)
{
    bool result = false;
    void *previous_contents = NULL;

    if (fs && file && new_contents && !file->is_directory)
    {
        if (fs_current_user_can_modify(fs, file))
        {
            previous_contents = file_set_contents(file, new_size, new_contents);

            if (previous_contents)
            {
                free(previous_contents);
                previous_contents = NULL;
                result = true;
            }
            else
            {
                _write_message(fs, 3, "Unable to modify file");
            }
        }
        else
        {
            _write_message(fs, 2, "Access denied");
        }
    }
    else
    {
        _write_message(fs, 1, "Invalid parameters");
    }

    return result;
}

void _fs_traverse_helper(_list_node_t *node, void *ctx)
{
    void **ctxe = (void **)ctx;
    file_t *file = (file_t *)node->value;
    void (*callback)(_list_node_t *, void *) = ctxe[3];

    /* Call the function for the current file */
    callback(node, ctxe[2]);

    /* If recursive was specified */
    if (*(bool *)ctxe[1] && file->is_directory)
    {
        fs_traverse((filesystem_t *)ctxe[0], file, *(bool *)ctxe[1], ctxe[2],
                    callback);
    }
}

void fs_traverse(filesystem_t *fs, file_t *start_dir, bool recursive, void *ctx,
                 void callback(_list_node_t *node, void *ctx))
{
    void *ctxe[] = {fs, &recursive, ctx, callback};

    if (fs && start_dir && callback && start_dir->is_directory)
    {
        /* Call the function for every file in the start directory */
        list_traverse((list_t *)start_dir->contents, ctxe, _fs_traverse_helper);
    }
}

void _fs_list_dir_recursive_helper(_list_node_t *node, void *ctx)
{
    void **ctxe = (void **)ctx;
    char *str = (char *)ctxe[0];
    uint32_t *len = (uint32_t *)ctxe[1];
    uint32_t *depth = (uint32_t *)ctxe[2];
    file_t *file = (file_t *)node->value;

    printf("%s%s\n", str, file->name);

    if (file->is_directory && list_get_length((list_t *)file->contents) > 0)
    {
        if (*len >= 2)
        {
            str[*len - 2] = '\0';
            strcat(str, "  ");
        }

        strcat(str, "\t|--");
        *len += 4;
        *depth += 1;
    }
    else if (!node->next && *len >= 4 * *depth)
    {
        str[*len - (4 * *depth--)] = '\0';
    }
}

void fs_list_directory_recursive(filesystem_t *fs, file_t *start_dir)
{
    char *prefix = calloc(1024, sizeof(char));
    uint32_t len = 0;
    uint32_t depth = 0;
    void *ctxe[] = {prefix, &len, &depth};

    if (fs && start_dir && start_dir->is_directory)
    {
        fs_traverse(fs, start_dir, true, ctxe, _fs_list_dir_recursive_helper);
    }

    free(prefix);
}

void _fs_find_all_helper(_list_node_t *node, void *ctx)
{
    void **ctxe = (void **)ctx;
    bool (*cmp)(file_t *, void *) = ctxe[1];

    if (cmp((file_t *)node->value, ctxe[2]))
    {
        list_append((list_t *)ctxe[0], node->value);
    }
}

list_t *fs_find_all(filesystem_t *fs, file_t *start_dir, void *ctx,
                    bool recursive, bool cmp(file_t *, void *ctx))
{
    list_t *list = list_new(UINT32_MAX);
    void *ctxe[] = {list, cmp, ctx};

    if (fs && start_dir && cmp && start_dir->is_directory && list)
    {
        fs_traverse(fs, start_dir, recursive, ctxe, _fs_find_all_helper);
    }

    return list;
}

bool fs_user_can(filesystem_t *fs, file_t *file, user_t *user, uint16_t action)
{
    bool result = false;
    group_t *group = NULL;

    if (fs && file && user &&
        (action >= USER_EXEC) &&
        (action <= USER_WRITE + USER_READ + USER_EXEC))
    {
        group = group_list_get_by_id(fs->groups, file->group_id);
        result = ((file->owner_id == user->user_id && file_can(file, action)) ||
                  (user_in_group(group, user) && file_can(file, action >> 4)) ||
                  (file_can(file, action >> 8)));
    }

    return result;
}
