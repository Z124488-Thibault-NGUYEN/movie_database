#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

// How big the buffer for those lines should be
#define MOVIES_LINE_SIZE 2048

// How long any of the chars fields can be.
#define FIELD_BUFF_SIZE 256


typedef struct {
    int id;
    int release_year;
    char title[FIELD_BUFF_SIZE];
    char origin[FIELD_BUFF_SIZE];
    char genre[FIELD_BUFF_SIZE];
    char director[FIELD_BUFF_SIZE];
    bool is_deleted;
} Movie;

Movie *movie_fromline(Movie *movie, char *line);

Movie movie_placeholder();
