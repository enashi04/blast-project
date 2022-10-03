#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "macros.h"
#include "readargs.h"
#include "types.h"
#include <ctype.h>
#include <stdbool.h>
#define NETRA 0

double getMaxP(double maxp, char type);
void initSeqres(SeqHSP *seqres, SimPrf *simprf, char type, double maxp);
void verifLine(char *line);

double *profilBuilding(SeqHSP *seqres, FILE *file, char *line, int length, char *conserved, double *maxprofile, char type){
    
    /*Déclaration des variables*/
    double p, facteur, fctr;
    double *profil, *ptr, *simptr;
    SimPrf *simprf;
    char *outtext = NULL, *ptrstr, *begline, *queryseq, *seq, *seqhsp, *startline;
    int endofdbseq = 0, ok = 0, okhsp = 0, gapped, taux, naas, begin, end, debut, dline, n;
    int debdb, enddb, begdb, dline2, dline1;
    int identique;

//valeur de maxp
    double maxp= getMaxP(maxp, type);
    //initialisation de seqres et simprf
    initSeqres(seqres, simprf, type, maxp);
    //récupération de la ligne où nous nous trouvons
    outtext = (char *)malloc(strlen(line)+1);
    strcpy(outtext, line);

    //condition permettant de trouver le '>' dans l'outtext et de l'enlever
    verifLine(line);

    // récupération de desc
    seqres->desc = (char *)malloc(strlen(line) + 1);
    strcpy(seqres->desc, line);
    printf("desc is %s\n", seqres->desc);
    // récupération de name
    ptrstr = strtok(line, " ");
    seqres->name = (char *)malloc(strlen(ptrstr) + 1);
    strcpy(seqres->name, ptrstr);

    return 1;

}

double getMaxP(double maxp, char type){
    if (getargdouble("-maxp", &maxp) == NULL)
    {
        maxp = DEFMAXP;
        if (type == 'n')
        {
            if (getargdouble("-nmaxp", &maxp) == NULL)
                maxp = DEFMAXP;
        }
    }
    return maxp;
}

void initSeqres(SeqHSP *seqres, SimPrf *simprf, char type, double maxp){
    seqres->sc = 0;
    seqres->nmatch = 0;
    seqres->type = type;
    seqres->risedup = ' ';

    seqres->sim = (SimPrf *)malloc(sizeof(SimPrf));

    simprf = seqres->sim;
    simprf->text = NULL;
    simprf->score = 2;
    printf("je suis le %f\n", simprf->score);

    simprf->maxscore = 0;
    simprf->nmatch = 0;
    seqres->p = 1;
    seqres->prob = maxp;
}

