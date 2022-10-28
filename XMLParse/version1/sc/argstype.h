#include "xmlparser.h"

char *substr(const char *src, int m, int n);
int moreThanTwoArgs();
int twoArgs(char **argv);
int noArgument();
void modechoice(int mode, xmlDoc *fichier, FILE *fp, char *filename, char *name);