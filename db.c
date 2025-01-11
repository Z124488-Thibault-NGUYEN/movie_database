#include "db.h"
#include "types.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void db_clean(Database *db) {
    db->max_count = 0;
    free(db->movies);
    db->movies = NULL;
    db->movies_count = 0;
}

Database db_init(char *filename) {
    FILE *file = fopen(filename, "r");

    Database db = {
        .max_count=DEFAULT_ENTRY_COUNT,
        .movies=calloc(DEFAULT_ENTRY_COUNT, sizeof(Movie)),
        .movies_count=0
    };

    char buff[MOVIES_LINE_SIZE];

    if (fgets(buff, MOVIES_LINE_SIZE, file) == NULL) {
        // I miss my goto ...
        fclose(file);
        return db;
    }


    while (fgets(buff, MOVIES_LINE_SIZE, file) != NULL) {

        Movie movie;

        // In case of error, we wouldn't have reached EOF yet
        if (movie_fromline(&movie, buff) == NULL) {
            break;
        }

        // Add placeholders to keep 1-1 id <=> index mapping
        while (db.movies_count < movie.id) {
            db_add(&db, movie_placeholder());
        }

        db_add(&db, movie);
    }

    // Successfully reached end of file
    if (!feof(file)) {
        db_clean(&db);
    }

    fclose(file);
    return db;
}

int db_add(Database *db, Movie movie) {
    while (db->movies_count > db->max_count) {
        db->max_count *= 2;
        if (realloc(db->movies, db->max_count) == NULL) {
            return -1;
        }
    }

    db->movies[db->movies_count] = movie;

    return db->movies_count++;
}

int db_remove(Database *db, int id) {
    if (id < 0 || id > db->movies_count) {
        return -1;
    }

    if (db->movies[id].is_deleted) {
        return -1;
    }

    db->movies[id].is_deleted = true;
    return id;
}

int db_modify(Database *db, Movie new_movie) {
    if (new_movie.id < 0 || new_movie.id > db->movies_count) {
        return -1;
    }

    db->movies[new_movie.id] = new_movie;

    return new_movie.id;
}

int db_commit(Database *db, char *fname) {
    FILE *file = fopen(fname, "w");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    return 0;
}

void *db_free(Database *db) {
    free(db->movies);
}
