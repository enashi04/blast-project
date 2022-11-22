#ifndef _OPTIONS_
#define _OPTIONS_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ici on met l'affichage de help avec l'argument -h ou help
void help(char **argv);
// si y'a la présence du mode alors il faut qu'on voit l'argument d'apres. Si c'est bronze argent ou or.
char *modeChoice(int argc, char **argv, char *name);
// fichier de sortie si y'a un -o
char *outputName(int argc, char **argv, char *name);
 
int displayResults(int argc, char **argv);
void invalidOptions(int argc, char **argv);
char *inputRecovery(int argc, char **argv, char *inputName);
char *newname(char *name);
#endif
