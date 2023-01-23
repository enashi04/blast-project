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

    printf("%s", buffer);
    char line[1024];
    for(int i =0; i<fileSize; i++){
        strcat(line, buffer[i]);
        if(strcmp((const char *)buffer[i], "\n")==0){
            sscanf(line, "%[^	]	%[^	]	%[^	]	%[^\n]", taxoID, espece, ranks, parentID);
            printf("%u : %s %s %s %s\n",i, taxoID, espece, ranks, parentID);
            strcpy(line, "\0");
        }
    }

    free(buffer);
    fclose(file);

    return 0;
}
