#include <stdio.h>
#include <string.h>
#include <expat.h>
#include <ctype.h>
#include "options.h"
#define BUFSIZE 8192

char buf[BUFSIZE];
char content[BUFSIZE];
//FILE *output;
//int mode;


struct
{
    char query[128];
    char access;
} state;


int main(int argc, char **argv){
    if (argc==1)
    {
        fprintf(stderr,"Usage:\n");
        fprintf(stderr,"\t./parsing -option <argument>\n\n");
        fprintf(stderr, "-h or help to see options\n");
        exit(1);
    }
    else{
        for(int i =0; i<argc; i++){
            if(strncmp(argv[i], "-h", 2)==0 || strncmp(argv[i], "help", 4)==0 ){
              help();
            }
            if(strncmp(argv[i], "-m", 2)==0){
                //faut que ca retourne un char ici pour que le mode soit choisi;
            }
            if(strncmp(argv[i], "-o", 2)==0){
                //récupération de l'output
            }
            if(strncmp(argv[i],"-d", 2)==0){
                //affichage d'un exemple de csv
            }
            if(strncmp(argv[i], "-c", 2)==0){
                //affichage du résultat sur la console.
            }

            //printf("l'argument est %u %s\n", i, argv[i]);
        }
    }
    return 0;
}