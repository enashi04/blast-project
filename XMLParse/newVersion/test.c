#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NUMCOLUMNS 3
#define NUMROWS 2147398 // nombre d'espèce
static char tableau[NUMROWS][NUMCOLUMNS][128];

char *retrieveParent(FILE *taxo_file, char *speciesID, char *lineage)
{

    char l[1024];
    char taxID[128], name[128], rank[128], parentID[128];
    char *lignee = (char *)malloc(1024);
    //on revient au début du fichier
    rewind(taxo_file);
    while (fgets(l, 8192, taxo_file) != NULL)
    {
        sscanf(l, "%[^	]	%[^	]	%[^	]	%[^\n]", taxID, name, rank, parentID);
        if (strcmp(speciesID, taxID) == 0)
        {
            strcat(lignee, name);
            strcat(lignee,"/");
            strcat(lignee, lineage);

            strcpy(lineage, lignee);
            retrieveParent(taxo_file, parentID, lineage);
        }
    }
    
    return lineage;
    //return lignee;    
}

char *readTaxo(FILE *taxo_file, char *species_name, char *speciesID)
{
    char taxID[128], name[128], rank[128], parentID[128], l[1024];
    char *lineage = (char *)malloc(1024);
    char *finalResult = (char *)malloc(1024);
    // récupération de la premiere ligne
 
    while (fgets(l, 8192, taxo_file) != NULL)
    {
        sscanf(l, "%[^	]	%[^	]	%[^	]	%[^\n]", taxID, name, rank, parentID);
        //printf("name :%s\n", name);
        if (strcmp(name, species_name) == 0)
        {
            if(strcmp(rank, "superkingdom")==0){
                finalResult=name;
                break;
            }
            else{
                strcpy(lineage, name);
                printf("on est là y le parent id est %s\n", parentID);
                // //récupération du parent puis on cherche le parent à l'aide de son taxid
                finalResult = retrieveParent(taxo_file, parentID,lineage);
                // break;
            }
           
        }
    }

    return finalResult;
}

char (*createTableau())[3][128]
{
    FILE *f = fopen("taxonomy.reduced", "r");
    char taxID[128], name[128], rank[128], parentID[128];
    char l[BUFSIZ];
    if (fgets(l, BUFSIZ, f) == NULL)
    {
        fprintf(stderr, "Empty taxo file");
    }
    int i = 1;
    rewind(f); //aller au début de la ligne
    while (fgets(l, BUFSIZ, f) != NULL)
    {
        sscanf(l, "%[^	]	%[^	]	%[^	]	%[^\n]", taxID, name, rank, parentID);
        for (int j = 0; j < 2147398 ; j++)
        {
            if (j == 0)
            {
                strcpy(tableau[i][j], name); // species
            }
            else if (j == 1)
            {
                strcpy(tableau[i][j], taxID);
            }
            else if (j == 2)
            {
                strcpy(tableau[i][j], rank);
            }
            else if (j == 3)
            {
                strcpy(tableau[i][j], parentID);
                //fprintf(stdout, "ici on doit avoir : %s\n", parentID);
            }
        }
        i++;
    }
    return tableau;
}

int main(int argc, char **argv)
{
    createTableau();
    for(int j =0;j<2147398 ; j++){
        fprintf(stdout,"%s %s %s %s\n", tableau[j][0], tableau[j][1], tableau[j][2], tableau[j][3]);
    }

    return 0;
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