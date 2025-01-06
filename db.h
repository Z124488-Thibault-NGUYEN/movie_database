#include "types.h"

typedef struct {
    int lines;
} Database;

/**
 * @brief Opens the database and gather informations for it in the provided pointer
 * Sets lines to -1 if the file couldn't be found, and 0 if there are no column names
 */
int db_open(Database *db, char* filename);

/**
 * @brief Displays all movies in the collection
 * @returns -1 if no movies were found, or the amount of movies that were displayed
 */
int db_display();

/**
 * @brief Adds a new movie to the collection.
 * @returns The ID of the newly inserted book, or 0 on error not.
 *
 */
int db_add(Movie movie);

/**
 * @brief Removes a movie from the collection
 * @returns The ID of the removed book, or -1 if it wasn't found
 */
int db_remove(int id);

/**
 * @brief Modifies a movie from the collection
 * @returns The ID of the modified movie, or -1 if it wasn't found
 */
int db_modify(int id);
