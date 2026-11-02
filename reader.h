#ifndef READER_H
#define READER_H

#include "header.h" // Include the header file that contains the definition of the TagData structure and related functions

TagData* read_id3_tags(const char *filename); //function prototype or declaration 

void display_metadata(const TagData *data); //function prototype or declaration 

void view_tags(const char *filename); //function prototype or declaration 

#endif // ID3_READER_H