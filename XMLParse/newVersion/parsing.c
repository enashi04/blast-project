#include "options.h"
#include "retrieveInfo.h"
#include "lineage.h"
#include "XMLtoBLASTP.h"
#include "parameters.h"
#include "extractMotifs.h"

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

    blastOutPut_iteration(f, mode, buffer);

    // //fprintf(output, "\t],\n");
    // if(strcmp(mode, "gold")==0){
    //     //XMLBLAST
    //     printf("Here we go");
    //     xmlDoc *xmlfile = xmlReadFile("stdin.xml", NULL, 0);
    //     FILE *output1 = fopen("output.blastp", "w");
    //     xmlNode  *root= xmlDocGetRootElement(xmlfile),*child = root->children;
    //     blastInfo(xmlfile, output1, child);
    //      //faire appel au ballast
    //     system("../../BAL-LAST/code/./ballast -p output.blastp -o outputTest");
        
    // // //Récupération des motifs depuis le fichier puis l'afficher dans le fichier initial
    //     char *blastFile = makebuffer("outputTest.motifs");
    //     char *extractmotif=getMotifs(blastFile);
    //     fprintf(output,"%s\n\t\t}\n\t]\n}",extractmotif);
    // }
 

}