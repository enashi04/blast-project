#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parameters.h"

int main(int argc, char *argv[])
{

    FILE *fichierXML = fopen("doubleBlast.xml", "r");
    if(fichierXML ==NULL){
        fprintf("le fichier n'existe pas");
        exit(1);
    }

    //parcours du fichier XML
    //ouvrir le fichier XML
    //parcours du fichier XML
    //d'abord on récupère les informations de blast : version | référence | db

        //-> pour chaque Iteration_iter_num, on crée un fichier blastp out + i, où i est le numéro de l'itération, avec les éléments qu'on aura récup et mis dans un char *
        //récupérer ensuite la query-def en entrant dans le BlastOutput_iterations puis iteration et une fois 
        //une fois arrivé dans les stats on ferme le fichier.
}