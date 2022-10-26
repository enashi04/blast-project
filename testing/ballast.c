#include <math.h>
#include <string.h>
#include "readargs.h"
#include "prototypes.h"
#include "macros.h"
#include <ctype.h>
#include <stdbool.h>

#define HELPDIR "/ballast/Help"
#define MSFSEUIL 0.001
#define DEFANCHORSEUIL 0.20
extern double *profilBuilding(SeqHSP *seqres, FILE *file, char *line, int length, char *conserved, double *maxprofile, char type);

int main(int argc, char *argv[])
{

	/******************************************************************************/
	/***************************Déclaration des variables**************************/
	/******************************************************************************/
  	SeqHSP *seqres, *first;
	Motif *motif, *motifptr;
	Sbjmot *motifdb, *firstmotdb, *firstalnmotdb;

	double *maxprofile;
	double *maxprofile2;
	double *profiltotal;
	double *profiltotal2;
	double *smoothed;
	double *smoothed2;
	double *trimmed;
	double *trimmed2;
	double *contribution;
	double maxvalue;
	double maxscore = 0.0;
	double maxmotifscore = 0.0;
	double *ptrdbl;
	float weight;
	int length;
	int length2;

	float msfseuil;
	float tableseuil;

	int mismatches, prevn;
	int nmotifs = 0;

	char *conserved;
	char *conserved2;
	char *ptrstr;
	char *ptrseq;
	char *lastchar;

	BlastHeader blhd1;
	BlastHeader blhd2;

	int i, f;
	int tblastnsearch = 0;

	char *infilename;
	char *infilename2;
	char *outfilename;
	char *outfilename2;
	char *msffilename;
	char *tablefilename;
	char *anchorfilename;
	char *motifsfilename;
	char *maskfilename;
	char *queryname;
	char *nomprofil;
	char *mask;
	char curline[256];
	FILE *infile, *outfile, *msffile, *tablefile, *anchorfile, *motifsfile;

#define NUMARGS 20

	/******************************************************************************/
	/***************************Définitions des arguments**************************/
	/******************************************************************************/
    ARGDEF arg_def[NUMARGS] = {
        {"-anchors", 'b', 0, '\0'},
        {"-both", 'b', 0, '\0'},
        {"-mask", 's', 1, '\0'},
        {"-maxp", 'f', 1, '\0'},
        {"-maxtab", 'f', 1, '\0'},
        {"-n", 's', 1, '\0'},
        {"-nmaxp", 'f', 1, '\0'},
        {"-noext", 'b', 0, '\0'},
        {"-nosort", 'b', 0, '\0'},
        {"-o", 's', 1, 'y'},
        {"-p", 's', 1, 'y'},
        {"-plotn", 'b', 0, '\0'},
        {"-profil", 's', 1, '\0'},
        {"-queryname", 's', 1, '\0'},
        {"-S", 'f', 1, '\0'},
        {"-s", 'b', 0, '\0'},
        {"-sens", 'f', 1, '\0'},
        {"-smooth", 'd', 1, '\0'},
        {"-t", 'f', 1, '\0'},
        {"-table", 'b', 0, '\0'}};


    if (initargs(argv, argc, arg_def, NUMARGS) == -1)
    {
        printusage(argv[0], HELPDIR);
        exit(1);
    }

    tablefile = NULL;
    anchorfile = NULL;

    getargchar("-o", &outfilename);

    if (getargfloat("-maxp", &msfseuil) == NULL)
    {
        msfseuil = DEFMAXP;
    }
    if (getargfloat("-maxtab", &tableseuil) == NULL)
    {
        tableseuil = msfseuil;
    }

    blhd1.prog = NULL;
    blhd1.query = NULL;
    blhd1.dbase = NULL;
    blhd2.prog = NULL;
    blhd2.query = NULL; 

    //bt et apres p espace j
    blhd2.dbase = NULL;

    seqres = (SeqHSP *)malloc(sizeof(SeqHSP));
    first = seqres;
    seqres->prev = NULL;
    seqres->next = NULL;

    getargchar("-p", &infilename);
    if (!(infile = fopen(infilename, "r")))
    {
        printf("\n*** ERROR: %s file not found ***\n\n", infilename);
        exit(1);
    }
	/******************************************************************************/
	/*************************Longueur de la séquence query************************/
	/******************************************************************************/
    length = initresfile(infile, curline, &blhd1);

    conserved = (char *)malloc(length + 1);
    maxprofile = (double *)malloc(sizeof(double) * length);

    for (i = 0; i < length; i++)
    {
        ptrstr = (char *)(conserved + i);
        *ptrstr = '.';
        ptrdbl = (double *)(maxprofile + i);
        *ptrdbl = 0.0;
    }

    ptrstr = (char *)(conserved + length);
    *ptrstr = '\0';


    /******************************************************************************/
	/*************************Construction du profil blastp************************/
	/******************************************************************************/
    if (length != 0)
    {
        i = 1;
        profiltotal = profilBuilding(seqres, infile, curline, length, conserved, maxprofile, 'p');
        while (curline[0] != '\0')
        {
            seqres->rank = i++;
            seqres->next = (SeqHSP *)malloc(sizeof(SeqHSP));
            (seqres->next)->prev = seqres;
            seqres = seqres->next;
            seqres->next = NULL;
            contribution = profilBuilding(seqres, infile, curline, length, conserved, maxprofile, 'p');
            if (strcmp(seqres->prev->sim->hsp, seqres->sim->hsp) != 0)
            {                
                addprofils(profiltotal, contribution, length);
            }
        }
    }
    else
    {
        fprintf(outfile, "\n *********************************");
        fprintf(outfile, "\n ***     FATAL BLASTP error    ***");
        fprintf(outfile, "\n ***                           ***");
        fprintf(outfile, "\n *** No HSP to build \'profile\' ***");
        fprintf(outfile, "\n *********************************\n\n");
        if (outfile != stdout)
        {
            fclose(outfile);
        }
        exit(1);
    }
    fclose(infile);

    /******************************************************************************/
	/****************************Lire un profil existant***************************/
	/******************************************************************************/
    if (getargchar("-profil", &nomprofil) != NULL)
    {
        if (fopen(nomprofil, "r"))
        {
            readprofil(nomprofil, profiltotal);
        }
    }

    /*****************************************************************/
    /*** Masks profile if required                  ******************/
    /*** otherwise treats and uses the full profile ******************/
    /*****************************************************************/
    if (getargchar("-mask", &maskfilename) != NULL)
    {
        if (fopen(maskfilename, "r"))
        {
            readmask(maskfilename, &mask);
            // printf("On va ici\n");
            trimmed = maskprofil(profiltotal, length, mask);
        }
    }
    else
    {
        
        smoothed = smoothprofil(profiltotal, length, conserved);
        if (getargbool("-noext") == 0)
        {
            trimmed = trimprofilext(profiltotal, smoothed, length, conserved);
        }
        else
        {
            trimmed = trimprofil(profiltotal, smoothed, length, conserved);
        }
    }

    // on passe à l'extraction du motif
    motif = extractmotifs(trimmed, length, conserved);

    return 0;
}