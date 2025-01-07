#include <stdlib.h>
#include <stdio.h>

#include "db.h"
#include "ux.h"

#define DB_FNAME "db.csv"


int main(void) {
    int choice;

    Database db = db_init(DB_FNAME);
    if (db.movies == NULL) {
        fprintf(stderr, "Error setting up database");
        return EXIT_FAILURE;
    }

    do {
        printf("****Welcome to my final project****\n");
        printf("1. Display Movies\n");
        printf("2. Add Movies\n");
        printf("3. Delete Movies\n");
        printf("4. Edit Movies\n");
        printf("5. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                ux_display(&db);
                break;
            case 2:
                ux_add(&db);
                break;
            case 3:
                ux_add(&db);
                break;
            case 4:
                ux_edit(&db);
                break;
            case 5:
                break;
            default:
                ux_error(&db, choice);
                break;
        }
    } while (choice != 5);

    return EXIT_SUCCESS;
}
