#pragma once

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>

#include "types.h"


/*
 * @brief Trim leading and ending widespace from line
 */
static char *trim_whitespace(char *str) {
    while (isspace((unsigned char)*str)) {
        str++;
    }

    // All spaces or empty
    if (str[0] == '\0') {
        return NULL;
    }

    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }

    end[1] = '\0';

    return str;
}

/*
 * @brief Fills the provided buffer with the next field in the provided line
 */
static void parse_field(char **str, char *buffer, size_t buffer_size) {
    char *p = *str;
    size_t len = 0;

    while (*p != '\0' && *p != ',' && len < buffer_size - 1) {
        buffer[len++] = *p++;
    }
    buffer[len] = '\0';

    if (*p == ',') {
        p++;
    }

    *str = p;
}

/**
 * @brief Populates the provided movie with data extracted from the extracted line
 */
Movie* movie_fromline(Movie *movie, char *line) {
    char *ptr = line;

    char buffer[FIELD_BUFF_SIZE];
    parse_field(&line, buffer, FIELD_BUFF_SIZE);
    if (sscanf(buffer, "%d", &movie->id) != 1) {
        fprintf(stderr, "Failed to parse release year for %s", ptr);
    }

    parse_field(&line, buffer, FIELD_BUFF_SIZE);
    if (sscanf(buffer, "%d", &movie->release_year) != 1) {
        fprintf(stderr, "Failed to parse release year for %s", ptr);
    }

    parse_field(&line, movie->title, FIELD_BUFF_SIZE);
    if (*movie->title == '\0') {
        fprintf(stderr, "Failed to parse title for %s", ptr);
        return NULL;
    }

    parse_field(&line, movie->origin, FIELD_BUFF_SIZE);
    if (*movie->origin == '\0') {
        fprintf(stderr, "Failed to parse origin for %s", ptr);
        return NULL;
    }

    parse_field(&line, movie->genre, FIELD_BUFF_SIZE);
    if (*movie->genre == '\0') {
        fprintf(stderr, "Failed to parse genre for %s", ptr);
        return NULL;
    }

    parse_field(&line, movie->director, FIELD_BUFF_SIZE);
    if (*movie->director == '\0') {
        fprintf(stderr, "Failed to parse director for %s", ptr);
        return NULL;
    }

    return movie;
}

Movie movie_placeholder() {
    return (Movie){
        .id=-1,
        .release_year=1,
        .title='\0',
        .origin='\0',
        .genre='\0',
        .director='\0',
        .is_deleted=true
    };
}

