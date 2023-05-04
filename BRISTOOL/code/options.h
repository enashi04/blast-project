#ifndef _OPTIONS_
#define _OPTIONS_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void help(char **argv);
char *modeChoice(int argc, char **argv);
char *outputName(int argc, char **argv);
 
int displayResults(int argc, char **argv);
void invalidOptions(int argc, char **argv);
char *inputRecovery(int argc, char **argv);
char *newname(char *name);
void noArgument(int argc);

#endif
