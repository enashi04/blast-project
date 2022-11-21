#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// ici on met l'affichage de help avec l'argument -h ou help
void help();
// si y'a la présence du mode alors il faut qu'on voit l'argument d'apres. Si c'est bronze argent ou or.
char *modeChoice(char *argv);
// fichier de sortie si y'a un -o
char *outputName(char *argv);
 
void getArgs(int argc, char **argv, char *mode, char *output, int displayConsole);