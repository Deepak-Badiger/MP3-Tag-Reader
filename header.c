#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

TagData* create_tag_data() {
    TagData *data = (TagData *)malloc(sizeof(TagData)); //allocate memory for TagData structure
    if (data) { //initialize all fields to NULL
        data->title = NULL; 
        data->artist = NULL;
        data->album = NULL;
        data->year = NULL;
        data->genre = NULL;
        data->comment = NULL;
    }
    return data;
}

void free_tag_data(TagData *data) {
    if (data) { //free memory allocated for each field and the structure itself
        free(data->title);
        free(data->artist);
        free(data->album);
        free(data->year);
        free(data->genre);
        free(data->comment);
        free(data);
    }
}

void display_error()  //function defineation to display error message for invalid arguments
{ 
    printf("---------------------ERROR MESSAGE--------------------------\n");
    printf("\nERROR : a.out : Invalid arguments\n");
    printf("USAGE :\n");
    printf("To view pass like this : ./a.out -v <mp3_file>\n");                                     //err message for view option
    printf("TO edit pass like this: ./a.out -e -t/-a/-A/-y/-m/-c [changing_text mp3 file name]\n");
    printf("To get heip pass like this: ./a.out --help\n");
    printf("\n------------------------------------------------------------\n");
} 

void help_menu() //function defineation to display help menu
{
    printf("------------------- HELP MENU -----------------------\n");
    printf("\n1. -v --> to view mp3 file contents\n");            //-v to view mp3 file contents
    printf("2. -e --> to edit mp3 file contents\n");              //-e to edit mp3 file contents
    printf("\t2.1 -> -t --> to edit song title\n");               //-t to edit song title
    printf("\t2.2 -> -a --> to edit artist name\n");              //-a to edit artist name
    printf("\t2.3 -> -A --> to edit album name\n");               //-A to edit album name
    printf("\t2.4 -> -y --> to edit year of release\n");          //-y to edit year of release  
    printf("\t2.5 -> -m --> to edit content (or) music type\n");  //-m to edit content (or) music type
    printf("\t2.6 -> -c --> to edit comment\n");                  //-c to edit comment
    printf("\n---------------------------------------------------\n");
}

int check_file_exists(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        return 0;   // file does not exist
    }
    fclose(fp);
    return 1;       // file exists
}