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
    BlastHeader blhd1;//, blhd2;

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

   // char *conserved2;
    char *ptrstr;
    char *ptrseq;
    char *lastchar;
    char *infilename;
    char *outfilename;
 //   char *msffilename;
    char *tablefilename;
    char *anchorfilename;
    char *motifsfilename;
    char *maskfilename;
    char *queryname;
    char *nomprofil;
    char *mask;
    char curline[256];

    int mismatches, prevn;
    int nmotifs = 0;

    FILE *infile, *outfile, *tablefile, *anchorfile, *motifsfile; //, *msffile

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
    // blhd2.prog = NULL;
    // blhd2.query = NULL; 

    //bt et apres p espace j
    // blhd2.dbase = NULL;

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
    if (length != 0)
    {
        i = 1;
        int j = 1;
        profiltotal = profilBuilding(seqres, infile, curline, length, conserved, maxprofile, 'p');
        while (curline[0] != '\0')
        {
            printf("curline is %s\n", curline);
            if (curline[0] == '>')
            {
                printf("Tour : %u\n", j);
                //printf("curline is %s\n", curline);

                seqres->rank = i++;
                seqres->next = (SeqHSP *)malloc(sizeof(SeqHSP));
                (seqres->next)->prev = seqres;
                seqres = seqres->next;
                seqres->next = NULL;
                j++;
                contribution = profilBuilding(seqres, infile, curline, length, conserved, maxprofile, 'p');
                printf("salina\n");
                if (strcmp(seqres->prev->sim->hsp, seqres->sim->hsp) != 0)
                {                
                    printf("salina\n");
                    addprofils(profiltotal, contribution, length);
                }

            }
            else
            {
                fgets(curline, 256, infile);
                if (curline[0] != '>')
                {
                    break;
                }
            }
        }
      //  printf("Le profil total est %f\n", *profiltotal);

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

    /**Reads an already existing profile if required**/
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
        // On va d'abord lisser le profil en récupérant d'abord la partie conservée
        // printf("conserved %s\n", conserved); //nickel
        // lissage
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

    if (motif == NULL)
    {
        outfile = stdout;
        printf(outfile, "\n *********************************");
        fprintf(outfile, "\n ***      FATAL Error          ***");
        fprintf(outfile, "\n ***                           ***");
        fprintf(outfile, "\n ***    Words too short        ***");
        fprintf(outfile, "\n *** could not extract motifs  ***");
        fprintf(outfile, "\n ***                           ***");
        fprintf(outfile, "\n *********************************\n\n");
        if (outfile != stdout)
        {
            fclose(outfile);
        }
        exit(1);
    }

    // déterminer le score maximum possible
    motifptr = motif;

    while (motifptr != NULL)
    {
        maxscore += motifptr->maxscore;
        if (motifptr->maxscore > maxmotifscore)
            maxmotifscore = motifptr->maxscore;
        motifptr = motifptr->next;
    }

    /*****************************************************************/

    motifsfilename = (char *)malloc(strlen(outfilename) + 8);
    strcpy(motifsfilename, outfilename); // d'ou viens le outfilename
    strcat(motifsfilename, ".motifs");

    if (!(motifsfile = fopen(motifsfilename, "w")))
    {
        motifsfile = stdout;
        printf("****************** W A R N I N G **********************\n");
        printf("Cannot write %s file, printing suggested words to Standard Output\n\n", motifsfilename);
        printf("*******************************************************\n");
    }

    fprintf(motifsfile, "\nSuggested Words : \n\n");

    motifptr = motif;
    // printf("mots conservés %s\n", *conserved);
    // ici a lieu l'écriture dans le fichier .motif
    while (motifptr != NULL)
    {
        // ici on écrit direct dans le motifsfile fichier .motif (j'ai ajouté le +1 de end parce que pas trop cohérent)
        fprintf(motifsfile, "\t%4d - %4d\t: ", motifptr->begin + 1, motifptr->end + 1);
        for (i = motifptr->begin; i < motifptr->end + 1; i++)
        {
            fprintf(motifsfile, "%c", *(conserved + i));
            // on va afficher ici ce qu'il y'a dans conserved
            //  et d'ici on peut avoir les positions de chaque acide aminé conservé
        }
        fprintf(motifsfile, "  \t:%8.2f\n\n", motifptr->maxscore);
        // fprintf(stdout,"Le max score est : %lf\n", motifptr->maxscore);
        motifptr = motifptr->next;
    }

    if (motifsfile != stdout)
        fclose(motifsfile);

    /*****************************************************************/
    /*** Plots main 'profile' to *.prof file                      ****/
    /*****************************************************************/

    maxvalue = profilplot(profiltotal, trimmed, length, outfilename, conserved, first, maxprofile);

    /*****************************************************************/
    /*** Writes consensus sequence in FastA format                ****/
    /*****************************************************************/

//     if (getargbool("-s") == 1)
//     {
//         consseq(profiltotal, length, outfilename, conserved);
//     }

//     if (getargbool("-anchors") == 1)
//     {
//         anchorfilename = (char *)malloc(strlen(outfilename) + 9);
//         strcpy(anchorfilename, outfilename);
//         strcat(anchorfilename, ".anchors");

//         if (!(anchorfile = fopen(anchorfilename, "w")))
//         {
//             printf("****************** W A R N I N G **********************\n");
//             printf("*** Cannot write %s anchors file ***\n\n", anchorfilename);
//             printf("*******************************************************\n");
//             anchorfile = NULL;
//         }
//         if (anchorfile)
//             fprintf(anchorfile, "Ballast %f\n", maxmotifscore);
//     }

//     if (getargbool("-table") == 1)
//     {
//         tablefilename = (char *)malloc(strlen(outfilename) + 7);
//         strcpy(tablefilename, outfilename);
//         strcat(tablefilename, ".table");
//         if (!(tablefile = fopen(tablefilename, "w")))
//         {
//             printf("****************** W A R N I N G **********************\n");
//             printf("*** Cannot write %s table file ***\n\n", tablefilename);
//             printf("*******************************************************\n");
//             tablefile = NULL;
//         }
//         else
//         {
//             for (motifptr = motif; motifptr != NULL; motifptr = motifptr->next)
//             {
//                 fprintf(tablefile, "%d ", motifptr->n);
//                 nmotifs = motifptr->n;
//             }
//         }
//     }

//     seqres = first;
//     while (seqres != NULL)
//     {
//         seqres->aligned = NULL;

// #ifdef DEBUG
//         printf("***%s\n", seqres->desc);
// #endif

//         /*****************************************************************/
//         /*** Identifies overlapping motifs/HSPs **************************/
//         /*****************************************************************/

//         motifdb = getsbjmotifs(motif, seqres->sim, trimmed, seqres->type);

//         if (motifdb != NULL)
//         {

//             /********************************************************/
//             /*** Sorts HSPs by order of position                 ****/
//             /********************************************************/

//             firstmotdb = sortbybegdb(motifdb);
//             seqres->sc = MHalign(motif, firstmotdb, trimmed, seqres->type, &firstalnmotdb);

//             if (seqres->type == 'p')
//             {
//                 if ((tablefile) && (seqres->p < tableseuil))
//                 {
//                     prevn = 0;
//                     fprintf(tablefile, "\n%-15s ", seqres->name);
//                 }

//                 seqres->aligned = (char *)malloc(length + 1);
//                 for (i = 0; i < length; i++)
//                 {
//                     ptrseq = (char *)(seqres->aligned + i);
//                     *ptrseq = '.';
//                 }
//                 ptrseq = (char *)(seqres->aligned + length);
//                 *ptrseq = '\0';

//                 motifdb = firstalnmotdb;
//                 while (motifdb != NULL)
//                 {
//                     ptrstr = (char *)malloc(motifdb->enddb - motifdb->begdb + 2);
//                     strncpy(ptrstr, motifdb->hsp->hsp + motifdb->begdb - motifdb->hsp->begdb, motifdb->enddb - motifdb->begdb + 1);
//                     lastchar = (char *)(ptrstr + motifdb->enddb - motifdb->begdb + 1);
//                     *lastchar = '\0';
//                     /*weight = MAX (0,1-motifdb->hsp->p);*/
//                     weight = motifdb->hsp->score;
//                     consmotif(motifdb->motif, ptrstr, motifdb->begin - motifdb->motif->begin, weight, weight);
//                     if (((seqres->p) < msfseuil))
//                     {
//                         if (anchorfile)
//                         {
//                             if (getargchar("-queryname", &queryname))
//                             {
//                                 fprintf(anchorfile, "seq: %15s %15s\tpos: %10d\tbeg: %10d %10d\tlen: %6d\tweight: %6.2f\n", queryname, seqres->name, motifdb->motif->peak, motifdb->begin + 1, motifdb->begdb + 1, motifdb->enddb - motifdb->begdb + 1, 100.0 * motifdb->score / maxmotifscore);
//                             }
//                             else
//                             {
//                                 if (seqres != first)
//                                 {
//                                     fprintf(anchorfile, "seq: %15s %15s\tpos: %10d\tbeg: %10d %10d\tlen: %6d\tweight: %6.2f\n", first->name, seqres->name, motifdb->motif->peak, motifdb->begin + 1, motifdb->begdb + 1, motifdb->enddb - motifdb->begdb + 1, 100.0 * motifdb->score / maxmotifscore);
//                                 }
//                             }
//                         }
//                         for (i = 0; i < (motifdb->enddb - motifdb->begdb + 1); i++)
//                         {
//                             ptrseq = (char *)(seqres->aligned + motifdb->begin + i);
//                             *ptrseq = *(ptrstr + i);
//                         }
//                     }
//                     free(ptrstr);

//                     if ((tablefile) && (seqres->p < tableseuil))
//                     {
//                         mismatches = motifdb->motif->n - prevn - 1;
//                         for (i = 0; i < mismatches; i++)
//                         {
//                             fprintf(tablefile, "-1.00 ");
//                         }
//                         fprintf(tablefile, "%5.2f ", motifdb->score / motifdb->motif->maxscore);
//                         prevn = motifdb->motif->n;
//                     }

//                     motifdb = motifdb->alnnext;
//                 }
//             }
//             freesbjmotifs(firstmotdb);

//             if ((tablefile) && (seqres->p < tableseuil) && (prevn < nmotifs))
//             {
//                 mismatches = nmotifs - prevn;

//                 for (i = 0; i < mismatches; i++)
//                 {
//                     fprintf(tablefile, "-1.00 ");
//                 }
//             }
//             seqres->nmatch = countmatches(seqres->sim);
//         }

//         seqres->twins = NULL;
//         seqres = seqres->next;
//     }


    return 0;
}