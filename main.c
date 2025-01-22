#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINES 100
#define MAX_LINE_LENGTH 256

/******************************************************
 * HELPER FUNCTIONS
 ******************************************************/

/* Safely read a line from stdin (removes trailing '\n'). */
static void safe_read_line(char *buffer, size_t size) {
    if (fgets(buffer, size, stdin)) {
        // Remove trailing newline if present
        char *newline_ptr = strchr(buffer, '\n');
        if (newline_ptr) {
            *newline_ptr = '\0';
        }
    }
    else {
        // If fgets fails, clear the buffer
        if (!feof(stdin)) {
            clearerr(stdin);
        }
        buffer[0] = '\0';
    }
}

/* 
 * Returns a new file pointer opened for reading. 
 * If the file does not exist when adding, create it with a header.
 */
static FILE* open_csv_for_read(const char *filename, bool create_if_missing) {
    FILE *fp = fopen(filename, "r");
    if (!fp && create_if_missing) {
        // Create file with header if missing
        fp = fopen(filename, "w");
        if (!fp) {
            fprintf(stderr, "Error: Could not create file %s.\n", filename);
            return NULL;
        }
        fprintf(fp, "ID,Release Year,Title,Origin,Genre,Director\n");
        fclose(fp);

        // Reopen for reading
        fp = fopen(filename, "r");
    }
    return fp;
}

/******************************************************
 * DISPLAY MOVIES
 ******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

void ux_display() {
    FILE *file = fopen("movie.csv", "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file for reading.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];

    // First line is the CSV header; read & verify
    if (!fgets(line, sizeof(line), file)) {
        // File is empty or unreadable
        fprintf(stderr, "Error: File is empty or unreadable.\n");
        fclose(file);
        return;
    }

    // Print a header for our table display
    printf("ID   Year   Title                               Origin          Genre       Director\n");
    printf("------------------------------------------------------------------------------------\n");

    // Read remaining lines (actual movie data)
    while (fgets(line, sizeof(line), file)) {
        int id, year;
        // Temporarily use larger buffers to read from CSV
        char title[80], origin[80], genre[80], director[80];

        // Attempt to parse all 6 expected fields
        if (sscanf(line, "%d,%d,%79[^,],%79[^,],%79[^,],%79[^\n]",
                   &id, &year, title, origin, genre, director) == 6) {
            /*
             * Truncated printing:
             *   - ID: 4 wide
             *   - Year: 6 wide
             *   - Title: 35 wide (truncate if longer)
             *   - Origin: 15 wide (truncate if longer)
             *   - Genre: 10 wide (truncate if longer)
             *   - Director: 15 wide (truncate if longer)
             */
            printf("%-4d %-6d %-35.35s %-15.15s %-10.10s %-15.15s\n",
                   id, year, title, origin, genre, director);

        } else {
            // If we can't parse a full line, skip it
            fprintf(stderr, "Ignoring malformed line: %s", line);
        }
    }

    fclose(file);
}

/******************************************************
 * ADD MOVIES
 ******************************************************/
void ux_add() {
    int year;
    char title[50], nationality[50], type[50], author[50];

    // Prompt user for input
    printf("Enter the publication year of the new movie: ");
    if (scanf("%d", &year) != 1 || year < 0 || year > 9999) {
        fprintf(stderr, "Failed to parse year.\n");
        // clear stdin if needed
        while (getchar() != '\n');
        return;
    }

    while (getchar() != '\n'); // consume leftover newline

    printf("Enter the title of the new movie: ");
    safe_read_line(title, sizeof(title));

    printf("Enter the nationality of the new movie: ");
    safe_read_line(nationality, sizeof(nationality));

    printf("Enter the genre of the new movie: ");
    safe_read_line(type, sizeof(type));

    printf("Enter the director of the new movie: ");
    safe_read_line(author, sizeof(author));

    // 1) Find the max ID from the existing file
    FILE *rfile = open_csv_for_read("movie.csv", true);
    if (!rfile) {
        // Could not open or create file
        return;
    }

    char line[MAX_LINE_LENGTH];
    int max_id = 0;

    // Read header line
    if (fgets(line, sizeof(line), rfile) == NULL) {
        fprintf(stderr, "File is empty !");
        return;
    }

    // For each subsequent line, parse the ID
    while (fgets(line, sizeof(line), rfile)) {
        int current_id;
        // We only care about the first token (ID)
        if (sscanf(line, "%d,", &current_id) == 1) {
            if (current_id > max_id) {
                max_id = current_id;
            }
        }
    }
    fclose(rfile);

    // 2) Now append the new movie with ID = max_id + 1
    FILE *afile = fopen("movie.csv", "a");
    if (!afile) {
        fprintf(stderr, "Error: Could not open file for appending.\n");
        return;
    }

    if (fprintf(afile, "%d,%d,%s,%s,%s,%s\n",
                max_id + 1, year, title, nationality, type, author) < 0) {
        fprintf(stderr, "Error: failed to write the new line to the file.\n");
    }

    fclose(afile);
}


