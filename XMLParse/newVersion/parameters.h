#ifndef PARAMETERS_H
#define PARAMETERS_H

#define MIN_SIZE 128
#define MAX_SIZE 4096
#define MAXI_SIZE 8192
#define FILE_SIZE 2429416

#define FICHIER "taxonomy.dat"

//définir une structure
typedef struct SpeciesInfo {
    char *name;
    char *rank;
    char *id;
    char *id_parent;
}SpeciesInfo;

#endif
