#include "options.h"
#include "retrieveInfo.h"

int main(int argc, char **argv){
    //no argument
    noArgument(argc);
    //affichage du help
    help(argv);
    //ici on initialise déjà les variables input, mot et output.
    char *inputName = NULL;
    inputName = inputRecovery(argc, argv, inputName);
    // récupération des valeurs passées en arguments

    char *mode = '\0';
    mode = modeChoice(argc, argv, mode);
    char *outName = NULL;
    outName = outputName(argc, argv, outName);
    // si l'user met des options non valides
    invalidOptions(argc, argv);

   // FILE *f = fopen(inputName, "r");
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

   // createSpeciesTable();
   
    blastOutPut_iteration(f, mode);

    fprintf(output, "\t]\n}");

}