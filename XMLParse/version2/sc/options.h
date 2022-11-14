#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//ici on met l'affichage de help avec l'argument -h ou help
void help(){
    fprintf(stderr,"XMLParser\n\n");
    fprintf(stderr,"Usage:\n");
    fprintf(stderr,"\r -h \t Show this screen.\n");
    fprintf(stderr,"\r -m \t Choose mode [bronze, silver or gold].\n");
    fprintf(stderr,"\r -o \t Put an output file\n");
    fprintf(stderr,"\r -a \t Display CSV file example\n\n");
    
};

//si y'a la présence du mode alors il faut qu'on voit l'argument d'apres. Si c'est bronze argent ou or.
char* modeChoice(char *argv) {
    if(strcmp("bronze", argv)==0){
        return argv;
    }
    else if(strcmp("silver", argv)==0){
        return argv;
    }
    else if(strcmp("gold", argv)==0){
        return argv;
    }
    else{
        fprintf(stderr,"The mode argument is not valid.\n");
        fprintf(stderr,"Please choose a mode between : bronze, silver and gold;\n");
        exit(1);
    }
};

 //fichier de sortie si y'a un -o
char *outputName(char *argv){
    if(strstr(argv, ".csv")!=NULL){
        return argv;
    }
    else{
        fprintf(stderr,"The output name is not valid. You need to add the csv extension\n");
        fprintf(stderr,"Like output.csv\n");
        exit(1);
    }
};

void getArgs(int argc, char **argv, char *mode, char *output, int displayConsole){
    for (int i = 0; i < argc; i++)
    {
        if (strncmp(argv[i], "-m", 2) == 0)
        {
            // faut que ca retourne un char ici pour que le mode soit choisi;
            mode = modeChoice(argv[i + 1]);
        }
        if (strncmp(argv[i], "-o", 2) == 0)
        {
            // récupération de l'output
            output = outputName(argv[i + 1]);
        }
        if (strncmp(argv[i], "-a", 2) == 0)
        {
            // affichage du résultat sur la console.
            displayConsole = 1;
        }
        if(strncmp(argv[i], "-h", 2)==0 || strncmp(argv[i], "help", 4)==0 ){
            help();
            if(strncmp(argv[i+1], "-m", 2) != 0 || strncmp(argv[i+1], "-o", 2) != 0 || strncmp(argv[i+1], "-c", 2) != 0){
                fprintf(stderr,"Error : do not know -> %s\n", argv[i+1]);
            }
            exit(1);
        }
        

    }
};