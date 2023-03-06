#include "extractMotifs.h"
#include "parameters.h"

char *getMotifs(char *file){
    char *line=strtok(strdup(file),"\n");
    char *createJSON =(char *)malloc(sizeof(char*)*MAX_SIZE);
    strcpy(createJSON,"\t\t\"motifs\":[\n");
    line=strtok(NULL, "\n");
    while(line!=NULL){
         char pdebut[MIN_SIZE],pfin[MIN_SIZE], motifs[MIN_SIZE], score[MIN_SIZE];
        sscanf(line, "	 %[^ -] -   %[^	:] : %[^	  :] 	:%s",pdebut, pfin,motifs, score);
        strcat(createJSON,"\t\t {\n\t\t\t\"position\":\"");
        strcat(createJSON,pdebut);
        strcat(createJSON,"-");
        strcat(createJSON,pfin);
        strcat(createJSON,"\",\n\t\t\t\"motif\":\"");
        strcat(createJSON,motifs);
        strcat(createJSON,"\",\n\t\t\t\"score\":\"");
        strcat(createJSON,score);
        strcat(createJSON,"\"\n\t\t },");  
        line=strtok(NULL, "\n"); 
    }
    createJSON[strlen(createJSON)-1]=']';
    return createJSON;
}