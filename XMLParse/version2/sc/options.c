#include "options.h"

// ici on met l'affichage de help avec l'argument -h ou help
void help(char **argv)
{
    if (strncmp(argv[1], "-h", 2) == 0 || strncmp(argv[1], "help", 4) == 0)
    {
        fprintf(stderr, "XMLParser\n\n");
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "\r -h \t Show this screen.\n");
        fprintf(stderr, "\r -m \t Choose mode [bronze, silver or gold].\n");
        fprintf(stderr, "\r -o \t Put an output file\n");
        fprintf(stderr, "\r -a \t Display CSV file example\n\n");        
        exit(1);
    }
}

// si y'a la présence du mode alors il faut qu'on voit l'argument d'apres. Si c'est bronze argent ou or.
char *modeChoice(int argc, char **argv, char *name)
{
    for (int i = 0; i < argc; i++)
    {
        if (strncmp(argv[i], "-m", 2) == 0)
        {
            // faut que ca retourne un char ici pour que le mode soit choisi;
            if (argv[i + 1] == NULL)
            {
                fprintf(stderr, "You need to put an argument after -m.\n");
                fprintf(stderr, "Please choose a mode between : bronze, silver and gold;\n");
                exit(1);
            }
            else {
                if (strcmp("bronze", argv[i + 1]) == 0)
                {
                     name = argv[i + 1];
                }
                else if (strcmp("silver", argv[i + 1]) == 0)
                {
                    return name = argv[i + 1];
                }
                else if (strcmp("gold", argv[i + 1]) == 0)
                {
                    return name = argv[i + 1];
                }
                else
                {
                    fprintf(stderr, "The mode argument is not valid.\n");
                    fprintf(stderr, "Please choose a mode between : bronze, silver and gold;\n");
                    exit(1);
                }
            }
        }
    }
    return name;
    
}

// fichier de sortie si y'a un -o
char *outputName(int argc, char **argv, char *name)
{
    char *extension;
    *name ='\0';
    for (int i = 0; i < argc; i++)
    {
        if (strncmp(argv[i], "-o", 2) == 0)
        {
            // récupération de l'output
            if(argv[i+1]!=NULL){
                if(strchr(argv[i+1], '.')!=NULL){
                    extension = strrchr(argv[i+1], '.');
                    printf("extension is %s\n", extension);
                    if (strcmp(extension, ".csv")==0)
                    {
                        name = argv[i+1];
                    }
                }
                else
                {
                    fprintf(stderr, "The output name is not valid. You need to add the csv extension\n");
                    fprintf(stderr, "Like output.csv\n");
                    exit(1);
                }
            }
            else{
                fprintf(stderr, "The output name is empty. Please add an output file\n");
                fprintf(stderr, "Like output.csv\n");
                exit(1);
            }
           
           
        }
    }
    fprintf(stderr,"le name est %s\n", name);
    return name;
}

int displayResults(int argc, char **argv){
    int results;
     for (int i = 0; i < argc; i++)
    {
        if (strncmp(argv[i], "-a", 2) == 0)
        
        {
            // affichage du résultat sur la console.
            results= 1;
        }
        else{
            results= 0;
        }
    }
    return results;
}

void invalidOptions(int argc, char **argv){
    for(int i =0; i<argc; i++){
        if(strncmp(argv[i], "-",1)==0){
            fprintf(stdout, "%s %u\n", argv[i], i);
            if(strncmp(argv[i], "-a", 2) != 0 && strncmp(argv[i], "-m", 2) != 0 && strncmp(argv[i], "-h", 2) != 0 && strncmp(argv[i], "-o", 2) != 0){
                fprintf(stderr,"The option %s is invalid.\n", argv[i]);
                fprintf(stderr,"See ./parsingv2 -h for more help.\n");
                exit(1);
            }
        } 
    }
}
