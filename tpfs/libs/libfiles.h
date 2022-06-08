#ifndef LIB_FILES
#define LIB_FILES

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "libpermissions.h"
#include "liblists.h"

typedef struct _file_t
{
    char *name;
    uint8_t owner_id;
    uint8_t group_id;
    uint16_t permissions;
    uint32_t size;
    bool is_directory;
    time_t created_on;
    time_t modified_on;
    void *contents;
} file_t;

/**
 * @brief Creates a new file
 * @param name The name of the new file
 * @param owner_id The id of the owner of the file
 * @param group_id The id of the group the owner belongs to
 * @param permissions The permissions of the file. Refer to
 * libpermissions.h
 * @param size The size of the file, in bytes
 * @param is_directory A value describing if the file is a directory
 * @param contents A pointer to the contents of the file
 * @return A pointer to the new file, or NULL if something failed
 */
file_t *file_new(char *name, const uint8_t owner_id, const uint8_t group_id,
                 const uint16_t permissions, const uint32_t size,
                 const bool is_directory, void *contents);

/**
 * @brief Frees the memory taken by a file object
 * @param file The file
 * @return True or false, depending on the result of the operation
 */
bool file_free(file_t *file);

/**
 * @brief Renames a given file, while freeing the memory allocated to
 * the current name string
 * @param file The file
 * @param name The new name
 * @return True or false, depending on the result of the operation
 */
bool file_set_name(file_t *file, char *name);

/**
 * @brief Returns A COPY of the name of a file
 * @param file The file
 * @return A COPY of the name of the file, or NULL if something failed.
 */
char *file_get_name(const file_t *file);

/**
 * @brief Changes the owner of a given file
 * @param file The file
 * @param owner_id The id of the new owner
 * @return True or false, depending on the result of the operation
 */
bool file_set_owner_id(file_t *file, const uint8_t owner_id);

/**
 * @brief Returns the id of the current owner of a file
 * @param file The file
 * @return The id of the owner, or UINT8_MAX if something failed.
 */
uint8_t file_get_owner_id(const file_t *file);

/**
 * @brief Changes the group of a given file
 * @param file The file
 * @param group_id The id of the new group
 * @return True or false, depending on the result of the operation
 */
bool file_set_group_id(file_t *file, const uint8_t group_id);

/**
 * @brief Returns the current group id of a given file
 * @param file The file
 * @return The current group id of the file, or UINT8_MAX if something
 * failed.
 */
uint8_t file_get_group_id(const file_t *file);

/**
 * @brief Changes the permissions of a given file
 * @param file The file
 * @param permissions A number that defines the permissions. Refer to
 * libpermissions.h
 * @return True or false, depending on the result of the operation
 */
bool file_set_permissions(file_t *file, const uint16_t permissions);

/**
 * @brief Returns the current permissions of a file
 * @param file The file
 * @return The permissions of the file, or UINT16_MAX if something
 * failed. Refer to libpermissions.h for possible values.
 */
uint16_t file_get_permissions(const file_t *file);

/**
 * @brief Modifies the contents of a file
 * @param file The file
 * @param size The size of the new content, in bytes
 * @param contents A pointer to the new contents
 * @return A pointer to the previous contents of the file, so you can
 * choose to free it, or NULL if something failed
 */
void *file_set_contents(file_t *file, const uint32_t size, void *contents);

/**
 * @brief Returns the contents of a file
 * @param file The file
 * @return A pointer to the contents of the file, or NULL if something
 * failed
 */
void *file_get_contents(const file_t *file);


/**
 * @brief Checks if user, group or others have permission to do certain
 * things. For instance, can be used to check whether the user can read 
 * and write by passing USER_READ + USER_WRITE as the value for 
 * permissions. Refer to libpermissions.h for possible values.
 * @param file The file
 * @param permissions The permissions to check
 * @return True or false, depending on the result of the operation
 */
bool file_can(const file_t *file, const uint16_t permissions);


/**
 * @brief Creates an exact copy of a file
 * @param file The file
 * @return A copy of the file, or NULL if something failed
 */
file_t *file_copy(const file_t *file);

/**
 * @brief Finds the file in the list whose name matches a 
 * NULL-terminated string
 * @param list The list of files
 * @param name The name of the file sought
 * @return A pointer to the file, or NULL if it could not be found
 */
file_t *file_list_get_by_name(const list_t *list, const char *name);

/**
 * @brief Finds the files in the list whose owner_id matches a parameter
 * @param list The list of files
 * @param owner_id The owner_id to look for
 * @return A new list of files (remember to call list_free on it, but do
 * not free the individual files!)
 */
list_t *file_list_get_by_owner_id(const list_t *list, uint8_t owner_id);

/**
 * @brief Finds the files in the list whose group_id matches a parameter
 * @param list The list of files
 * @param group_id The group_id to look for
 * @return A new list of files (remember to call list_free on it, but do
 * not free the individual files!)
 */
list_t *file_list_get_by_group_id(const list_t *list, uint8_t group_id);

/**
 * @brief Finds all the directories in a list of files
 * @param list The list of files
 * @return A new list of files (remember to call list_free on it, but do
 * not free the individual files!)
 */
list_t *file_list_get_directories(const list_t *list);

/**
 * @brief Finds all the files in a list of files (those which are not 
 * directories)
 * @param list The list of files
 * @return A new list of files (remember to call list_free on it, but do
 * not free the individual files!)
 */
list_t *file_list_get_files(const list_t *list);

#endif