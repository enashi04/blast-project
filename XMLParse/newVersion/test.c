#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parameters.h"

char *makebuffer(char *filename)
{
    char *buffer;
    // Open the taxonomy file
    FILE *file = fopen(filename, "r");
    // check of the file existence
    if (!file)
    {
        printf("Unable to open file");
        exit(1);
    }
    // move to the end of the file
    fseek(file, 0, SEEK_END);
    // get the length of the file
    long fileSize = ftell(file);
    // return to the beginning of the file
    rewind(file);
    // initialize the buffer
    buffer = (char *)malloc((fileSize + 1) * sizeof(char));
    // fill the buffer with the contents of the file
    fread(buffer, sizeof(char), fileSize, file);
    // don't forget to put the '\0' at the end of the buffer
    buffer[fileSize] = '\0';
    return buffer;
}


int main(int argc, char **argv)
{
    char *buffer = makebuffer(FICHIER);
    char *taxID = getQuerySpeciesId("Mycobacterium tuberculosis (strain ATCC 25618 / H37Rv)", buffer);
    printf("l'id est%s\n", taxID);
    return 0;
}
