#include "types.h"

typedef struct {
    int movies_count;
    Movie *movies;
} Database;

/**
 * @brief Opens the database and gather informations for it in the provided pointer
 * Sets lines to -1 if the file couldn't be found, and 0 if there are no column names
 */
int db_create(char* filename);

/**
 * @brief Displays all movies in the collection
 * @returns -1 if no movies were found, or the amount of movies that were displayed
 */
int db_display(Database *db);

/**
 * @brief Adds a new movie to the collection.
 * @returns The ID of the newly inserted book, or 0 on error not.
 *
 */
int db_add(Database *db, Movie movie);

/**
 * @brief Removes a movie from the collection
 * @returns The ID of the removed book, or -1 if it wasn't found
 */
int db_remove(Database *db, int id);

/**
 * @brief Modifies a movie from the collection
 * @returns The ID of the modified movie, or -1 if it wasn't found
 */
int db_modify(Database *db, int id);

/**
 *
 * @brief Commits the current changes in the database
 * @returns The amount of lines written or -1 on error
 */
int db_commit(Database *db);
