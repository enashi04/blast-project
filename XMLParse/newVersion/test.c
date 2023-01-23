#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *file;

char *buffer;

char *makebuffer()
{
    long fileSize;

    if (!file)
    {
        printf("Unable to open file");
        exit(1);
    }
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    buffer = (char *)malloc((fileSize + 1) * sizeof(char));
    fread(buffer, sizeof(char), fileSize, file);

    buffer[fileSize] = '\0';
    return buffer;
}

char *retrieveParent(char *speciesID, char *lineage)
{
    char *line = strtok(strdup(buffer), "\n");
    char taxID[128], name[128], rank[128], parentID[128];
    char *lignee = (char *)malloc(2058);
    // on revient au début du fichier
    while (line != NULL)
    {
        sscanf(line, "%[^	]	%[^	]	%[^	]	%[^\n]", taxID, name, rank, parentID);
        if (strcmp(speciesID, taxID) == 0)
        {
            strcat(lignee, name);
            strcat(lignee, "/");
            strcat(lignee, lineage);
            strcpy(lineage, lignee);
            retrieveParent(parentID, lineage);
        }
        line = strtok(NULL, "\n");
    }
    return lineage;
}

char *readTaxo(char species_name[255])
{
    char *line = strtok(strdup(buffer), "\n");

    char taxID[128], name[128], rank[128], parentID[128];
    char *lineage = (char *)malloc(1024);
    char *finalResult = (char *)malloc(1024);

    while (line != NULL)
    {
        sscanf(line, "%[^	]	%[^	]	%[^	]	%[^\n]", taxID, name, rank, parentID);
        // printf("on rentre ici \n");
        if (strcmp(name, species_name) == 0)
        {
            // printf("name is %s\n", name);
            if (strcmp(rank, "superkingdom") == 0)
            {
                finalResult = name;
                break;
            }
            else
            {
                strcpy(lineage, name);
                finalResult = retrieveParent(parentID, lineage);
                // break;
            }
        }
        line = strtok(NULL, "\n");
    }
    return finalResult;
}

int main(int argc, char **argv)
{
    file = fopen("taxonomy.reduced", "r");
    makebuffer();
    printf("La lignee est %s\n", readTaxo("Mycobacterium tuberculosis"));
    fclose(file);
}

//     FILE *taxo_file;
//     if ((taxo_file = fopen("taxonomy.reduced", "r")) == NULL)
//     {
//         fprintf(stderr, "Can't open taxo file");
//     }
//     char l[1024];
//     if (fgets(l, 8192, taxo_file) == NULL)
//     {
//         fprintf(stderr, "Empty taxo file");
//     }
//     printf("La lignee est %s\n", readTaxo(taxo_file, "Mycobacterium tuberculosis", NULL));
//     return 0;
// }

// char *retrieveParent(FILE *taxo_file, char *speciesID, char *lineage)
// {

//     char l[1024];
//     char taxID[128], name[128], rank[128], parentID[128];
//     char *lignee = (char *)malloc(1024);
//     //on revient au début du fichier
//     rewind(taxo_file);
//     while (fgets(l, 8192, taxo_file) != NULL)
//     {
//         sscanf(l, "%[^	]	%[^	]	%[^	]	%[^\n]", taxID, name, rank, parentID);
//         if (strcmp(speciesID, taxID) == 0)
//         {
//             strcat(lignee, name);
//             strcat(lignee,"/");
//             strcat(lignee, lineage);

//             strcpy(lineage, lignee);
//             retrieveParent(taxo_file, parentID, lineage);
//         }
//     }

//     return lineage;
//     //return lignee;
// }

// char *readTaxo(FILE *taxo_file, char *species_name, char *speciesID)
// {
//     char taxID[128], name[128], rank[128], parentID[128], l[1024];
//     char *lineage = (char *)malloc(1024);
//     char *finalResult = (char *)malloc(1024);
//     // récupération de la premiere ligne

//     while (fgets(l, 8192, taxo_file) != NULL)
//     {
//         sscanf(l, "%[^	]	%[^	]	%[^	]	%[^\n]", taxID, name, rank, parentID);
//         //printf("name :%s\n", name);
//         if (strcmp(name, species_name) == 0)
//         {
//             if(strcmp(rank, "superkingdom")==0){
//                 finalResult=name;
//                 break;
//             }
//             else{
//                 strcpy(lineage, name);
//                 printf("on est là y le parent id est %s\n", parentID);
//                 // //récupération du parent puis on cherche le parent à l'aide de son taxid
//                 finalResult = retrieveParent(taxo_file, parentID,lineage);
//                 // break;
//             }

//         }
//     }

//     return finalResult;
// }
