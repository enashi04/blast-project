#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char taxoID[128], parentID[128], ranks[128], espece[128];

int main() {
    char *buffer;
    long fileSize;
    FILE *file = fopen("taxonomy.reduced", "r");

    if (!file) {
        printf("Unable to open file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    buffer = (char *) malloc((fileSize + 1) * sizeof(char));
    fread(buffer, sizeof(char), fileSize, file);

    buffer[fileSize] = '\0';

    //printf("%s", buffer);
    char *line = strtok(buffer, "\n");
    while (line != NULL) {
        char id[255];
        char name[255];
        char rank[255];
        char value[255];
        sscanf(line, "%s    %s  %s  %s\n", id, name, rank,value);
        printf("id: %s, name: %s, type: %s, value: %s\n", id, name, rank, value);
        line = strtok(NULL, "\n");
    }

    free(buffer);
    fclose(file);

    return 0;
}
