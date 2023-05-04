#include "options.h"
#include "retrieveInfo.h"
// #include "lineage.h"
#include "XMLtoBLASTP.h"
#include "parameters.h"
#include "extractMotifs.h"
#include "log.h"
#include "makebuffer.h"
#include "unistd.h"
#include <sys/wait.h>

int main(int argc, char **argv){
    noArgument(argc);
    help(argv);

    char *inputName =inputRecovery(argc, argv);

    char *mode = modeChoice(argc, argv);
  
    char *outName = outputName(argc, argv);
    invalidOptions(argc, argv);

    xmlDoc *f = xmlReadFile(inputName, NULL,0);

    if (outName != NULL)
        output = fopen(outName, "w");
    else
        output = stdout;
    
    if (f == NULL)
    {
        printf("Le fichier est vide\n");
        return 1;
    }

    char *buffer = makebuffer(FICHIER);
    //création d'un tableau pour le mode silver
    char tabInfo[14][2][20];
    strcpy(tabInfo[0][0], "Lineage");
    strcpy(tabInfo[1][0], "Hsp_num");
    strcpy(tabInfo[2][0], "Hsp_identity");
    strcpy(tabInfo[3][0], "Hsp_align-len");
    strcpy(tabInfo[4][0], "Hsp_gaps");
    strcpy(tabInfo[5][0], "Hsp_query-from");
    strcpy(tabInfo[6][0], "Hsp_query-to");
    strcpy(tabInfo[7][0], "Hsp_hit-from");
    strcpy(tabInfo[8][0], "Hsp_hit-to");
    strcpy(tabInfo[9][0], "Hsp_positive");
    strcpy(tabInfo[10][0], "Hsp_evalue");
    strcpy(tabInfo[11][0], "Hsp_score");
    strcpy(tabInfo[12][0], "Hsp_bit-score");
    strcpy(tabInfo[13][0], "Query_cover");

    if(strcmp(mode, "silver")==0){
        printf("Which informations do you want to display?\n");
        for(int i=0; i<14; i++){
            char answer[2];
            printf("%s ? (y or n)", tabInfo[i][0]);
            scanf("%s", answer);
            if(strcmp(answer, "y")==0 || strcmp(answer, "Y")==0){
                strcpy(tabInfo[i][1],"1");
            }
            else{
                strcpy(tabInfo[i][1],"0");
            }
        }
        INFO("The following elements will be displayed:");
        for(int i=0; i<14; i++){
            if(strcmp(tabInfo[i][1],"1")==0){
                INFO("%s", tabInfo[i][0]);
            }
        }
    }else{
        for(int i=0; i<14; i++){
            strcpy(tabInfo[i][1],"1");  
        }
    }
    INFO("Writing in progress...");
    blastOutPut_iteration(f, mode, buffer, tabInfo); 
    fseek(output, -2, SEEK_END);
    fprintf(output,"\n}");
    INFO("Writing done");
}