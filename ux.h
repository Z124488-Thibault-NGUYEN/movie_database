#include "db.h"

/**
 * @brief Displays all movies
 */
void ux_display(Database *db);

/**
 * @brief Asks the user about what movie to add
 */
void ux_add(Database *db);

/**
 * @brief Prompts the user to delete an entry
 */
void ux_delete(Database *db);

/**
 * @brief Prompts the user to edit an entry
 */
void ux_edit(Database *db);

/**
 * @brief Shows the user an error message
 */
void ux_error(Database *db, int invalid_choice);

