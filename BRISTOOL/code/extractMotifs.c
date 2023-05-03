#include "extractMotifs.h"
#include "parameters.h"

char *getMotifs(char *file) {
    char *line = strtok(strdup(file), "\n");
    int createJSONSize = snprintf(NULL, 0, "\t\t\"motifs\":[\n") + 1;
    char *createJSON = malloc(createJSONSize * sizeof(char));
    strcpy(createJSON, "\t\t\"motifs\":[\n");
    line = strtok(NULL, "\n");
    while (line != NULL) {
        char pdebut[MIN_SIZE], pfin[MIN_SIZE], motifs[MIN_SIZE], score[MIN_SIZE];
        sscanf(line, " %[^ -] - %[^:]:%[^:]:%s", pdebut, pfin, motifs, score);
        int entrySize = snprintf(NULL, 0, "\t\t {\n\t\t\t\"position\":\"%s-%s\",\n\t\t\t\"motif\":\"%s\",\n\t\t\t\"score\":\"%s\"\n\t\t},\n", pdebut, pfin, motifs, score) + 1;
        createJSONSize += entrySize;
        createJSON = realloc(createJSON, createJSONSize * sizeof(char));
        strcat(createJSON, "\t\t {\n\t\t\t\"position\":\"");
        strcat(createJSON, pdebut);
        strcat(createJSON, "-");
        strcat(createJSON, pfin);
        strcat(createJSON, "\",\n\t\t\t\"motif\":\"");
        strcat(createJSON, motifs);
        strcat(createJSON, "\",\n\t\t\t\"score\":\"");
        strcat(createJSON, score);
        strcat(createJSON, "\"\n\t\t},\n");
        line = strtok(NULL, "\n");
    }
    createJSON[createJSONSize - 3] = '\n';
    strcat(createJSON, "\t\t]\n");
    return createJSON;
}