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

    // déclaration des variables
    SeqHSP *seqres, *first;
    Motif *motif, *motifptr;
    Sbjmot *motifdb, *firstmotdb, *firstalnmotdb;
    BlastHeader blhd1, blhd2;

    double *profiltotal;
    double *smoothed;
    double *trimmed;
    double *contribution;
    double *maxprofile;
    char *conserved;
    double maxvalue;
    double maxscore = 0.0;
    double maxmotifscore = 0.0;
    double *ptrdbl;
    float weight;
    int length;
    int i;

    float msfseuil;
    float tableseuil;

    // int mismatches, prevn;
    // int nmotifs = 0;

    char *conserved2;
    char *ptrstr;
    char *ptrseq;
    char *lastchar;
    char *infilename;
    char *outfilename;
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

    // printf(("Déclaration des variables : ok \n"));

    // initialisation des variables

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

    length = initresfile(infile, curline, &blhd1);
    //  fprintf(stdout, "la longueur du fichier est de : %u \n", length);

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

    // fprintf(stdout, "Initialisation : ok\n");

    // construction du profil BlastP
    int j = 0;
    if (length != 0)
    {
        i = 1;
        int j = 1;
        profiltotal = profilBuilding(seqres, infile, curline, length, conserved, maxprofile, 'p');
        while (curline[0] != '\0')
        {
            fgets(curline, 256, infile);
            if (curline[0] == '>')
            {
                printf("Tour : %u\n", j);
                printf("curline is %s\n", curline);

                seqres->rank = i++;
                seqres->next = (SeqHSP *)malloc(sizeof(SeqHSP));
                (seqres->next)->prev = seqres;
                seqres = seqres->next;
                seqres->next = NULL;
                j++;
                contribution = profilBuilding(seqres, infile, curline, length, conserved, maxprofile, 'p');

                if (strcmp(seqres->prev->sim->hsp, seqres->sim->hsp) != 0)
                {
                    addprofils(profiltotal, contribution, length);
                }
            }
            else
            {
                break;
            }
        }
    }
    else{
        fprintf(outfile, "\n *********************************");
		fprintf(outfile, "\n ***     FATAL BLASTP error    ***");
		fprintf(outfile, "\n ***                           ***");
		fprintf(outfile, "\n *** No HSP to build \'profile\' ***");
		fprintf(outfile, "\n *********************************\n\n");
		if (outfile != stdout){
			fclose(outfile);
        }
		exit(1);
    }
    /**Reads an already existing profile if required**/
    if (getargchar("-profil", &nomprofil) != NULL)
	{
		readprofil(nomprofil, profiltotal);
	}

    /*****************************************************************/
	/*** Masks profile if required                  ******************/
	/*** otherwise treats and uses the full profile ******************/
	/*****************************************************************/

	if (getargchar("-mask", &maskfilename) != NULL)
	{
		readmask(maskfilename, &mask);
		trimmed = maskprofil(profiltotal, length, mask);
	}
    else{
        //ici le profil à lissé et à couper ! 
        
    }

    fclose(infile);

    return 0;
}