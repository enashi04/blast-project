#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "parameters.h"

char *makebuffer(char *filename)
{
    char *buffer;
    // Open the taxonomy file
    FILE *file = fopen(filename, "r");
    // check of the file existence
    if (!file)
    {
        printf("Unable to open file");
        exit(1);
    }
    // move to the end of the file
    fseek(file, 0, SEEK_END);
    // get the length of the file
    long fileSize = ftell(file);
    // return to the beginning of the file
    rewind(file);
    // initialize the buffer
    buffer = (char *)malloc((fileSize + 1) * sizeof(char));
    // fill the buffer with the contents of the file
    fread(buffer, sizeof(char), fileSize, file);
    // don't forget to put the '\0' at the end of the buffer
    buffer[fileSize] = '\0';
    return buffer;
}

// int main(int argc, char **argv)
// {
//     char *buffer = makebuffer(FICHIER);

//     char *line = strtok(strdup(buffer), "\n");
//     char id[256], name[256]; //, rank[256], parent[256];
//     while (line != NULL)
//     {
//         int len = strlen(line);
//         int compte = 0;
//         char parent[256], rank[256];
//         // printf("le rang est %s\n", rank);
//         sscanf(line, "%[^	] %*[^	] %[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^\n]", id, name);
//         // récupération du parent
//         for (int i = 0; i < len + 1; i++)
//         {
//             if (line[i] == '	')
//             {
//                 compte++;
//             }
//             if (compte == 9)
//             {
//                 int k = 0;
//                 for (int j = i + 1; j < len; j++)
//                 {
//                     if (line[j] == '	')
//                     {
//                         parent[k] = '\0';
//                         break;
//                     }
//                     else
//                     {
//                         parent[k] = line[j];
//                     }
//                     k++;
//                 }
//                 break;
//             }
//         }
//         // récupération du rang
//         compte = 0;
//         for (int i = 0; i < len + 1; i++)
//         {
//             if (line[i] == '	')
//             {
//                 compte++;
//             }
//             if (compte == 8)
//             {
//                 int k = 0;
//                 for (int j = i + 1; j < len; j++)
//                 {
//                     if (line[j] == '	')
//                     {
//                         rank[k] = '\0';
//                         break;
//                     }
//                     else
//                     {
//                         rank[k] = line[j];
//                     }
//                     k++;
//                 }
//                 break;
//             }
//         }
//         printf("parent is %s %s %s %s\n", id, name, parent, rank);
//         line = strtok(NULL, "\n");
//     }
//     return 0;
// }

char *getParentName(char *id_parent_species, char *buffer)
{
    char *line = strtok(strdup(buffer), "\n");
    char id_species[MIN_SIZE], name_species[MIN_SIZE];

    while (line != NULL)
    {
        sscanf(line, "%[^	] %*[^	] %[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^\n]", id_species, name_species);
        if (strcmp(id_parent_species, id_species) == 0)
        {
            break;
        }
        line = strtok(NULL, "\n");
    }
    printf("name is %s\n", name_species);

    return strdup(name_species);
}

int main(int argc, char **argv){
    char *buffer = makebuffer(FICHIER);
    
    char *parent= getParentName("77643", buffer);
  
    printf("le parent est %s \n", parent);
    return 0;
}
