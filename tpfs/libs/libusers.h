#ifndef LIB_USERS
#define LIB_USERS

#include <stdint.h>
#include <stdbool.h>

#include "liblists.h"

typedef struct _user_t
{
    uint8_t user_id;
    char *name;
    char *password;
} user_t;

/**
 * @brief Creates a new user
 * @param user_id The user id
 * @param name The name of the user
 * @param password The user's password
 * @return A pointer to the new user, or NULL if something failed
 */
user_t *user_new(uint8_t user_id, char *name, char *password);

/**
 * @brief Frees the memory taken up by a user
 * @param user A pointer to the user
 * @return True or false, depending on the result of the operation
 */
bool user_free(user_t *user);

/**
 * @brief Returns the id of a user
 * @param user A pointer to the user
 * @return The user's id, or UINT8_MAX if something failed
 */
uint8_t user_get_id(user_t *user);

/**
 * @brief Sets the id of a user
 * @param user A pointer to the user
 * @param user_id The new id
 * @return True or false, depending on the result of the operation
 */
bool user_set_id(user_t *user, uint8_t user_id);

/**
 * @brief Returns A COPY of the name of the user
 * @param user A pointer to the user
 * @return A COPY of the name of the user
 */
char *user_get_name(user_t *user);

/**
 * @brief Sets the name of a user, while freeing the memory taken up by
 * the previous name
 * @param user A pointer to the user
 * @param name The new name of the user
 * @return True or false, depending on the result of the operation
 */
bool user_set_name(user_t *user, char *name);

/**
 * @brief Returns A COPY of the user's password
 * @param user A pointer to the user
 * @return A COPY of the user's password, or NULL if something failed
 */
char *user_get_password(user_t *user);

/**
 * @brief Sets the password for a user, while freeing the memory taken 
 * up by the previous password
 * @param user A pointer to the user
 * @param password The new password
 * @return True or false, depending on the result of the operation
 */
bool user_set_password(user_t *user, char *password);

/**
 * @brief Returns a pointer to a user whose name matches a string
 * @param list A pointer to a list of users
 * @param name A NULL-terminated string containing the name to look for
 * @return A pointer to the user, or NULL if it could not be found
 */
user_t *user_list_get_by_name(const list_t *list, const char *name);

/**
 * @brief Returns a pointer to a user whose id matches a value
 * @param list A pointer to a list of users
 * @param id The id to look for
 * @return A pointer to the user, or NULL if it could not be found
 */
user_t *user_list_get_by_id(const list_t *list, uint8_t id);

/**
 * @brief Frees a list of users
 * @param list The list
 * @return True or false, depending on the result of the operation
 */
bool user_list_free(list_t *list);

#endif