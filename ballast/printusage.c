#include "prototypes.h"
#include <string.h>

void printusage(char *program, char *path)
{
    FILE *helpfile;
    char *helpfilename;
    char line[256];

    /*** Determines help file name from program name and path      ***/
    helpfilename = (char *)malloc(strlen(path) + strlen(program) + 6);
    sprintf(helpfilename, "%s/%s.hlp", path, program);

    /*** Prints usage to stdin                                     ***/
    if ((helpfile = fopen(helpfilename, "r")))
    {
        while (strncmp(fgets(line, 256, helpfile), "USAGE", 5) != 0)
            ;
        fgets(line, 256, helpfile);
        printf("\n");
        while ((line[0] == ' ') || (line[0] == '\t') || (line[0] == '\n'))
        {
            printf("%s", line);
            fgets(line, 256, helpfile);
        }
        printf("\n");
        fclose(helpfile);
    }

    else /*** Unless it cannot find the help file                       ***/
    {
        printf("\n*** Error! Could not find %s help file in %s\n\n", program, path);
    }

    free(helpfilename);
}