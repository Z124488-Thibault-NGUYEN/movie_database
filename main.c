#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define max_lines 100
#define max_line_length 256

void ux_display() {
    FILE *file=fopen("movie.csv", "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file.\n");
        return;
    }

    char line[max_line_length];
    // Skip the header line
    fgets(line, max_line_length, file);

    printf("ID\tYear\tTitle\t\t\t\tOrigin\t\tGenre\t\tDirector\n");
    printf("------------------------------------------------------------------------------------------------------------------\n");

    while (fgets(line, max_line_length, file) != NULL) {
        int id, year;
        char title[50], origin[50], genre[50], director[50];

        if (sscanf(line, "%d,%d,%49[^,],%49[^,],%49[^,],%49[^\n]",
                    &id, &year, title, origin, genre, director) == 6) {
            printf("%-4d\t%-6d\t%-30s\t%-15s\t%-15s\t%-20s\n",
                    id, year, title, origin, genre, director);
        }
    }

    fclose(file);
}

void ux_add() {
    int new_id, id=0;
	char movie[max_lines][max_line_length];
	char year[50];
    printf("Enter the publication year of the new movie\n");
    scanf("%s", year);

	char title[50];
    printf("Enter the title of the new movie\n");
    scanf("%s", title);
	
    char nationality[50];
    printf("Enter the nationality of the new movie\n");
    scanf("%s", nationality);

    char type[50];
    printf("Enter the type of the new movie\n");
    scanf("%s", type);
	
    char author[50];
    printf("Enter the author of the new movie\n");
    scanf("%s", author);
    
	FILE *rfile = fopen("movie.csv","r");
    if (rfile == NULL)
        {
        printf("Error");
        return;
        }
    while (fgets(movie[id], max_line_length, rfile) != NULL)
        {
        id++;
        }
    fclose(rfile);
	
    char *new_id_c;
	new_id_c = strtok(movie[id-1],",");
    new_id = atoi(new_id_c) + 1;
		
	FILE *afile=fopen("movie.csv", "a");			
	fprintf(afile, "\n%d, %s, %s, %s, %s, %s", new_id, year, title, nationality, type, author);
    fclose(afile);		
	}

void ux_delete() {
    int id=0, id_remove, i, id_rank_i, jump;
	char movie[max_lines][max_line_length];
	char movie_copy[max_lines][max_line_length];
	char *id_rank_c;

	printf("Enter the ID of the movie\n");
    scanf ("%d", &id_remove);

	FILE *rfile = fopen ("movie.csv", "r");
    while (fgets(movie[id], max_line_length, rfile) != NULL)
		{
		id++;
		}
	fclose(rfile);
	
	for (i=0; i<id; i++)
        {
		strcpy(movie_copy[i], movie[i]);
		id_rank_c = strtok(movie[i],",");
		id_rank_i = atoi(id_rank_c);
        if (id_rank_i==id_remove)
    		{
			jump = i;
            }
        }
	FILE *wfile = fopen ("movie.csv", "w");
	for (i=0; i<id; i++)
		{
		if (i!=jump)
    		{
			fputs(movie_copy[i], wfile);
            }
		}
	fclose(wfile);
    }

void ux_edit() {
    int x;
    printf("Enter the ID of the movie\n");
    scanf("%d", &x);

    char title[50];
    printf("Enter the title of the new movie\n");
    scanf("%s", title);

    char author[50];
    printf("Enter the author of the new movie\n");
    scanf("%s", author);

    char nationality[50];
    printf("Enter the nationality of the new movie\n");
    scanf("%s", nationality);

    char year[50];
    printf("Enter the publication year of the new movie\n");
    scanf("%s", year);

    char type[50];   
    printf("Enter the type of the new movie\n");
    scanf("%s", type);

    FILE *rfile = fopen("movie.csv","r");
    if (rfile == NULL) 
    {
        printf("Error");
        return;
    }
    char movie[max_lines][max_line_length];
    int id = 0;

    while (fgets(movie[id], max_line_length, rfile) != NULL)
    {
        id++;
    }
    fclose(rfile);

    snprintf(movie[x], max_line_length, "%d,%s,%s,%s,%s,%s\n", 
            x, title, author, nationality, year, type);

    FILE *wfile = fopen("movie.csv", "w");

    for (int i = 0; i < id; i++) 
    {
        fputs(movie[i], wfile);
    }
    fclose(wfile);

}

void ux_error() {
    printf("You have to choose 1, 2, 3, 4 or 5"); 
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


