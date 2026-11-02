#ifndef HEADER_H //if not defined HEADER_H, define it and include the definition of the TagData structure and related functions
#define HEADER_H

#include <stdio.h>

//structure to hold MP3 tag data
typedef struct {
    char *title;   // Title of the song 
    char *artist;  // Artist of the song 
    char *album;   // Album name 
    char *year;    // Year of release 
    char *genre;   // Genre of the song
    char *comment; // Comment
} TagData; 

TagData* create_tag_data(); // Function prototype to create and initialize a TagData structure

void free_tag_data(TagData *data);  // Function prototype to free memory allocated for a TagData structure

void display_error(); // Function prototype to display error message for invalid arguments

void help_menu(); // Function prototype to display help menu

int check_file_exists(const char *filename); // Function prototype to check if a file exists, returns 1 if it exists, 0 otherwise

#endif // ID3_UTILS_H