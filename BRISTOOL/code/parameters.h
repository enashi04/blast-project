#ifndef PARAMETERS_H
#define PARAMETERS_H
#define MIN_SIZE 128 //taille minimal
#define MAX_SIZE 4096 //taille 
#define MAXI_SIZE 8192 //taille maximal pour la lignée
#define FILE_SIZE 2429416 //taille du fichier taxonomy.dat
#define EXTRA_SIZE 16777216 //nombre maximal de caractère dans le fichier de sortie 2^24
#define SIZE_PLUS 16384


#define FICHIER "taxonomy.dat" //fichier taxonomy
#define BLAST_FILE "outblast"
#define BALLAST_ACCESS "../../BAL-LAST/code/./ballast"
#include "log.h"
int SPECIES_SIZE; //taille du tableau d'espèce

typedef struct SpeciesInfo
{
    char *name;//tableau de nom de l'espèce
    int parentid; //taxid du parent
    char *rank; //rang de l'espèce
} SpeciesInfo;

typedef struct FillSpeciesInfo{
    char *name; //nom de l'espèce
    char *rank; //rang de l'espèce
    char *lineage; //lignée de l'espèce
    int id; //taxid de l'espèce
    char *parent; //nom du parent de l'espèce
} FillSpeciesInfo;

#endif
