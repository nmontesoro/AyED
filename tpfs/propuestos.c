#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "libs/libfilesystem.h"

bool find_by_name(file_t *file, void *ctx)
{
    return (strcmp(file->name, (char *)ctx) == 0);
}

void ej1_helper(_list_node_t *node, void *ctx)
{
    void **ctxe = (void **)ctx;
    uint64_t *sum = (uint64_t *)ctxe[0];
    uint8_t *uid = (uint8_t *)ctxe[1];
    file_t *file = (file_t *)node->value;

    if (file->owner_id == *uid)
    {
        *sum += file->size;
    }
}

uint64_t ej1(filesystem_t *fs, char *username)
{
    uint64_t sum = 0;
    uint8_t uid = 0;
    user_t *user = NULL;
    void *ctxe[] = {&sum, &uid};

    if (fs && username)
    {
        if ((user = user_list_get_by_name(fs->users, username)))
        {
            uid = user->user_id;
            fs_traverse(fs, fs->cwd, true, ctxe, ej1_helper);
        }
    }

    return sum;
}

bool ej2_helper(file_t *file, void *ctx)
{
    return (strcmp(file->name, ctx) == 0);
}

void ej2_printer(_list_node_t *node, void *ctx)
{
    filesystem_t *fs = ctx;
    file_t *file = (file_t *)node->value;
    char *path = fs_get_full_path(fs, fs->root_dir, file);
    printf("%s\n", path);
    free(path);
}

void ej2(filesystem_t *fs, char *file_name)
{
    list_t *list = NULL;

    if (fs && file_name)
    {
        list = fs_find_all(fs, fs->root_dir, file_name, true, ej2_helper);
        list_traverse(list, fs, ej2_printer);
        list_free(list);
    }
}

void ej3_printer(_list_node_t *node, void *ctx)
{
    printf("%s%s", ((user_t *)node->value)->name, node->next ? ", " : "\n");
}

void ej3_helper(_list_node_t *node, void *ctx)
{
    void **ctxe = (void **)ctx;
    list_t *list = ctxe[0];
    filesystem_t *fs = ctxe[1];
    file_t *file = ctxe[2];
    user_t *user = (user_t *)node->value;

    /*
     * If the user can read, add it to the list.
     * They will not be repeated, as the user list is only iterated through
     * once.
     */

    if (fs_user_can(fs, file, user, USER_READ))
    {
        list_append(list, user);
    }
}

void ej3(filesystem_t *fs, file_t *file)
{
    if (fs && file)
    {
        list_t *list = list_new(fs->last_user_id + 1);
        void *ctxe[] = {list, fs, file};

        list_traverse(fs->users, ctxe, ej3_helper);
        list_traverse(list, NULL, ej3_printer);

        list_free(list);
    }
}

void ej4_helper(_list_node_t *node, void *ctx)
{
    void **ctxe = (void **)ctx;
    time_t *modified_on = ctxe[0];
    group_t *group = ctxe[1];
    uint64_t *size = ctxe[2];
    file_t *file = (file_t *)node->value;

    if (file->modified_on > *modified_on && file->group_id == group->group_id)
    {
        *size += file->size;
    }
}

void ej4(filesystem_t *fs, time_t modified_on, group_t *group)
{
    if (fs && group)
    {
        uint64_t size = 0;
        void *ctxe[] = {&modified_on, group, &size};

        fs_traverse(fs, fs->root_dir, true, ctxe, ej4_helper);
    }
}

void ej5_helper(_list_node_t *node, void *ctx)
{
    void **ctxe = (void **)ctx;
    file_t *result = ctxe[0],
           *file = (file_t *)node->value;
    user_t *user = ctxe[1];
    filesystem_t *fs = ctxe[2];

    if (!file->is_directory && fs_user_can(fs, file, user, USER_WRITE))
    {
        if (result == NULL)
        {
            result = file;
        }
        else
        {
            if (file->modified_on > result->modified_on)
            {
                result = file;
            }
        }
    }
}

file_t *ej5(filesystem_t *fs, char *username)
{
    file_t *result = NULL;
    user_t *user = NULL;

    if (fs && username)
    {
        user = user_list_get_by_name(fs->users, username);
        void *ctxe[] = {result, user, fs};

        if (user)
        {
            fs_traverse(fs, fs->root_dir, true, ctxe, ej5_helper);
        }
    }

    return result;
}

void ej6_helper(_list_node_t *node, void *ctx)
{
    void **ctxe = (void **)ctx;
    filesystem_t *fs = ctxe[0];
    user_t *user = ctxe[1];
    uint64_t *result = ctxe[2];
    file_t *file = (file_t *)node->value;

    if (!file->is_directory && fs_user_can(fs, file, user, USER_EXEC))
    {
        *result += 1;
    }
}

uint64_t ej6(filesystem_t *fs, user_t *user)
{
    uint64_t result = 0;

    if (fs && user)
    {
        void *ctxe[] = {fs, user, &result};
        fs_traverse(fs, fs->root_dir, true, ctxe, ej6_helper);
    }

    return result;
}

