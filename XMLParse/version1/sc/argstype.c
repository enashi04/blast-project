#include "argstype.h"

int moreThanTwoArgs()
{
    printf("You need to put 0 or 2 arguments\n");
    printf("0 argument: The program will parse stdin.xml on three differents modes\n");
    printf("2 arguments\targ1 : The program will parse either the number of XMLfiles or the XMLfile\n");
    printf("\t\targ2 : Mode choice: 1 (Bronze) | 2 (Silver) | 3 (Gold) | 4 (Personal)\n");
    return 1;
}

int twoArgs(char **argv)
{ 
    FILE *fp = NULL;
   // DIR *dr=opendir("../results1");
    char *next, *next1;
    long value = strtol(argv[1], &next, 10);
    long value1 = strtol(argv[2], &next1, 10);
    /**************************************************************************************/
    /*************Verifier que les éléments passés en paramètres sont bons*****************/
    /**************************************************************************************/

    /**************************************************************************************/
    /**************************Si le 1e argument est un string*****************************/
    /**************************************************************************************/
    if (next == argv[1] || (*next != '\0'))
    {
    /**************************************************************************************/
    /**********************Si ce n'est pas un fichier xml mais un chiffre******************/
    /**************************************************************************************/
        char *compare;
        compare = strstr(argv[1], ".xml");
        if (compare == NULL)
        {
            printf("it's not a XML file\n");
            return 1;
        }
    /**************************************************************************************/
    /***********************Si le 2e argument n'est pas un chiffre*************************/
    /**************************************************************************************/
        if (next1 == argv[2] || (*next1 != '\0'))
        {
            printf("You must specify a number between 1 and 3\n");
            return 1;
        }

    /**************************************************************************************/
    /*************************Parser le fichier xml passé en param*************************/
    /**************************************************************************************/
        xmlDoc *f = xmlReadFile(argv[1], NULL, 0);
    /**************************************************************************************/
    /******************************Si le fichier est vide**********************************/
    /**************************************************************************************/
        if (f == NULL)
        {
            printf("File is empty\n");
            return 1;
        }
    /**************************************************************************************/
    /****************************On va supprimer le .xml pour******************************/
    /*************************avoir un nouveau nom de fichier avec*************************/
    /**************************************************************************************/
        char *name = argv[1];
        for (int i = 0; i < strlen(name); i++)
        {
            if (i >= strlen(name) - 4)
            {
                name[i] = '\0';
            }
            
        }
        if(strncmp (name,"../../",6) == 0){
            memmove(name, name+6, strlen(name));
        }
        else if (strncmp(name, "../", 3)==0){
            memmove(name, name+3, strlen(name));
        }
       
        char *filename= (char *)malloc(100);
        //printf("name is %s et %u", name, atoi( argv[2]));
        switch(atoi(argv[2])){
            case 1: 
                strcpy(filename, "../results/BronzeResults_");
                strcat(strcat(filename, name), ".csv");
                printf("je suis le filename %s\n",filename);
                modechoice(1, f, fp, filename, name);                
                break;
            case 2:
                strcpy(filename, "../results/SilverResults_");
                strcat(strcat(filename, name), ".csv");
                printf("je suis le filename %s\n",filename);
                modechoice(2, f, fp, filename, name);    
                break;
            case 3:
                strcpy(filename, "../results/GoldResults_");
                strcat(strcat(filename, name), ".csv");
                printf("je suis le filename %s\n",filename);
                modechoice(3, f, fp, filename, name);    
                break;
            default:
                break;
        }

    }
    /**************************************************************************************/
    /********************Accéder au fichier contenant les fichiers*************************/
    /****************************************XML*******************************************/
    /**************************************************************************************/
    else
    {
        printf("It's a number\n");
        // accès au fichier et on le parse solo
    }
    return 0;
}

int noArgument()
{
    FILE *fp = NULL;
    printf("Pas de fichier en argument ...On analysera stdin.xml\n");
    xmlDoc *f = xmlReadFile("../XMLFiles/stdin.xml", NULL, 0);
    if (f == NULL)
    {
        printf("Le fichier est vide\n");
        return 1;
    }

    modechoice(1, f, fp, "../results/BronzeResults_stdin.csv", "stdin");
    modechoice(2, f, fp, "../results/SilverResults_stdin.csv", "stdin");
    modechoice(3, f, fp, "../results/GoldResults_stdin.csv", "stdin");

    return 0;
}

void modechoice(int mode, xmlDoc *fichier, FILE *fp, char *filename, char *name)
{

    switch (mode)
    {
        /***************************************************************************************************/
        /******************************************BRONZE***************************************************/
        /***************************************************************************************************/
    case 1:
        printf("on entre ici.");
        fp = fopen(filename, "w");
        fprintf(fp, "Bronze mode for %s\n\n", name);
        blastOutPut_iteration(fichier, fp, 1);
        printf("You chose the Bronze mode\nYou can open the file : %s\n", filename);
        break;
        /***************************************************************************************************/
        /******************************************ARGENT***************************************************/
        /***************************************************************************************************/
    case 2:
        fp = fopen(filename, "w");
        fprintf(fp, "Silver mode for %s\n\n", name);

        blastOutPut_iteration(fichier, fp, 2);
        printf("You chose the Silver mode\nYou can open the file : %s\n", filename);
        break;
        /***************************************************************************************************/
        /******************************************GOLD*****************************************************/
        /***************************************************************************************************/
    case 3:
        fp = fopen(filename, "w");
        fprintf(fp, "Gold mode for %s\n\n", name);
        blastOutPut_iteration(fichier, fp, 3);
        printf("You chose the Gold mode\nYou can open the file :%s\n", filename);

        break;
    default:
        break;
    }
}

char *substr(const char *src, int m, int n)
{
    int len = n - m;
    char *dest = (char *)malloc(sizeof(char) * (len + 1));
    strncpy(dest, (src + m), len);
    return dest;
}