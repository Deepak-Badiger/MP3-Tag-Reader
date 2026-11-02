/*
Name        : Deepak Balappa Badiger

Date        : 19-06-2026.

Project     : Mp3 Tag Reader & Editor in C.

Description : Mp3 tag reader and editor application

This application allows users to view and edit ID3 tags of mp3 files.
It supports viewing tags such as title, artist, album, year, and genre,
as well as editing these tags with new information provided by the user.

mp3 Tag Reader & Editor is a command-line application developed in the C programming
language that enables users to efficiently read and modify ID3 tags in mp3 files.
The project demonstrates the use of file handling, string operations,
memory management, and modular programming concepts in C.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "reader.h"
#include "edit.h"
#include "header.h"
int main(int argc, char *argv[])
{
    // Validate command-line arguments count
    if (argc <= 1 || argc > 6)
    {
        display_error();
        return 1;
    }

    // Display help menu
    else if (strstr(argv[1], "--help") != 0)
    {
        help_menu();
        return 0;
    }

    // View MP3 tag information
    else if (strcmp(argv[1], "-v") == 0)
    {
        if (argc != 3)
        {
            display_error();
            return 1;
        }

        // Check whether MP3 file exists
        if (!check_file_exists(argv[2]))
        {
            printf("ERROR: Unable to locate file: %s\n", argv[2]);
            printf("Please provide a valid mp3 file name ! Only support mp3 files\n");
            return 1;
        }

        view_tags(argv[2]);
        return 0;
    }

    // Edit MP3 tag information
    else if (strcmp(argv[1], "-e") == 0)
    {
        if (argc != 5)
        {
            display_error();
            return 1;
        }

        char *tag = argv[2];
        char *new_data = argv[3];
        char *filename = argv[4];

        // Check whether MP3 file exists
        if (!check_file_exists(filename))
        {
            printf("ERROR: Unable to locate file: %s\n", filename);
            printf("Please provide a valid mp3 file name ! Only support mp3 files\n");
            return 1;
        }

        // Update selected tag with new data
        if (edit_tag(filename, tag, new_data) != 0)
        {
            perror("Failed to edit tag");
            return 1;
        }

        printf("Tag edited successfully.\n");
        return 0;
    }

    // Handle invalid options
    else
    {
        display_error();
        return 1;
    }

    return 0;
}
