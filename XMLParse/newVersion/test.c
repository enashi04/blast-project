#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUMROWS 2147397
#define NUMCOLUMNS 4
#define BUFSIZE 8192


#define ROWS 3
#define COLS 5

/***********Remplir un tableau contenant les taxid et compagnie*************/
// char*** createSpeciesTable(){
//     FILE *f ;
//     char l[BUFSIZE];

//     if ((f=fopen("taxonomy.reduced", "r")) == NULL){
//         fprintf(stderr,"Can't open taxo file");
//     }
//     if (fgets(l, BUFSIZE, f) == NULL){
//         fprintf(stderr,"Empty taxo file");
//     }

//     // int i=0; //taxid de l'espèce et son dernier parent
//     char name[264], rank[128],taxID[64],parentID[64];

//     char ***tab;

//     tab = malloc(sizeof(int**) * NUMROWS);  // Allocate memory for the rows
//     for (int i = 0; i < NUMROWS; i++) {
//         tab[i] = malloc(sizeof(int*) * NUMCOLUMNS);  // Allocate memory for the columns
//     for (int j = 0; j < NUMCOLUMNS; j++) {
//         tab[i][j] = malloc(sizeof(int) * NUMCOLUMNS);  // Allocate memory for the depth
//     }
//     }
    
//     fprintf(stdout,"on est là ou non\n");
//     for(int i =0; i < NUMROWS; i++){
//         for(int j = 0; j < NUMCOLUMNS+1; j++){
//             sscanf(l, "%[^	]	%[^	]	%[^	]	%[^\n]", taxID, name, rank,parentID); for(int j=0; j<NUMCOLUMNS+1; j++)
//             if(j==0){
//                 tab[i][j] =name; //on va mettre le nom de l'espèce
//             }
//             else if(j==1){
//                 tab[i][j] = taxID; //on va mettre le taxID
//             }
//             else if(j==2){
//                 tab[i][j] = rank; //on va mettre son rang
//             }
//             else if(j==3){
//                 tab[i][j] =parentID; //on va mettre son parent
//             }
//         }
//         if(fgets(l,BUFSIZE,f) == NULL){
//                 break;
//         }
        
//     }
//     printf("test test test ");
    
//     for(int i = 0;i<NUMROWS; i++){
//         for(int j = 0;-j<NUMCOLUMNS+1; j++){
//             fprintf(stdout, "%s %u %u\n",tab[i][j], i, j);
//         }
//     }
//     fclose(f); 
//     return tab;
// }

// int main(int argc, char **argv){
//     char ***tab = createSpeciesTable();
// }

// int main(int argc, char **argv) {
    // char *array[ROWS][COLS];
    // int i, j;

    // for (i = 0; i < ROWS; i++) {
    //     for (j = 0; j < COLS; j++) {
    //         array[i][j] ="Element";
    //         printf("%s i = %u j = %u\n",array[i][j], i, j);
    //     }
    // }

    // FILE *f ;
    // char l[BUFSIZE];

    // if ((f=fopen("taxonomy.reduced", "r")) == NULL){
    //     fprintf(stderr,"Can't open taxo file");
    // }
    // if (fgets(l, BUFSIZE, f) == NULL){
    //     fprintf(stderr,"Empty taxo file");
    // }

    // int i=0; //taxid de l'espèce et son dernier parent
    // char name[264], rank[128],taxID[64],parentID[64];
    // printf("line is %s",l);
   // char *tab[NUMROWS][NUMCOLUMNS];

    // while(fgets(l, BUFSIZE, f)!=NULL){
        // if(sscanf(l, "%[^	]	%[^	]	%[^	]	%[^\n]", taxID, name, rank, parentID) != 4) {
        //     perror("Error parsing elements from string");
        //     return 1;
        //  }

        // sscanf(l, "%[^	]	%[^	]	%[^	]	%[^\n]", taxID, name, rank,parentID);
        // fprintf(stdout,"taxID = %s name= %s : genre = %s parent : %s\n", taxID,name, rank,parentID);
        //  for(int j=0; j<NUMCOLUMNS+1; j++){
        //     switch (j)
        //     {
        //         case 0: 
        //             tab[i][j] =name; //on va mettre le nom de l'espèce
        //             break;
        //         case 1: 
        //             tab[i][j] = taxID; //on va mettre le taxID
        //             break;
        //         case 2 : 
        //             tab[i][j] = rank; //on va mettre son rang
        //             break;
        //         case 3:  
        //             tab[i][j] =parentID; //on va mettre son parent
        //             break;
        //     }
        // }
       // i++;
   // }
//     fclose(f); 
//     return 0;
     
// }
char (*createTableau())[4][128] {
    FILE *f = fopen("taxonomy.reduced", "r");
    char taxID[128], name[128], rank[128], parentID[128];
    static char tableau[NUMROWS][NUMCOLUMNS][128];
    char l[BUFSIZE];
    if (fgets(l, BUFSIZE, f) == NULL){
        fprintf(stderr,"Empty taxo file");
    }
    int i =1;
    //on va d'abord remplir la première ligne !
    sscanf(l, "%[^	]	%[^	]	%[^	]	%[^\n]", taxID, name, rank,parentID);
   
    strcpy(tableau[0][0], name);
    strcpy(tableau[0][1], taxID);
    strcpy(tableau[0][2], rank);
    strcpy(tableau[0][3], parentID);

   while(fgets(l, BUFSIZE, f) != NULL){
    sscanf(l, "%[^	]	%[^	]	%[^	]	%[^\n]", taxID, name, rank,parentID);
        for(int j = 0; j < NUMCOLUMNS; j++){
            if(j==0){
                strcpy(tableau[i][j],name);
            }
            else if(j==1){
                strcpy(tableau[i][j],taxID);
            }
            else if(j==2){
                strcpy(tableau[i][j],rank);
            }
            else if(j==3){
                strcpy(tableau[i][j],parentID);
            }
        }
        i++;
    }
        

   return tableau;
}
int main(int argc, char **argv){
    char (*tableau)[4][128] = createTableau();

    for(int i=0; i<NUMROWS; i++){
        for(int j = 0; j < NUMCOLUMNS; j++){
            printf("value i=%u j=%u: %s\n",i,j, tableau[i][j]);
        }
    }
    return 0;
}