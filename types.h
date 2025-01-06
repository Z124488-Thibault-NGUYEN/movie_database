#pragma once

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

// How big the buffer for those lines should be
#define FIELD_LINE_SIZE 2048

// How long any of the chars fields can be.
#define FIELD_BUFF_SIZE 256


typedef struct {
    int release_year;
    char title[FIELD_BUFF_SIZE];
    char origin[FIELD_BUFF_SIZE];
    char genre[FIELD_BUFF_SIZE];
    char director[FIELD_BUFF_SIZE];
} Movie;


typedef struct {
    int id;
    Movie movie;
} MovieEntry;

int movie_fromline(Movie *movie, char *line);

MovieEntry movie_entry_init(int id, Movie movie);


