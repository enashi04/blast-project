#ifndef PARAMETERS_H
#define PARAMETERS_H

#define MIN_SIZE 128 //taille minimal
#define MAX_SIZE 4096 //taille 
#define MAXI_SIZE 8192 //taille maximal pour la lignée
#define FILE_SIZE 2429416 //taille du fichier taxonomy.dat
#define SPECIES_SIZE 2961947
#define EXTRA_SIZE 16777216 //nombre maximal de caractère dans le fichier de sortie 2^24

#define FICHIER "taxonomy.dat" //fichier taxonomy


typedef struct SpeciesInfo
{
    char *name;
    int parentid;
    char *rank; //id = position dans le tableau
    char *othername; // on en a besoin car il y'a des espèces qui ont d'autres noms !
} SpeciesInfo;

typedef struct FillSpeciesInfo{
    char *name; 
    char *parent;
    char *rank;
    char *lineage;
    int id;
} FillSpeciesInfo;

#endif
