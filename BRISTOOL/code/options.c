#include "options.h"
#include "log.h"
#include "parameters.h"


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
        INFO("\r Arguments for silver mode with -f: \n\t l : Lineage\n\t i : Identity\n\t a : Align-Length\n\t g : Gaps\n\t q : Query-from-to\n\t h : Hit-from-to\n\t p : Positive\n\t e : Evalue\n\t s : Score\n\t b : Bit-score\n\t c : Query-cover\n\t n : Hsp-num");
        INFO("\r -o \t Put an output file");
        INFO("\rExample of execution:");
        INFO("\r./bristool -m bronze -o output.json stdin.xml\n");
        exit(1);
    }
}


/// @brief Retrieve the mode name
/// @param argc 
/// @param argv 
/// @param name 
/// @return mode name
char *modeChoice(int argc, char **argv, char tabInfo[13][2][20] ) //rajouter le tableau en argument
{
    char *mode= "bronze";
    for(int i=0; i<13; i++){
        strcpy(tabInfo[i][1],"0"); 
    }
    
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-m") == 0)
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
                    for(int i=0; i<14; i++){
                        strcpy(tabInfo[i][1],"1"); 
                    }
                    return mode = argv[i + 1];
                }
                else if (strcmp("silver", argv[i + 1]) == 0)
                {
                    INFO("Mode silver choosen");
                    if(strcmp(argv[i +2],"-f")==0 && strchr(argv[i + 3], '.') == NULL){
                        int len = strlen(argv[i + 3]);
                        char argument[MAX_SIZE];
                        for(int j=0; j<len ; ++j){
                            if(argv[i + 3][j] == 'l'){ //lineage
                                strcpy(tabInfo[0][1],"1");
                                strcat(argument,"\n- Lineage");
                            }
                            if(argv[i + 3][j] == 'i'){ //identity
                                strcpy(tabInfo[1][1],"1");
                                strcat(argument,"\n- Identity");
                            }
                            if(argv[i + 3][j] == 'a'){ //align-len
                                strcpy(tabInfo[2][1],"1");
                                strcat(argument,"\n- Align-Length");
                            }
                            if(argv[i + 3][j] == 'g'){ //gaps
                                strcpy(tabInfo[3][1],"1");
                                strcat(argument,"\n- Gaps");
                            }
                            if(argv[i + 3][j] == 'q'){ //query-from/to
                                strcpy(tabInfo[4][1],"1");
                                strcpy(tabInfo[5][1],"1");
                                strcat(argument,"\n- Query-from\n- Query-to");
                            }
                            if(argv[i + 3][j] == 'h'){ //hit-from/to
                                strcpy(tabInfo[6][1],"1");
                                strcpy(tabInfo[7][1],"1");
                                strcat(argument,"\n- Hit-from\n- Hit-to");
                            }
                            if(argv[i + 3][j] == 'p'){ //positive
                                strcpy(tabInfo[8][1],"1");
                                strcat(argument,"\n- Positive");
                            }
                            if(argv[i + 3][j] == 'e'){ //evalue
                                strcpy(tabInfo[9][1],"1");
                                strcat(argument,"\n- E-value");
                            }
                            if(argv[i + 3][j] == 's'){ //score
                                strcpy(tabInfo[10][1],"1");
                                strcat(argument,"\n- Score");
                            }
                            if(argv[i + 3][j] == 'b'){//bitscore
                                strcpy(tabInfo[11][1],"1");
                                strcat(argument,"\n- Bit-score");
                            }
                            if(argv[i + 3][j] == 'c'){ //query-cover
                                strcpy(tabInfo[12][1],"1");
                                strcat(argument,"\n- Query-cover");
                            }
                        }
                        INFO("The following arguments were passed to the script: %s", argument);
                    }
                    else{
                        WARNING("You didn't specify any parameters for the silver mode. The default parameters will be used");
                    }
                    return mode = argv[i + 1];
                }
                else if (strcmp("gold", argv[i + 1]) == 0)
                {
                    INFO("Mode gold choosen");
                    for(int i=0; i<13; i++){
                        strcpy(tabInfo[i][1],"1");  
                    }
                    return mode = argv[i + 1];
                }
                else
                {
                    ERROR("You need to put an argument after -m.\n");
                    ERROR("Please choose a mode between : bronze, silver and gold;\n");
                    exit(1);
                }
            }
        }
        // else{
        //     mode = "bronze";
        //     INFO("The mode will be : Bronze");
        //     return mode;

        // }
    }
    return mode;
}

/// @brief 
/// @param argc 
/// @param argv 
/// @param name 
/// @return the outputName
char *outputName(int argc, char **argv)
{
    char *name=NULL;
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
            if (strncmp(argv[i], "-a", 2) != 0 && strncmp(argv[i], "-m", 2) != 0 && strncmp(argv[i], "-h", 2) != 0 && strncmp(argv[i], "-o", 2) != 0 && strncmp(argv[i], "-f", 2) != 0)
            {
                ERROR("The option %s is invalid.\n", argv[i]);
                ERROR("See ./bristool -h for more help.\n");
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
char *inputRecovery(int argc, char **argv)
{
    char *inputName;
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
        FATAL("See ./bristool -h for more help.\n");
        exit(1);
    }

}