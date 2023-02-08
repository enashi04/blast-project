#ifndef PARAMETERS_H
#define PARAMETERS_H

#define MIN_SIZE 128
#define MAX_SIZE 4096
#define MAXI_SIZE 8192
#define FILE_SIZE 2429416

#define FICHIER "taxonomy.dat"

//définir une structure
typedef struct SpeciesInfo {
    char name[MIN_SIZE];
    char rank[MIN_SIZE];
    char id[MIN_SIZE];
    char parent[MIN_SIZE];
    char id_parent[MIN_SIZE];
    char *lineage;
    //struct SpeciesInfo *next;
}SpeciesInfo;

#endif
