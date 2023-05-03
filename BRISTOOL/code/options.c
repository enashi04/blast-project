#include "options.h"
#include "log.h"


/// @brief Display help 
/// @param argv 
void help(char **argv)
{
    if (strncmp(argv[1], "-h", 2) == 0 || strncmp(argv[1], "help", 4) == 0)
    {
        INFO("BRISTOOL\n");
        INFO("Usage:");
        INFO("\r -h \t Show this screen.");
        INFO("\r -m \t Choose mode [bronze, silver or gold].");
        INFO("\r -o \t Put an output file");
        INFO("Example of execution:");
        INFO("./parsing -m bronze -o output.json stdin.xml");
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
                ERROR("You need to put an argument after -m.\n");
                ERROR("Please choose a mode between : bronze, silver and gold;\n");
                exit(1);
            }
            else
            {
                if (strcmp("bronze", argv[i + 1]) == 0)
                {
                    INFO("Mode bronze choosen");
                    name = argv[i + 1];
                }
                else if (strcmp("silver", argv[i + 1]) == 0)
                {
                    INFO("Mode silver choosen");
                    return name = argv[i + 1];
                }
                else if (strcmp("gold", argv[i + 1]) == 0)
                {
                    INFO("Mode gold choosen");
                    return name = argv[i + 1];
                }
                else
                {
                    ERROR("You need to put an argument after -m.\n");
                    ERROR("Please choose a mode between : bronze, silver and gold;\n");
                    exit(1);
                }
            }
        }
        else{
            name = "bronze";
            INFO("The mode will be : Bronze");
            return name;

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
                    INFO("The output name is %s", name);
                }
                else
                {
                    ERROR("The output name is not valid. You need to add the json extension\n");
                    ERROR("Like output.json\n");
                    exit(1);
                }
            }
            else
            {
                ERROR("The output name is not valid. You need to add the json extension\n");
                ERROR("Like output.json\n");
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
                ERROR("The option %s is invalid.\n", argv[i]);
                ERROR("See ./parsing -h for more help.\n");
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
                FATAL("Can't open blast file'");
                exit(1);
            }
            return inputName;
        }
        else if (strcmp(inputName, ".json") == 0)
        {
            WARNING("The input file is a json file. The program will convert it to xml.\n");
            return inputName = "stdin.xml";
        }
        else
        {
            FATAL("The extension is not right. Please put a xml file.\n");
            exit(1);
        }
    }
    else
    {
        WARNING("You didn't put an input file. The input will be : stdin.xml\n");
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
            results = 1;
        }
        else
        {
            results = 0;
        }
    }
    return results;
}

void noArgument(int argc){
    if (argc == 1)
    {
        FATAL("You need to put an argument.\n");
        FATAL("See ./parsing -h for more help.\n");
        exit(1);
    }

}