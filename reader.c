#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "reader.h"
#include "header.h"
#define RED     "\033[1;31m" //define for red color
#define YELLOW  "\033[1;33m" //define for yellow color
#define RESET   "\033[0m" //define for reset color

TagData* read_id3_tags(const char *filename)  // Read ID3 tags from MP3 file
{ 
    FILE *file = fopen(filename, "rb");

    // Check whether file opened successfully
    if (!file) 
    {
        perror("Could not open file");
        return NULL;
    }

    // Create structure to store tag information
    TagData *tag = create_tag_data();
    if (!tag)
    {
        fclose(file);
        return NULL;
    }

    // Move to the first frame after ID3 header
    fseek(file, 10, SEEK_SET);

    unsigned char tag_buffer[5];

    // Read required ID3 frames
    for (int i = 0; i < 6; i++)
    {
        if (fread(tag_buffer, 1, 4, file) != 4)
            break;

        tag_buffer[4] = '\0';

        // Read frame size
        unsigned char size_buf[4];
        fread(size_buf, 1, 4, file);

        unsigned int size =(size_buf[0] << 24) | (size_buf[1] << 16) | (size_buf[2] << 8)  | size_buf[3];

        // Skip frame flags
        fseek(file, 2, SEEK_CUR);

        // Allocate memory for frame data
        unsigned char *new_data = malloc(size + 1);
        if (!new_data)
            break;

        // Read frame content
        fread(new_data, 1, size, file);
        new_data[size] = '\0';

        char *text = (char *)(new_data + 1);

        // Store frame data into corresponding structure fields
        if (strcmp(tag_buffer, "TIT2") == 0)
            tag->title = strdup(text);

        else if (strcmp(tag_buffer, "TPE1") == 0)
            tag->artist = strdup(text);

        else if (strcmp(tag_buffer, "TALB") == 0)
            tag->album = strdup(text);

        else if (strcmp(tag_buffer, "TYER") == 0 ||
                 strcmp(tag_buffer, "TDRC") == 0)
            tag->year = strdup((char *)text);

        else if (strcmp(tag_buffer, "TCON") == 0)
            tag->genre = strdup(text);

        else if (strcmp(tag_buffer, "COMM") == 0)
            tag->comment = strdup(text);

        free(new_data);
    }

    // Close file and return extracted metadata
    fclose(file);
    return tag;
}

void display_metadata(const TagData *data) // Display MP3 metadata
{
    printf("---------------------------------------------------------\n");
    printf("          MP3 TAG READRE AND EDITOR FOR ID3V2\n");
    printf("---------------------------------------------------------\n");
    printf("\n------------------ ID3 TAG METADATA----------------------\n");

    printf("TITLE   : %s\n", data->title  ? data->title  : "Not available");
    printf("ARTIST  : %s\n", data->artist ? data->artist : "Not available");
    printf("ALBUM   : %s\n", data->album  ? data->album  : "Not available");
    printf("YEAR    : %s\n", data->year   ? data->year   : "Not available");
    printf("MUSIC   : %s\n", data->genre  ? data->genre  : "Not available");
    printf("Comment : %s\n", data->comment? data->comment: "Not available");

    printf("----------------------------------------------------------\n");
}

void view_tags(const char *filename) // View metadata of an MP3 file
{
    // Read tag information from file
    TagData *data = read_id3_tags(filename);

    // Check whether tags were read successfully
    if (!data)
    {
        perror("Failed to read ID3 tags");
        return;
    }

    // Display and free metadata
    display_metadata(data);
    free_tag_data(data);
}