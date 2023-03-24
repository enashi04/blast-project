#include "options.h"
#include "retrieveInfo.h"
#include "lineage.h"
#include "XMLtoBLASTP.h"
#include "parameters.h"
#include "extractMotifs.h"
<<<<<<< HEAD
#include "unistd.h"
#include <sys/wait.h>
=======
#include<unistd.h>
>>>>>>> 481466a682146874a098ded042caa384c41c0b67

int main(int argc, char **argv){

    noArgument(argc);
    help(argv);

    char *inputName = NULL;
    inputName = inputRecovery(argc, argv, inputName);

    char *mode = '\0';
    mode = modeChoice(argc, argv, mode);
  
    char *outName = NULL;
    outName = outputName(argc, argv, outName);
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
    //création d'un tableau
    char tabInfo[13][2][20];
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

 
    if(strcmp(mode, "silver")==0){
        for(int i=0; i<13; i++){
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
    }else{
        for(int i=0; i<13; i++){
            strcpy(tabInfo[i][1],"1");  
        }
    }
<<<<<<< HEAD


=======
    //mettre les fichiers outputblastp
    xmlNode *root =xmlDocGetRootElement(f), *child=root->children;
    char *database = (char*)malloc(sizeof(char));
    char blastInfo[MAXI_SIZE];
    strcpy(blastInfo,getInfoBlast(child, database));
    convertToBlastP(f, child, blastInfo, database);
    sleep(0.1);
>>>>>>> 481466a682146874a098ded042caa384c41c0b67
    blastOutPut_iteration(f, mode, buffer, tabInfo); //ajout d'un autre élément

    //supprimer les fichiers ensuite 

    // //fprintf(output, "\t],\n");
    // if(strcmp(mode, "gold")==0){
    //     //XMLBLAST
    //     printf("Here we go");
    //     xmlDoc *xmlfile = xmlReadFile("stdin.xml", NULL, 0);
    //     FILE *output1 = fopen("output.blastp", "w");
    //     xmlNode  *root= xmlDocGetRootElement(xmlfile),*child = root->children;
    //     convertToBlastP(xmlfile, output1, child);
    //      //faire appel au ballast
    //     system("../../BAL-LAST/code/./ballast -p output.blastp -o outputTest");
        
    // // //Récupération des motifs depuis le fichier puis l'afficher dans le fichier initial
    //     char *blastFile = makebuffer("outputTest.motifs");
     //   char *extractmotif=getMotifs(blastFile);
    //     fprintf(output,"%s\n\t\t}\n\t]\n}",extractmotif);
    // }
 
}