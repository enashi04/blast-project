#include "options.h"
#include "retrieveInfo.h"
#include "lineage.h"
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
    //ajout d'un  char pour tout mettre dedans et à la fin on copie tout dans le fichier.
    blastOutPut_iteration(f, mode, buffer);

    fprintf(output, "\t]\n}");

}