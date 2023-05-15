/*** profilplot **************************************************/
/*                                                               */
/*   'Plots' 'profile' to ascii file including list of sequences */
/*   producing HSPs that were included into the 'profile'        */
/*   Returns maximum value of 'profile'                          */
/*                                                               */
/*****************************************************************/
/* double *profil       : 'profile' to print out                 */
/* int     length       : length of profile                      */
/* char   *filename     : output file name                       */
/* char   *conserved    : residues appearing in at least one HSP */
/* SeqHSP *first        : first seq producing HSPs               */
/*                                                               */
/* char   *plotfilename : 'profile' output file name             */
/* FILE   *plotfile     : 'profile' output file handler          */
/* SeqHSP *seqres       : current seq producing HSPs             */
/* double  max          : max value of 'profile'                 */
/* double  maxvalue     : returned max value of 'profile'        */
/* double  scale        : scaling factor                         */
/* double *ptr          : pointer to 'profile' positions         */
/*****************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "readargs.h"
#include "types.h"

#define LARGEUR 50.0

void printnicedouble(FILE *outfile, double p);

double profilplot(double *profil, double *profil2, int length, char *filename, char *conserved, SeqHSP *first, double *maxprofile)
{
    SeqHSP *seqres;
    int i, j, k;
    double *ptr;
    double max, maxvalue, scale;
    char *plotfilename;
    FILE *plotfile;
    FILE *listfile;
#ifdef WWW
    FILE *maskfile;
    char *maskfilename;
    char *listfilename;
#endif
    int both;
    int at_least_one = 0;

    //double delta, ddelta;

    both = getargbool("-both");

    max = 0;

    /*****************************************************************/
    /*** What's the 'profile' file name ? ****************************/
    /*****************************************************************/

    plotfilename = (char *)malloc(strlen(filename) + 6);
    sprintf(plotfilename, "%s.prof", filename);

#ifdef WWW
    if (getargchar("-mask", &maskfilename) == NULL)
    {
        maskfilename = (char *)malloc(strlen(filename) + 6);
        sprintf(maskfilename, "%s.mask", filename);
    }

    listfilename = (char *)malloc(strlen(filename) + 6);
    sprintf(listfilename, "%s.list", filename);
#endif
    /*****************************************************************/

    plotfile = fopen(plotfilename, "w");
#ifdef WWW
    if (getargchar("-mask", &maskfilename) == NULL)
        maskfile = fopen(maskfilename, "w");
    listfile = fopen(listfilename, "w");
#endif

    /*****************************************************************/
    /*** Writes a list of sequences used to build 'profile'         **/
    /*** typically sequences with seqres->p != 1 except nucleotides **/
    /*** if the -both parameter was not on the command line         **/
    /*****************************************************************/

#ifndef WWW
    listfile = plotfile;
#endif

    fprintf(listfile, "BALLAST profile %s\n\n", plotfilename);
    fprintf(listfile, "The following sequences from BLAST results were used to build this profile :\n");
    fprintf(listfile, "--------------------------------------------------------------------------------\n");

    for (seqres = first; seqres != NULL; seqres = seqres->next)
    {
        if ((seqres->type == 'n') && (both == FALSE))
            break;
        if (seqres->p == 1)
            continue;
        fprintf(listfile, "%s ", seqres->desc);
        printnicedouble(listfile, seqres->prob);
        fprintf(listfile, "\n");
        at_least_one = 1;
    }

    if (at_least_one == 0)
    {
        fprintf(listfile, "\n***************************************************************************\n");
        fprintf(listfile, "***                                                                     ***\n");
        fprintf(listfile, "***                          WARNING                                    ***\n");
        fprintf(listfile, "***                                                                     ***\n");
        fprintf(listfile, "*** There was no sequence satisfying the criteria to build the profile! ***\n");
        fprintf(listfile, "*** You should try to modify the \'-maxp\' and/or use \'-both\' parameters. ***\n");
        fprintf(listfile, "***                                                                     ***\n");
        fprintf(listfile, "***************************************************************************\n");

        printf("\n***************************************************************************\n");
        printf("***                          WARNING                                    ***\n");
        printf("***                                                                     ***\n");
        printf("*** There was no sequence satisfying the criteria to build the profile! ***\n");
        printf("*** You should try to modify the \'-maxp\' and/or use \'-both\' parameters. ***\n");
        printf("***************************************************************************\n\n");
    }
    /*****************************************************************/

    /*****************************************************************/
    /*** Writes a list of sequences NOT used to build 'profile'   ****/
    /*** typically sequences with seqres->p == 1 and nucleotides  ****/
    /*** if the -both parameter was not on the command line       ****/
    /*****************************************************************/

    fprintf(listfile, "\nThe following sequences from BLAST results were NOT used to build this profile :\n");
    fprintf(listfile, "--------------------------------------------------------------------------------\n");

    for (seqres = first; seqres != NULL; seqres = seqres->next)
    {
        if (seqres->prob >= 1)
            continue;
        if ((seqres->type == 'n') && (both == TRUE) && (seqres->p != 1))
            continue;
        if ((seqres->type == 'p') && (seqres->p != 1))
            continue;
        fprintf(listfile, "%s ", seqres->desc);
        printnicedouble(listfile, seqres->prob);
        fprintf(listfile, "\n");
    }
    /*****************************************************************/

    /*****************************************************************/
    /*** What's the max value ?                                   ****/
    /*****************************************************************/

    for (i = 0; i < length; i++)
    {
        /*
        if (max < *(profil + i)) max = *(profil + i);
        */
        if (max < *(maxprofile + i))
            max = *(maxprofile + i);
    }

    maxvalue = max;
    fprintf(listfile, "...\n******************************************************************************\n\n");

#ifdef WWW
    fprintf(plotfile, "%d\n", length);
    fprintf(plotfile, "%f\n", max);
#else
    fprintf(plotfile, "maximum : %f\n", max);

    if (max == 0)
        max = 1;

    scale = max / LARGEUR;
#endif
    /*****************************************************************/

    /*****************************************************************/
    /*** Do an ASCII plot in plotfile                             ****/
    /*****************************************************************/

    // delta = *profil;
    // ddelta = 0;

    for (i = 0; i < length; i++)
    {
        ptr = (double *)(profil + i);

#ifndef WWW
        fprintf(plotfile, "%c : %5d : %10.3f :", *(conserved + i), i + 1, *ptr);
        k = (int)(*ptr / scale);
        if (*(profil2 + i) > 0.0)
        {
            for (j = 0; j < k; j++)
                fprintf(plotfile, "#");
        }
        else
        {
            for (j = 0; j < k; j++)
                fprintf(plotfile, "+");
        }

        ptr = (double *)(maxprofile + i);
        k = (int)(*ptr / scale) - k;
        for (j = 1; j < k - 1; j++)
            fprintf(plotfile, " ");
        fprintf(plotfile, "|");
#else
        fprintf(plotfile, "%c %10.3f", *(conserved + i), *ptr);
        if (getargchar("-mask", &maskfilename) == NULL)
        {
            if (*(profil2 + i) > 0.0)
            {
                fprintf(maskfile, "1");
            }
            else
            {
                fprintf(maskfile, "0");
            }
        }
#endif

        fprintf(plotfile, "\n");
    }
    /*****************************************************************/

    fclose(plotfile);

#ifdef WWW
    fclose(listfile);
    if (getargchar("-mask", &maskfilename) == NULL)
        fclose(maskfile);
#endif

    free(plotfilename);
    return (maxvalue);
}
