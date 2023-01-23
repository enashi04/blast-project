#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *putFileInMemory(){
    char *buffer; 
    long fileSize;

    FILE *file = fopen("taxonomy.reduced", "r");

    if (!file) {
        printf("Unable to open file");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    buffer = (char *) malloc((fileSize + 1) * sizeof(char));
    fread(buffer, sizeof(char), fileSize, file);

    buffer[fileSize] = '\0';
    fclose(file);
    return buffer;
};