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
        motifs[strlen(motifs)-3]='\0';
        pfin[strlen(pfin)-1]='\0';
        int entrySize = snprintf(NULL, 0, "{\"position\":\"%s-%s\",\"motif\":\"%s\",\"score\":%s},", pdebut, pfin, motifs, score) + 1;
        createJSONSize += entrySize;
        createJSON = realloc(createJSON, createJSONSize * sizeof(char));
        strcat(createJSON, "{\"position\":\"");
        strcat(createJSON, pdebut);
        strcat(createJSON, "-");
        strcat(createJSON, pfin);
        strcat(createJSON, "\",\"motif\":\"");
        strcat(createJSON, motifs);
        strcat(createJSON, "\",\"score\":");
        strcat(createJSON, score);
        strcat(createJSON, "},");
        line = strtok(NULL, "\n");
    }
    createJSON[createJSONSize - 3] = '\n';
    strcat(createJSON, "]");
    return createJSON;
}