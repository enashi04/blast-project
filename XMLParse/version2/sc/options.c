#include "options.h"


/// @brief Display help 
/// @param argv 
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


/// @brief Retrieve the mode name
/// @param argc 
/// @param argv 
/// @param name 
/// @return mode name
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
            else
            {
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

/// @brief 
/// @param argc 
/// @param argv 
/// @param name 
/// @return the outputName
char *outputName(int argc, char **argv, char *name)
{
    char *extension;
    for (int i = 0; i < argc; i++)
    {
        if (strncmp(argv[i], "-o", 2) == 0)
        {
            // récupération de l'output
            if (argv[i + 1] != NULL)
            {
                if (strchr(argv[i + 1], '.') != NULL)
                {
                    extension = strrchr(argv[i + 1], '.');
                    // printf("extension is %s\n", extension);
                    if (strcmp(extension, ".json") == 0)
                    {
                        name = argv[i + 1];
                    }
                }
                else
                {
                    fprintf(stderr, "The output name is not valid. You need to add the json extension\n");
                    fprintf(stderr, "Like output.json\n");
                    exit(1);
                }
            }
            else
            {
                fprintf(stderr, "The output name is empty. Please add an output file\n");
                fprintf(stderr, "Like output.json\n");
                exit(1);
            }
        }
    }
    return name;
}

/// @brief Display error message if we put wrong options
/// @param argc 
/// @param argv 
void invalidOptions(int argc, char **argv)
{
    for (int i = 0; i < argc; i++)
    {
        if (strncmp(argv[i], "-", 1) == 0)
        {
            // fprintf(stdout, "%s %u\n", argv[i], i);
            if (strncmp(argv[i], "-a", 2) != 0 && strncmp(argv[i], "-m", 2) != 0 && strncmp(argv[i], "-h", 2) != 0 && strncmp(argv[i], "-o", 2) != 0)
            {
                fprintf(stderr, "The option %s is invalid.\n", argv[i]);
                fprintf(stderr, "See ./parsingv2 -h for more help.\n");
                exit(1);
            }
        }
    }
}

/// @brief 
/// @param argc 
/// @param argv 
/// @param inputName 
/// @return the name of the input
char *inputRecovery(int argc, char **argv, char *inputName)
{
    if (argv[argc - 1] != NULL && strchr(argv[argc - 1], '.'))
    {
        inputName = strrchr(argv[argc - 1], '.');

        if (strcmp(inputName, ".xml") == 0)
        {
            inputName = argv[argc - 1];
            FILE *f = fopen(inputName, "r");
            if (!f)
            {
                fprintf(stderr, "Can't open blast file\n");
                exit(1);
            }
            return inputName;
        }
        else if (strcmp(inputName, ".json") == 0)
        {
            printf("You didn't put an input file. The input will be : stdin.xml\n");
            return inputName = "stdin.xml";
        }
        else
        {
            fprintf(stderr, "the extension is not right\n");
            exit(1);
        }
    }
    else
    {
        printf("You didn't put an input file. The input will be : stdin.xml\n");
        return inputName = "stdin.xml";
    }
}

char *newname(char *name)
{
    for (size_t i = 0; i < strlen(name); i++)
    {
        if (i >= strlen(name) - 4)
        {
            name[i] = '\0';
        }
    }

    if (strncmp(name, "../", 3) == 0)
    {
        if (strncmp(name, "../XMLFiles/", 12) == 0)
        {
            memmove(name, name + 12, strlen(name));
        }
        else if (strncmp(name, "../../", 6) == 0)
        {
            memmove(name, name + 6, strlen(name));
        }
        else
        {
            memmove(name, name + 3, strlen(name));
        }
    }

    if (strncmp(name, "results/", 8) == 0)
    {
        memmove(name, name + 8, strlen(name));
    }
    return name;
}


int displayResults(int argc, char **argv)
{
    int results;
    for (int i = 0; i < argc; i++)
    {
        if (strncmp(argv[i], "-a", 2) == 0)

        {
            // affichage du résultat sur la console.
            results = 1;
        }
        else
        {
            results = 0;
        }
    }
    return results;
}
