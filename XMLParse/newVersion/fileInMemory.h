#include <stdlib.h>
#include <string.h>
#include <stdio.h>


char *makebuffer(FILE *file)
{
    char *buffer;
    long fileSize;

  
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    buffer = (char *)malloc((fileSize + 1) * sizeof(char));
    fread(buffer, sizeof(char), fileSize, file);

    buffer[fileSize] = '\0';
    return buffer;
};