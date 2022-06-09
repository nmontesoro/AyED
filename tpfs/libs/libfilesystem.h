#ifndef LIB_FILESYSTEM
#define LIB_FILESYSTEM

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "libfiles.h"
#include "libgroups.h"
#include "liblists.h"
#include "libpermissions.h"
#include "libusers.h"

typedef struct _fs_msg_t
{
    uint8_t code;
    char *message;
} fs_msg_t;

typedef struct _filesystem_t
{
    list_t *users;
    list_t *groups;
    file_t *root_dir;
    user_t *current_user;
    group_t *current_group;
    file_t *cwd;
    fs_msg_t *last_message;
    uint8_t last_user_id;
    uint8_t last_group_id;
} filesystem_t;

/**
 * @brief Creates a new filesystem
 * @return A pointer to the new filesystem, or NULL if something failed
 */
filesystem_t *fs_new(void);

/**
 * @brief Frees the memory taken up by a filesystem
 * @param fs A pointer to the filesystem
 * @return True or false, depending on the result of the operation
 */
bool fs_free(filesystem_t *fs);

/**
 * @brief Creates a new user and adds it to the filesystem. Checks for
 * duplicate user names
 * @param fs A pointer to the filesystem
 * @param username The username
 * @param password The password, which _can_ be blank
 * @return True or false, depending on the result of the operation
 */
bool fs_add_user(filesystem_t *fs, char *username, char *password);

/**
 * @brief Removes a user from the filesystem and from every group
 * within it
 * @param fs A pointer to the filesystem
 * @param name The name of the user to be removed
 * @return True or false, depending on the result of the operation
 */
bool fs_remove_user(filesystem_t *fs, char *name);

/**
 * @brief Creates a new group and adds it to the filesystem
 * @param fs A pointer to the filesystem
 * @param name The name of the new group
 * @return True or false, depending on the result of the operation
 */
bool fs_add_group(filesystem_t *fs, char *name);

/**
 * @brief Removes a group from the filesystem
 * @param fs A pointer to the filesystem
 * @param name The name of the group to be removed
 * @return True or false, depending on the result of the operation
 */
bool fs_remove_group(filesystem_t *fs, char *name);

/**
 * @brief Checks if a user exists
 * @param fs A pointer to the filesystem
 * @param name The name of the user
 * @return True or false, depending on the result of the operation
 */
bool fs_user_exists(filesystem_t *fs, char *name);

/**
 * @brief Checks if a group exists
 * @param fs A pointer to the filesystem
 * @param name The name of the group
 * @return True or false, depending on the result of the operation
 */
bool fs_group_exists(filesystem_t *fs, char *name);

/**
 * @brief Gets the last error code
 * @param fs The filesystem
 * @return A number indicating the last error ode
 */
uint8_t fs_get_last_error_code(filesystem_t *fs);

/**
 * @brief Gets the last error message
 * @param fs The filesystem
 * @return A string containing the last error message
 */
char *fs_get_last_error_message(filesystem_t *fs);

/**
 * @brief Checks if a user belongs to the root group
 * @param fs A pointer to the filesystem
 * @param user A pointer to the user
 * @return True or false, depending on the result of the operation
 */
bool fs_user_is_admin(filesystem_t *fs, user_t *user);

/**
 * @brief If the current user is admin, then this function adds any user
 * to a group it doesn't already belong to
 * @param fs A pointer to the filesystem
 * @param user A pointer to the user
 * @param group A pointer to the group
 * @return True or false, depending on the result of the operation
 */
bool fs_add_user_to_group(filesystem_t *fs, user_t *user, group_t *group);

/**
 * @brief Logs a user in
 * @param fs A pointer to the filesystem
 * @param username A string contanining the username
 * @param password A string containing the password
 * @return True or false, depending on the result of the operation
 */
bool fs_login(filesystem_t *fs, char *username, char *password);

/**
 * @brief Checks if the current user has enough permissions to modify a
 * file or directory
 * @param fs A pointer to the filesystem
 * @param file The file to be modified
 * @return True or false, depending on the result of the operation
 */
bool fs_user_can_modify(filesystem_t *fs, file_t *file);

/**
 * @brief Checks that all necessary conditions are met, and then creates
 * a file in the given directory
 * @param fs A pointer to the filesystem
 * @param name The name of the file
 * @param permissions The permissions of the file (refer to
 * libpermissions.h)
 * @param size The size of the file, in bytes
 * @param contents The contents of the file (cannot be NULL)
 * @param dir The directory in which the file will be stored
 * @return True or false, depending on the result of the operation
 */
