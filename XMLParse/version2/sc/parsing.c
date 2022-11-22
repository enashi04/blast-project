#include <ctype.h>
//#include <expat.h>
#include "options.h"
#include "tagParse.h" 


int main(int argc, char **argv)
{
    // si on n'a pas d'argument
    if (argc == 1)
    {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "\t./parsing -option <argument>\n\n");
        fprintf(stderr, "-h or help to see options\n");
        exit(1);
    }

    // Affichage du help si besoin
    help(argv);
    char *inputName = NULL;

    inputName = inputRecovery(argc, argv, inputName);
    // récupération des valeurs passées en arguments
    char *mode = '\0';
    mode = modeChoice(argc, argv, mode);
    char *outName = NULL;
    outName = outputName(argc, argv, outName);
    //printf("l'outname est %s\n", outName);
    // si l'user met des options non valides
    invalidOptions(argc, argv);
    // ouverture du fichier pour mettre le résultat
    FILE *f = fopen(inputName, "r");

    if (outName != NULL)
    {
        output = fopen(outName, "w");
    }
    else
    {
        output = stdout;
    }

    // récupérer le nom du fichier sans extension

    if (strcmp(mode, "bronze") == 0)
    {
        fprintf(stderr, "You chose the Bronze mode !\n");

        //fprintf(output, "Bronze results \n\n");
        fprintf(output, "id, num, score, evalue,align-length \n");

        XML_Parser parser = XML_ParserCreate(NULL);
        XML_SetElementHandler(parser, bronze_tag_start, tag_end);
        XML_SetCharacterDataHandler(parser, tag_value);

        state.access = 0;
        test_error(f, parser);
        if (outName != NULL)
        {
            fprintf(stderr, "You can open the file: %s\n", outName);
        }
    }
    else if (strcmp(mode, "silver") == 0)
    {
        fprintf(stdout, "You chose the Silver mode !\n");
        //fprintf(output, "Silver results\n\n");
        fprintf(output, "id,hit,score,evalue,identity,positive,gaps,align-length \n");

        // fprintf(stdout, "Silver results of %s\n\n", name);
        // fprintf(stdout, "id,hit,score,evalue,identity,positive,gaps,align-length \n");

        XML_Parser parser = XML_ParserCreate(NULL);
        XML_SetElementHandler(parser, silver_tag_start, tag_end);
        XML_SetCharacterDataHandler(parser, tag_value);

        state.access = 0;
        test_error(f, parser);

        if (outName != NULL)
        {
            fprintf(stderr, "You can open the file: %s\n", outName);
        }
    }
    else if (strcmp(mode, "gold") == 0)
    {
        fprintf(stderr, "You chose the Gold mode !\n");

        //fprintf(output, "Gold results\n\n");
        fprintf(output, "id,hit,bitscore,score,evalue,query-from,query-to,target-from,target-to,identity,positive,gaps,query-cover,align-length \n");

        // fprintf(stdout, "Gold results of %s\n\n", name);
        // fprintf(stdout, "id,hit,bitscore,score,evalue,query-from,query-to,target-from,target-to,identity,positive,gaps,align-length \n");

        XML_Parser parser = XML_ParserCreate(NULL);
        XML_SetElementHandler(parser, gold_tag_start, tag_end);
        XML_SetCharacterDataHandler(parser, tag_value);

        state.access = 0;
        test_error(f, parser);

        if (outName != NULL)
        {
            fprintf(stderr, "You can open the file: %s\n", outName);
        }
    }

    return 0;
}