/******************************************************
 * DELETE MOVIES
 ******************************************************/
void ux_delete() {
    int id_remove;

    printf("Enter the ID of the movie to delete: ");
    if (scanf("%d", &id_remove) != 1) {
        fprintf(stderr, "Invalid input. Please enter a valid number.\n");
        while (getchar() != '\n'); // clear buffer
        return;
    }
    while (getchar() != '\n'); // consume leftover newline

    FILE *rfile = fopen("movie.csv", "r");
    if (!rfile) {
        fprintf(stderr, "Error: Could not open file.\n");
        return;
    }

    char movie[MAX_LINES][MAX_LINE_LENGTH];
    int line_count = 0;

    // Read up to MAX_LINES lines into memory
    // Quick and dirty fix
    while (line_count < MAX_LINES && fgets(movie[line_count], MAX_LINE_LENGTH, rfile)) {
        line_count++;
    }
    fclose(rfile);

    if (line_count == 0) {
        fprintf(stderr, "Error: The file is empty. Nothing to delete.\n");
        return;
    }

    // We'll rewrite everything except the line matching the given ID
    // The first line is the header; always keep it
    int i;
    int deleted_index = -1; // track if we found and deleted the ID
    for (i = 1; i < line_count; i++) {
        // Parse the ID from each line (skip the header)
        int current_id;
        if (sscanf(movie[i], "%d,", &current_id) == 1) {
            if (current_id == id_remove) {
                deleted_index = i;
                break;
            }
        }
    }

    if (deleted_index == -1) {
        fprintf(stderr, "Error: No movie found with ID %d.\n", id_remove);
        return;
    }

    // Now rewrite the file
    FILE *wfile = fopen("movie.csv", "w");
    if (!wfile) {
        fprintf(stderr, "Error: Could not open file for writing.\n");
        return;
    }

    // Write everything except the deleted line
    for (i = 0; i < line_count; i++) {
        if (i == deleted_index) {
            // skip this
            continue;
        }
        fputs(movie[i], wfile);
    }
    fclose(wfile);
}


/******************************************************
 * EDIT MOVIES
 ******************************************************/
void ux_edit() {
    int id_to_edit;

    printf("Enter the ID of the movie to edit: ");
    if (scanf("%d", &id_to_edit) != 1) {
        fprintf(stderr, "Invalid input. Please enter a valid number.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n'); // consume leftover newline

    // Gather new fields
    int year;
    char title[50], author[50], nationality[50], type[50];

    printf("Enter the new publication year: ");
    if (scanf("%d", &year) != 1 || year < 0 || year > 9999) {
        fprintf(stderr, "Failed to parse year.\n");
        // clear stdin if needed
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n'); // consume leftover newline

    printf("Enter the new title: ");
    safe_read_line(title, sizeof(title));

    printf("Enter the new origin: ");
    safe_read_line(nationality, sizeof(nationality));

    printf("Enter the new genre: ");
    safe_read_line(type, sizeof(type));

    printf("Enter the new director: ");
    safe_read_line(author, sizeof(author));

    FILE *rfile = fopen("movie.csv", "r");
    if (!rfile) {
        fprintf(stderr, "Error: Could not open file.\n");
        return;
    }

    char movie[MAX_LINES][MAX_LINE_LENGTH];
    int line_count = 0;

    // Read all lines into memory
    while (line_count < MAX_LINES && fgets(movie[line_count], MAX_LINE_LENGTH, rfile)) {
        line_count++;
    }
    fclose(rfile);

    if (line_count == 0) {
        fprintf(stderr, "Error: The file is empty. Nothing to edit.\n");
        return;
    }

    // Attempt to find the line with the matching ID (skipping header)
    int found = 0;
    for (int i = 1; i < line_count; i++) {
        int current_id;
        if (sscanf(movie[i], "%d,", &current_id) == 1) {
            if (current_id == id_to_edit) {
                snprintf(movie[i], MAX_LINE_LENGTH, "%d,%d,%s,%s,%s,%s\n",
                         id_to_edit, year, title, nationality, type, author);
                found = 1;
                break;
            }
        }
    }

    if (!found) {
        fprintf(stderr, "Error: No movie found with ID %d.\n", id_to_edit);
        return;
    }

    // Rewrite the file with the updated line
    FILE *wfile = fopen("movie.csv", "w");
    if (!wfile) {
        fprintf(stderr, "Error: Could not open file for writing.\n");
        return;
    }
    for (int i = 0; i < line_count; i++) {
        fputs(movie[i], wfile);
    }
    fclose(wfile);
}


/******************************************************
 * INVALID CHOICE
 ******************************************************/
void ux_error() {
    printf("You have to choose 1, 2, 3, 4, or 5.\n");
}


/******************************************************
 * MAIN
 ******************************************************/
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