bool fs_create_file(filesystem_t *fs, char *name, const uint16_t permissions,
                    const uint32_t size, void *contents, file_t *dir);

/**
 * @brief Prints a list of users to the specified file
 * @param fs A pointer to the filesystem
 * @param fp A pointer to the file
 * @return (void)
 */
void fs_print_users(filesystem_t *fs, FILE *fp);

/**
 * @brief Prints a list of groups (with their corresponding users) to the
 * specified file
 * @param fs A pointer to the filesystem
 * @param fp A pointer to the file
 * @return (void)
 */
void fs_print_groups(filesystem_t *fs, FILE *fp);

/**
 * @brief Checks if the current user belongs to the admin group
 * @param fs A pointer to the filesystem
 * @return True or false, depending on the result of the operation
 */
bool fs_current_user_is_admin(filesystem_t *fs);

/**
 * @brief Prints the contents of a directory to stdout (only if current
 * user has read permissions)
 * @param fs A pointer to the filesystem
 * @param dir The directory
 * @return (void)
 */
void fs_list_dir(filesystem_t *fs, file_t *dir);

/**
 * @brief Checks if a user can read a file or directory
 * @param fs A pointer to the filesystem
 * @param file A pointer to the file
 * @return True or false, depending on the result of the operation
 */
bool fs_user_can_read(filesystem_t *fs, file_t *file);

/**
 * @brief Checks if a user can execute a file or list a directory
 * @param fs A pointer to the filesystem
 * @param file A pointer to the file
 * @return True or false, depending on the result of the operation
 */
bool fs_user_can_execute(filesystem_t *fs, file_t *file);

/**
 * @brief Checks that all necessary conditions are met, and then creates
 * a new directory inside the given directory
 * @param fs A pointer to the filesystem
 * @param name The name of the directory
 * @param permissions The permissions of the directory (refer to
 * libpermissions.h)
 * @param dir The directory in which the new directory will be stored
 * @return True or false, depending on the result of the operation
 */
bool fs_create_dir(filesystem_t *fs, char *name, const uint16_t permissions,
                   file_t *dir);

/**
 * @brief Checks that all necessary conditions are met and then changes
 * a user's password
 * @param fs A pointer to the filesystem
 * @param user A pointer to the user whose password is to be changed
 * @param password A null-terminated string containing the new password
 * @return True or false, depending on the result of the operation
 */
bool fs_change_user_password(filesystem_t *fs, user_t *user, char *password);

/**
 * @brief If current user is admin, this function removes a user from a
 * group
 * @param fs A pointer to the filesystem
 * @param user A pointer to the user
 * @param group A pointer to the group
 * @return True or false, depending on the result of the operation
 */
bool fs_remove_user_from_group(filesystem_t *fs, user_t *user, group_t *group);

/**
 * @brief If the current user has execute permissions, change the
 * current working directory
 * @param fs A pointer to the filesystem
 * @param dir A pointer to the directory
 * @return True or false, depending on the result of the operation
 */
bool fs_change_directory(filesystem_t *fs, file_t *dir);

/**
 * @brief Checks if conditions are met and then copies a file into a
 * directory
 * @param fs A pointer to the filesystem
 * @param source A pointer to the source file
 * @param dest_dir A pointer to the destination directory
 * @return True or false, depending on the result of the operation
 */
bool fs_copy_file(filesystem_t *fs, file_t *source, file_t *dest_dir);

/**
 * @brief If conditions are met, removes a file from the parent
 * directory. Does NOT free the file pointer.
 * @param fs A pointer to the filesystem
 * @param parent_dir A pointer to the parent directory
 * @param file A pointer to the file
 * @return True or false, depending on the result of the operation
 */
bool fs_remove_file(filesystem_t *fs, file_t *parent_dir, file_t *file);

/**
 * @brief Checks if conditions are met and then moves a file from
 * source_dir to dest_dir
 * @param fs A pointer to the filesystem
 * @param source_dir A pointer to the source directory
 * @param dest_dir A pointer to the destination directory
 * @param source A pointer to the source file
 * @return True or false, depending on the result of the operation
 */
bool fs_move_file(filesystem_t *fs, file_t *source_dir, file_t *dest_dir,
                  file_t *source);

/**
 * @brief If conditions are met, changes the permissions of a file
 * @param fs A pointer to the filesystem
 * @param file A pointer to the file
 * @param permissions An integer representing the new permissions (refer
 * to libpermissions.h)
 * @return True or false, depending on the result of the operation
 */
bool fs_file_change_permissions(filesystem_t *fs, file_t *file,
                                uint16_t permissions);

#endif