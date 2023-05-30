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

    //create a table
    char tabInfo[14][2][20];
    strcpy(tabInfo[0][0], "Lineage"); // L
    strcpy(tabInfo[1][0], "Hsp_identity"); //I
    strcpy(tabInfo[2][0], "Hsp_align-len"); //A
    strcpy(tabInfo[3][0], "Hsp_gaps"); //G
    strcpy(tabInfo[4][0], "Hsp_query-from"); //Q
    strcpy(tabInfo[5][0], "Hsp_query-to"); //Q
    strcpy(tabInfo[6][0], "Hsp_hit-from"); //H
    strcpy(tabInfo[7][0], "Hsp_hit-to"); //H
    strcpy(tabInfo[8][0], "Hsp_positive"); //P
    strcpy(tabInfo[9][0], "Hsp_evalue"); //E
    strcpy(tabInfo[10][0], "Hsp_score"); //S
    strcpy(tabInfo[11][0], "Hsp_bit-score"); //B
    strcpy(tabInfo[12][0], "Query_cover"); //C
    strcpy(tabInfo[13][0], "Hsp_num"); //N

    noArgument(argc);
    help(argv);

    char *inputName =inputRecovery(argc, argv);

    char *mode = modeChoice(argc, argv, tabInfo);
  
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

    char *fichiertaxo = getenv("TAXOFILE"); 
    if(fichiertaxo!=NULL){
        INFO("Taxonomy file found");
    }
    else{
        ERROR("Taxonomy file not found");
        return 1;
    }
    char *buffer = makebuffer(fichiertaxo);

    INFO("Writing in progress...");
    blastOutPut_iteration(f, mode, buffer, tabInfo); 
    INFO("Writing done");
}