#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

int main(int argc, char **argv)
{
    FILE *taxo_file;
    if ((taxo_file = fopen("taxonomy.reduced", "r")) == NULL)
    {
        fprintf(stderr, "Can't open taxo file");
    }
    char l[1024];
    if (fgets(l, 8192, taxo_file) == NULL)
    {
        fprintf(stderr, "Empty taxo file");
    }

    printf("La lignee est %s\n", readTaxo(taxo_file, "Azorhizobium caulinodans", NULL));
    return 0;
}