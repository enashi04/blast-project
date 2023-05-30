#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "log.h"
#include "parameters.h"

// char *makebuffer(char *filename){
//     char *buffer=(char *)malloc(sizeof(char)*MAX_SIZE);
//     FILE *file =fopen(filename,"r");
//     if(!file){
//         FATAL("Unable to open file %s", filename);
//         exit(1);
//     }
//     //parcourir le fichier file
//     char ligne[MAX_SIZE];
//     INFO("%s\n",fgets(ligne,MAX_SIZE,file));
//     strcpy(buffer,fgets(ligne,MAX_SIZE,file));

//     while(fgets(ligne,MAX_SIZE,file)!=NULL){
//         //reallocation du buffer
//         buffer=(char *)realloc(buffer,sizeof(char)*(strlen(buffer)+strlen(ligne)+1));
//         strcat(buffer,ligne);
//     }
//     return buffer;
// }

// int main(int argc, char *argv[]){
//     char *buffer = makebuffer(argv[1]);
//     printf("%s",buffer);
//     free(buffer);
//     return 0;
//}
char *makebuffer(char *filename){
    
    char *buffer=NULL;
    // Open the taxonomy file
    FILE *file = fopen(filename, "r");
    // check of the file existence
    if (!file)
    {
        FATAL("Unable to open file %s",filename);
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
    buffer[fileSize-1] = '\0';   
    //find the last occurence of \n
    
    char *last_line =strrchr(buffer, '\n');

    if(last_line != NULL){
        char id_species[MIN_SIZE];
        sscanf(last_line, "%[^	] %*[^	]", id_species);
        SPECIES_SIZE = atoi(id_species);
        INFO("SPECIES_SIZE %d", SPECIES_SIZE);
    }
    // close the file
    fclose(file);

    return buffer;
}

  