void ej7_helper(_list_node_t *node, void *ctx)
{
    void **ctxe = (void **)ctx;
    filesystem_t *fs = ctxe[0];
    file_t *file = (file_t *)node->value;
    user_t *owner = user_list_get_by_id(fs->users, file->owner_id);
    group_t *group = group_list_get_by_id(fs->groups, file->group_id);

    printf("%s%s(O: %s) (G: %s)\n", file->name,
           file->is_directory ? " (d) " : " ",
           owner->name,
           group->name);
}

void ej7(filesystem_t *fs, file_t *dir)
{
    if (fs && dir)
    {
        void *ctxe[] = {fs};
        fs_traverse(fs, dir, true, ctxe, ej7_helper);
    }
}

void ej8_helper(_list_node_t *node, void *ctx)
{
    void **ctxe = (void **)ctx;
    filesystem_t *fs = ctxe[0];
    file_t *file = ctxe[1];
    list_t *list = ctxe[2];
    user_t *user = (user_t *)node->value;

    if (fs_user_can(fs, file, user, USER_READ))
    {
        list_append(list, user->name);
    }
}

list_t *ej8(filesystem_t *fs, file_t *file)
{
    list_t *list = NULL;

    if (fs && file && list)
    {
        void *ctxe[] = {fs, file, list};
        list_traverse(fs->users, ctxe, ej8_helper);
    }

    return list;
}

bool ej9(filesystem_t *fs, file_t *file, file_t *source_dir, file_t *dest_dir,
         user_t *user)
{
    return (fs_user_can(fs, source_dir, user, USER_WRITE) && 
            fs_user_can(fs, dest_dir, user, USER_WRITE) &&
            fs_user_can(fs, file, user, USER_READ + USER_WRITE));
}

void ej10_helper(_list_node_t *node, void *ctx)
{
    void **ctxe = (void **)ctx;
    filesystem_t *fs = ctxe[0];
    file_t *file = ctxe[1];
    list_t *list = ctxe[2];
    user_t *user = (user_t *)node->value;

    if (!fs_user_can(fs, file, user, USER_WRITE))
    {
        list_append(list, user);
    }
}

list_t *ej10(filesystem_t *fs, file_t *file)
{
    list_t *list = list_new(fs->last_user_id + 1);

    if (fs && file)
    {
        void *ctxe[] = {fs, file, list};

        list_traverse(fs->users, ctxe, ej10_helper);
    }

    return list;
}

int main(int argc, char const *argv[])
{
    filesystem_t *fs = fs_new();

    char *f1 = calloc(20, sizeof(char)),
         *f2 = calloc(20, sizeof(char)),
         *f3 = calloc(20, sizeof(char)),
         *f4 = calloc(20, sizeof(char)),
         *f5 = calloc(20, sizeof(char)),
         *f6 = calloc(20, sizeof(char)),
         *f7 = calloc(20, sizeof(char)),
         *d1 = calloc(20, sizeof(char)),
         *d2 = calloc(20, sizeof(char)),
         *d3 = calloc(20, sizeof(char));

    int *c1 = calloc(1, sizeof(int)),
        *c2 = calloc(1, sizeof(int)),
        *c3 = calloc(1, sizeof(int)),
        *c4 = calloc(1, sizeof(int)),
        *c5 = calloc(1, sizeof(int)),
        *c6 = calloc(1, sizeof(int)),
        *c7 = calloc(1, sizeof(int));

    file_t *dir1 = NULL,
           *dir2 = NULL,
           *file1 = NULL;

    strncpy(f1, "file 1", 20);
    strncpy(f2, "file 2", 20);
    strncpy(f3, "file 3", 20);
    strncpy(f4, "file 4", 20);
    strncpy(f5, "file 5", 20);
    strncpy(f6, "file 6", 20);
    strncpy(f7, "file 7", 20);
    strncpy(d1, "directory 1", 20);
    strncpy(d2, "directory 2", 20);
    strncpy(d3, "directory 3", 20);

    fs_add_user(fs, "naza", "naza");

    fs_create_file(fs, f1, USER_ALL + OTHERS_READ, sizeof(int), (void *)c1, fs->cwd);
    fs_create_file(fs, f2, USER_ALL, sizeof(int), (void *)c2, fs->cwd);
    fs_create_dir(fs, d1, USER_ALL, fs->cwd);
    dir1 = file_list_get_by_name((list_t *)fs->cwd->contents, d1);
    fs_create_file(fs, f3, USER_ALL, sizeof(int), (void *)c3, dir1);
    fs_create_file(fs, f4, USER_ALL, sizeof(int), (void *)c4, dir1);
    fs_create_file(fs, f5, USER_ALL, sizeof(int), (void *)c5, dir1);
    fs_create_dir(fs, d2, USER_ALL, dir1);
    dir2 = file_list_get_by_name((list_t *)dir1->contents, d2);
    fs_create_file(fs, f6, USER_ALL, sizeof(int), (void *)c6, dir2);
    fs_create_dir(fs, d3, USER_ALL, fs->cwd);
    fs_create_file(fs, f7, USER_ALL, sizeof(int), (void *)c7, fs->cwd);

    file1 = file_list_get_by_name((list_t *)fs->cwd->contents, f1);

    printf("%" PRIu64 "\n", ej1(fs, "root"));
    // ej2(fs, f1);
    ej3(fs, file1);
    ej7(fs, fs->root_dir);

    return 0;
}
