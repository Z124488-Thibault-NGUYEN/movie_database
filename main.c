#include <stdlib.h>
#include <stdio.h>

#include "db.h"

#define DB_FNAME "db.csv"

int main(void) {
    int choice;

    do {
        printf("****Welcome to my final project****\n");
        printf("1. Display Books\n");
        printf("2. Add Books\n");
        printf("3. Delete Books\n");
        printf("4. Edit Books\n");
        printf("5. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
        }

    } while (choice != 5);

    return EXIT_SUCCESS;
}
