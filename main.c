#include <stdlib.h>
#include <stdio.h>

void ux_display() {

}

void ux_add() {

}

void ux_delete() {

}

void ux_edit() {

}

void ux_error() {

}


int main(void) {
    int choice;

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


