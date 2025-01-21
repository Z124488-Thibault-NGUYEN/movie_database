#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINES 100
#define MAX_LINE_LENGTH 256

void ux_display() {
    FILE *file = fopen("movie.csv", "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];

    // Skip the header line
    if (fgets(line, MAX_LINE_LENGTH, file) == NULL) {
        fprintf(stderr, "Error: File is empty\n");
        fclose(file);
        return;
    }

    printf("ID\tYear\tTitle\t\t\t\tOrigin\t\tGenre\t\tDirector\n");
    printf("------------------------------------------------------------------------------------------------------------------\n");

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        int id, year;
        char title[50], origin[50], genre[50], director[50];

        if (sscanf(line, "%d,%d,%49[^,],%49[^,],%49[^,],%49[^\n]",
                   &id, &year, title, origin, genre, director) == 6) {
            printf("%-4d\t%-6d\t%-30s\t%-15s\t%-15s\t%-20s\n",
                   id, year, title, origin, genre, director);
        } else {
            fprintf(stderr, "Ignoring erraneous line\n");
        }
    }

    fclose(file);
}

void ux_add() {
    int year;
    char title[50], nationality[50], type[50], author[50];

    printf("Enter the publication year of the new movie: ");
    if (scanf("%d", &year) != 1) {
        fprintf(stderr, "Failed to parse year\n");
        return;
    };

    printf("Enter the title of the new movie: ");
    scanf("%49s", title);  // Read title with spaces.

    printf("Enter the nationality of the new movie: ");
    scanf("%49s", nationality);

    printf("Enter the genre of the new movie: ");
    scanf("%49s", type);

    printf("Enter the director of the new movie: ");
    scanf("%49s", author);

    FILE *rfile = fopen("movie.csv", "r+");
    if (rfile == NULL) {
        fprintf(stderr, "Error: Could not open file.\n");
        return;
    }

    char *last_valid_line = NULL;
    char lookahead[MAX_LINE_LENGTH];

    while (fgets(lookahead, MAX_LINE_LENGTH, rfile) != NULL) {
        last_valid_line = lookahead;
    }

    if (last_valid_line == NULL) {
        fprintf(stderr, "Error: File is empty\n");
        return;
    }

    char *new_id_c = strtok(last_valid_line, ",");
    int new_id = atoi(new_id_c) + 1;

    if (fprintf(rfile, "%d,%d,%s,%s,%s,%s\n", new_id, year, title, nationality, type, author) < 0) {
        fprintf(stderr, "Error: failed to write the new line to the file\n");
    }
    fclose(rfile);
}

void ux_delete() {
    int id = 0, id_remove, i, id_rank_i, jump = -1;
    char movie[MAX_LINES][MAX_LINE_LENGTH];
    char movie_copy[MAX_LINES][MAX_LINE_LENGTH];

    printf("Enter the ID of the movie to delete: ");
    if (scanf("%d", &id_remove) != 1) {
        fprintf(stderr, "Invalid input. Please enter a valid number.\n");
        return;
    }

    FILE *rfile = fopen("movie.csv", "r");
    if (rfile == NULL) {
        fprintf(stderr, "Error: Could not open file.\n");
        return;
    }

    while (fgets(movie[id], MAX_LINE_LENGTH, rfile) != NULL) {
        id++;
    }
    fclose(rfile);

    for (i = 0; i < id; i++) {
        strcpy(movie_copy[i], movie[i]);
        char *id_rank_c = strtok(movie[i], ",");
        id_rank_i = atoi(id_rank_c);
        if (id_rank_i == id_remove) {
            jump = i;
        }
    }

    if (jump == -1) {
        fprintf(stderr, "Error: No movie found with ID %d.\n", id_remove);
        return;
    }

    FILE *wfile = fopen("movie.csv", "w");
    if (wfile == NULL) {
        fprintf(stderr, "Error: Could not open file for writing.\n");
        return;
    }

    for (i = 0; i < id; i++) {
        if (i != jump) {
            fputs(movie_copy[i], wfile);
        }
    }
    fclose(wfile);
}

void ux_edit() {
    int id_to_edit = -1, id = 0;
    char title[50], author[50], nationality[50], year[50], type[50];
    char movie[MAX_LINES][MAX_LINE_LENGTH];

    printf("Enter the ID of the movie to edit: ");
    if (scanf("%d", &id_to_edit) != 1) {
        fprintf(stderr, "Invalid input. Please enter a valid number.\n");
        return;
    }

    printf("Enter the new title: ");
    scanf(" %49[^\n]", title);

    printf("Enter the new director: ");
    scanf("%49s", author);

    printf("Enter the new nationality: ");
    scanf("%49s", nationality);

    printf("Enter the new publication year: ");
    scanf("%49s", year);

    printf("Enter the new genre: ");
    scanf("%49s", type);

    FILE *rfile = fopen("movie.csv", "r");
    if (rfile == NULL) {
        fprintf(stderr, "Error: Could not open file.\n");
        return;
    }

    while (fgets(movie[id], MAX_LINE_LENGTH, rfile) != NULL) {
        id++;
    }
    fclose(rfile);

    int found = 0;
    for (int i = 0; i < id; i++) {
        char id_rank_c[10];
        sscanf(movie[i], "%9[^,]", id_rank_c);
        int id_rank_i = atoi(id_rank_c);
        if (id_rank_i == id_to_edit) {
            snprintf(movie[i], MAX_LINE_LENGTH, "%d,%s,%s,%s,%s,%s\n",
                     id_to_edit, year, title, nationality, type, author);
            found = 1;
            break;
        }
    }

    if (!found) {
        fprintf(stderr, "Error: No movie found with ID %d.\n", id_to_edit);
        return;
    }

    FILE *wfile = fopen("movie.csv", "w");
    if (wfile == NULL) {
        fprintf(stderr, "Error: Could not open file for writing.\n");
        return;
    }

    for (int i = 0; i < id; i++) {
        fputs(movie[i], wfile);
    }
    fclose(wfile);
}

void ux_error() {
    printf("You have to choose 1, 2, 3, 4, or 5.\n");
}

int main(void) {
    int choice;

    do {
        printf("****Welcome to My Final Project****\n");
        printf("1. Display Movies\n");
        printf("2. Add Movies\n");
        printf("3. Delete Movies\n");
        printf("4. Edit Movies\n");
        printf("5. Exit\n");

        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Invalid input. Please enter a valid number.\n");
            while (getchar() != '\n');  // Clear the input buffer
            continue;
        }

        switch (choice) {
            case 1:
                ux_display();
                break;
            case 2:
                ux_add();
                break;
            case 3:
                ux_delete();
                break;
            case 4:
                ux_edit();
                break;
            case 5:
                break;
            default:
                ux_error();
                break;
        }
    } while (choice != 5);

    return EXIT_SUCCESS;
}

