#ifndef PARAMETERS_H
#define PARAMETERS_H

#define MIN_SIZE 128
#define MAX_SIZE 4096
#define FICHIER "taxonomy.dat"

//définir une structure
typedef struct SpeciesInfo {
    char *name;
    char *rank;
    char *id;
    char *parent;
    char *lineage;
    struct SpeciesInfo *next;
    struct SpeciesInfo *previous;
}SpeciesInfo;

#endif
