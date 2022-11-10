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
    fprintf(stderr,"\r -a \t Display CSV file example\n");
    exit(1);
};

//si y'a la présence du mode alors il faut qu'on voit l'argument d'apres. Si c'est bronze argent ou or.
void mode();

//si l'argument est -d, alors on affiche directement un exemple de fichier parsé sous le format gold
void display();

 //fichier de sortie si y'a un -o
void output();