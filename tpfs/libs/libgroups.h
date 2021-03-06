#ifndef LIB_GROUPS
#define LIB_GROUPS

#include <stdint.h>

#include "liblists.h"
#include "libusers.h"

typedef struct _group_t
{
    uint8_t group_id;
    char *name;
    list_t *users;
} group_t;

/**
 * @brief Creates a new group
 * @param group_id The id of the group
 * @param name The name of the group
 * @return A pointer to the new group, or NULL if something failed
 */
group_t *group_new(uint8_t group_id, char *name);

/**
 * @brief Frees the memory taken up by a group
 * @param group A pointer to the group
 * @return True or false, depending on the result of the operation
 */
bool group_free(group_t *group);

/**
 * @brief Returns the id of a group
 * @param group A pointer to the group
 * @return The id of the group, or UINT8_MAX if something failed
 */
uint8_t group_get_id(const group_t *group);

/**
 * @brief Sets the id of a group
 * @param group A pointer to the group
 * @param group_id The new id
 * @return True or false, depending on the result of the operation
 */
bool group_set_id(group_t *group, uint8_t group_id);

/**
 * @brief Returns A COPY of the name of the group
 * @param group A pointer to the group
 * @return A COPY of the name of the group, or NULL if something failed
 */
char *group_get_name(group_t *group);

/**
 * @brief Sets the name of the group
 * @param group A pointer to the group
 * @param name The new name
 * @return True or false, depending on the result of the operation
 */
bool group_set_name(group_t *group, char *name);

/**
 * @brief Adds a user to the group
 * @param group A pointer to the group
 * @param user A pointer to the user
 * @return True or false, depending on the result of the operation
 */
bool group_add_user(group_t *group, user_t *user);

/**
 * @brief Removes a user from the group
 * @param group A pointer to the group
 * @param user A pointer to the user
 * @return True or false, depending on the result of the operation
 */
bool group_remove_user(group_t *group, user_t *user);

/**
 * @brief Checks if a user belongs to a group
 * @param group A pointer to the group
 * @param user A pointer to the user
 * @return True or false, depending on the result of the operation
 */
bool user_in_group(group_t *group, user_t *user);

/**
 * @brief Returns a pointer to a group whose name matches a string
 * @param list A pointer to a list of groups
 * @param name A NULL-terminated string containing the name to look for
 * @return A pointer to the group, or NULL if it could not be found
 */
group_t *group_list_get_by_name(const list_t *list, const char *name);

/**
 * @brief Returns a pointer to a group whose id matches a value
 * @param list A pointer to a list of groups
 * @param id The id to look for
 * @return A pointer to the group, or NULL if it could not be found
 */
group_t *group_list_get_by_id(const list_t *list, uint8_t id);

/**
 * @brief Returns the member of the group whose id matches a parameter
 * @param group The group
 * @param id The id of the user
 * @return A pointer to the user, or NULL if it could not be found
 */
user_t *group_get_member_by_id(const group_t *group, uint8_t id);

/**
 * @brief Returns the member of the group whose name matches a parameter
 * @param group The group
 * @param name The name of the user
 * @return A pointer to the user, or NULL if it could not be found
 */
user_t *group_get_member_by_name(const group_t *group, const char *name);

/**
 * @brief Calls a function for each user in the group
 * @param group The group
 * @param ctx An optional pointer to context (can be NULL)
 * @param callback The function to call
 * @return (void)
 */
void group_traverse(const group_t *group, void *ctx,
                    void callback(_list_node_t *user, void *ctx));

/**
 * @brief Frees a list of groups. Does NOT free the users within the
 * group
 * @param list The list
 * @return True or false, depending on the result of the operation
 */
bool groups_list_free(list_t *list);

/**
 * @brief Returns a list with all the members belonging to a group
 * @param group The group
 * @return A pointer to a list of user_t
 */
list_t *group_get_members(const group_t *group);

/**
 * @brief Removes every instance of user from every group in the list
 * @param groups A pointer to the list of groups
 * @param user A pointer to the user
 * @return (void)
 */
void group_list_remove_member(list_t *groups, user_t *user);

#endif