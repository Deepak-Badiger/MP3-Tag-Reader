#ifndef EDIT_H //if not defined EDIT_H, define it and include function prototypes for editing MP3 tags
#define EDIT_H // Function prototypes for editing MP3 tags

#include <stdio.h>
#include "header.h"

char *get_frame_id(const char *tag); //function prototype or declaration to get frame id based on user input tag

int opration_new_file(FILE *scr, FILE *dest, char *new_data); //function prototype or declaration to write new data into destination file

int edit_tag(const char *filename, const char *tag, char *new_data); //function prototype or declaration to edit tag in mp3 file, returns 0 on success, -1 on failure

#endif