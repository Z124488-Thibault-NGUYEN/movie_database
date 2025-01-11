#pragma once

#include "types.h"
#include <stddef.h>

// We don't know how many entries we will have.
// So we reallocate dynamically, doubling the amount of available spaces
// every time. 
// This is easy to implement, and has a low amount of overhead
#define DEFAULT_ENTRY_COUNT 64

#define HEADER "ID,Release Year,Title,Origin,Genre,Director"

typedef struct {
    int movies_count;
    size_t max_count;
    Movie *movies;
} Database;

/**
 * @brief Opens the database and gather informations for it in the provided pointer
 * Sets lines to -1 if the file couldn't be found, and 0 if there are no column names
 */
Database db_init(char* filename);

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
int db_modify(Database *db, Movie new_movie);

/**
 *
 * @brief Commits the current changes in the database
 * @returns The amount of lines written or -1 on error
 */
int db_commit(Database *db, char *filename);

/**
 * @brief Indicates that a DB should not be used.
 * Accessing any field is undefined behaviour
 */
void *db_free(Database *db);
