#include "options.h"
#include "retrieveInfo.h"
#include "lineage.h"
#include "XMLtoBLASTP.h"
#include "parameters.h"

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

    fprintf(output, "\t]\n}");
    //XMLBLAST
    xmlDoc *xmlfile = xmlReadFile("stdin.xml", NULL, 0);
    FILE *output1 = fopen("output.blastp", "w");
    xmlNode  *root= xmlDocGetRootElement(xmlfile),*child = root->children;
    blastInfo(xmlfile, output1, child);
    //faire appel au ballast

    //Récupération des motifs

    //bingo fin! 

}