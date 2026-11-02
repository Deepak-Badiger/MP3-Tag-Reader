#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "edit.h"
#include "header.h"

char *get_frame_id(const char *tag) //function defineation to get frame id based on user input tag
{
    if (strcmp(tag, "-t") == 0) return "TIT2"; // Title
    if (strcmp(tag, "-a") == 0) return "TPE1"; // Artist
    if (strcmp(tag, "-A") == 0) return "TALB"; // Album
    if (strcmp(tag, "-y") == 0) return "TYER"; // Year
    if (strcmp(tag, "-m") == 0) return "TCON"; // Music / Genre
    if (strcmp(tag, "-c") == 0) return "COMM"; // Comment

    return NULL; // Invalid tag
}

int opration_new_file(FILE *scr, FILE *dest, char *new_data)   //function defineation to write new data into destination file, returns 1 on success, -1 on failure
{
    unsigned char frame_data[4]; // buffer to read frame size
    if (fread(frame_data, 1, 4, scr) != 4) // read frame size from source file
    {
        fprintf(stderr, "Failed to read frame size (EOF or corrupted file)\n");
        return -1;
    }
    unsigned int old_size = (frame_data[0] << 24) | (frame_data[1] << 16) | (frame_data[2] << 8) | frame_data[3]; // calculate old size from bytes

    unsigned int new_size = strlen(new_data) + 1;// encoding(\0) + text

    unsigned char new_size_bytes[4] = {(new_size >> 24) & 0xFF, (new_size >> 16) & 0xFF, (new_size >> 8) & 0xFF, new_size & 0xFF}; // convert new size to bytes

    fwrite(new_size_bytes, 1, 4, dest); // write new size into new file 

    unsigned char flags[2];
    fread(flags, 1, 2, scr); //read flag form old file
    fwrite(flags, 1, 2, dest); // copy 2 bytes of frame flags

    fseek(scr, old_size, SEEK_CUR); //skip the old data

    fputc('\0', dest); //null terminator
    fwrite(new_data, 1, strlen(new_data), dest); //write new data

    return 1; //success
}

int edit_tag(const char *filename, const char *tag, char *new_data)  //function defineation and returns 0 on success, -1 on failure
{
    if(strlen(new_data) == 0)//check if new data is empty
    {
        fprintf(stderr, "New data is empty\n");
        return -1;
    }

    FILE *scr = fopen(filename, "rb");//open source file in read mode
    FILE *dest = fopen("new_file.mp3", "wb");//open destination file in write mode
    if (!scr || !dest) 
    {
        perror("Could not open scr/dest file");//print error message if file opening fails
        fclose(scr);
        fclose(dest);
        return -1;
    }

    unsigned char buffer[10];//buffer to read header
    fread(buffer, 10, 1, scr); //read header form scr file
    fwrite(buffer, 10, 1, dest);//write the header dest file

    long header_size = ((buffer[6] << 21) | (buffer[7] << 14) | (buffer[8] << 7) | buffer[9]) + 10; //calculate total header size (10 bytes of header + size from header)

    char *expected_frame = get_frame_id(tag); //get expected frame id based on user input tag
    if (!expected_frame) //check if the expected frame id is NULL (invalid tag)
    {
        printf("Invalid tag option\n");
        fclose(scr);
        fclose(dest);
        return -1;
    }

    while(ftell(scr) < header_size)//loop until we reach the end of header
    {
        unsigned char frame_id[5];//buffer to read frame id
        fread(frame_id, 1, 4, scr);//read frame id from source file
        if(frame_id[0] == 0) //check if we have reached padding (frame id starts with 0 indicates padding)
        {
            break;
        }
        frame_id[4] = '\0';//null terminate the frame id

        if (strcmp(frame_id, expected_frame) == 0)//check if the current frame id matches the expected frame id
        {
            fwrite(frame_id, 1, 4, dest);//write the frame id to destination file
            if(opration_new_file(scr, dest, new_data))//call function to write new data into destination file
            {
                printf("Frame %s updated successfully.\n", expected_frame); //print success message
            }     
        }
        else
        {
            fwrite(frame_id, 1, 4, dest); //if tag does not match write the frame id to destination file

            unsigned char size_buf[4]; //buffer to read frame size
            fread(size_buf, 1, 4, scr); //read frame size from source file
            fwrite(size_buf, 1, 4, dest); //  write frame size to destination file

            unsigned int size = (size_buf[0] << 24) | (size_buf[1] << 16) | (size_buf[2] << 8)  | size_buf[3]; //calculate frame size from bytes

            unsigned char flags[2]; // buffer to read frame flags
            fread(flags, 1, 2, scr); //read frame flags from source file
            fwrite(flags, 1, 2, dest); //write frame flags to destination file

            unsigned char *buf = malloc(size); //buffer to read frame data
             if (!buf) 
             {
                fprintf(stderr, "Memory allocation failed\n");
                fclose(scr);
                fclose(dest);
                return -1;
            }
            fread(buf, 1, size, scr); //read frame data from source file
            fwrite(buf, 1, size, dest); //write frame data to destination file
            free(buf); //free the buffer after use
        }
    }
    
    unsigned char buffer1[1024 * 64];  //buffer to read remaining data in chunks of 64KB it's is fast and efficient for large files
    size_t bytes; // unsigned long to store number of bytes read
    while ((bytes = fread(buffer1, 1, sizeof(buffer1), scr)) > 0) //read remaining data from source file
    {
        fwrite(buffer1, 1, bytes, dest); //write remaining data to destination file
    }
    
    fclose(scr); //close source file
    fclose(dest); //close destination file
    remove(filename); //remove original file
    rename("new_file.mp3", filename); //rename new file to original file name
    return 0;
}