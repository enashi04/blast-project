/*** ballast *****************************************************/
/*                                                               */
/*   BALLAST    version 1.00   (03/00)                           */
/*                                                               */
/*  Uses the information given by high scoring HSPs from a BLAST */
/*  search to devise a rescoring scheme based upon conservation  */
/*  of residues. It is then able to rescore all proposed HSPs to */
/*  increase the sensitivity of the search.                      */
/*                                                               */
/*   Frederic PLEWNIAK, Julie D. Thompson  & Olivier POCH (2000) */
/*   'Ballast: Blast post-processing based on locally conserved  */
/*   segments'; Bioinformatics (in press)                        */
/*                                                               */
/*   I.G.B.M.C.  -  Strasbourg - France                          */
/*   plewniak@igbmc.u-strasbg.fr                                 */
/*                                                               */
/*****************************************************************/
/* SeqHSP *seqres       : Sequences producing HSPs               */
/* SeqHSP *first        : First sequence producing HSPs          */
/* SimPrf *simptr       : similarity 'profiles'                  */
/*                                                               */
/* Motif  *motif        : list of proposed motifs                */
/* Motif  *motifptr     : pointer to current motif               */
/*                                                               */
/* double *profiltotal  : 'profile' from BlastP search           */
/* double *profiltotal2 : 'profile' from TBlastN search          */
/* double *smoothed     : smoothed 'profile' from BlastP         */
/* double *smoothed2    : smoothed 'profile' from TBlastN        */
/* double *trimmed      : trimmed  'profile' from  BlastP        */
/* double *trimmed2     : trimmed  'profile' from TBlastN        */
/* double *contribution : contribution of current sequence to    */ 
/*                        the 'profile'                          */
/* double  maxvalue     : maximum value of 'profile'             */
/* double  maxscore     : maximum possible score                 */
/* int     length       : length of BlastP 'profile'             */
/* int     length2      : length of TBlastN 'profile'            */
/*                                                               */
/* char   *conserved    : residues found in at least one HSP     */
/* char   *conserved2   : idem for TBlastN search                */
/* char   *ptrstr       : pointer to the above                   */
/* BlastHeader blhd1    : BlastP header                          */
/* BlastHeader blhd2    : TBlastN header                         */
/*                                                               */
/* char *infilename     : BlastP result file name                */
/* char *infilename2    : TBlastN result file name               */
/* char *infilename2    : TBlastN result file name               */
/* char *outfilename    : BALLAST output file name               */
/* char *outfilename2   : file name for TBlastN 'profile'        */
/* char *maskfilename   : Mask file name                         */
/* char *mask           : Mask to apply to profile               */
/* char *nomprofil      : Name for input profile file name       */
/* char  curline[256]   : current line read from BLAST file      */
/* FILE *infile         : input file handler                     */
/* FILE *outfile        : output file handler                    */
/*****************************************************************/
#define MAIN_PROC

#define HELPDIR "/ballast/Help"
#define MSFSEUIL 0.001
#define DEFANCHORSEUIL 0.20

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "global.h"
#include "readargs.h"
#include "macros.h"
#include "types.h"
//#include "prototypes.h"

#include "consmotif.h"
#include "consseq.h"
#include "countmatches.h"
#include "extractmotifs.h"
#include "filter.h"
#include "freesbjmotifs.h"
#include "gcgcheck.h"
#include "getargs.h"
#include "handlegaps.h"
#include "initresfile.h"
#include "loadhsp.h"
#include "maskprofil.h"
#include "mhalign.h"
#include "overlap.h"
#include "printheader.h"
#include "printmsf.h"
#include "printnicedouble.h"
#include "printqueryseq.h"
#include "printresout.h"
#include "profilplot.h"
#include "readmask.h"
#include "readprofil.h"
#include "redundout.h"
#include "riseup.h"
#include "scalaire.h"




int main (int argc, char *argv[])
{
SeqHSP *seqres, *first;
Motif  *motif, *motifptr;
Sbjmot *motifdb, *firstmotdb,*firstalnmotdb;

double *maxprofile;
double *maxprofile2;
double *profiltotal;
double *profiltotal2;
double *smoothed;
double *smoothed2;
double *trimmed;
double *trimmed2;
double *contribution;
double  maxvalue;
double  maxscore=0.0;
double  maxmotifscore = 0.0;
double *ptrdbl;
float   weight;
int     length;
int     length2;

float   msfseuil;

float   tableseuil;

int     mismatches, prevn;
int     nmotifs=0;

char   *conserved;
char   *conserved2;
char   *ptrstr;
char   *ptrseq;
char   *lastchar;

BlastHeader blhd1;
BlastHeader blhd2;

int i;

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
char   curline[256];
FILE   *infile, *outfile, *msffile, *tablefile, *anchorfile, *motifsfile;
int   tblastnsearch=0;

#define NUMARGS 20

Argdef  arg_def [NUMARGS] = {
        {"-anchors"  , 'b' , 0 , '\0' },
        {"-both"     , 'b' , 0 , '\0' },
        {"-mask"     , 's' , 1 , '\0' },
        {"-maxp"     , 'f' , 1 , '\0' },
        {"-maxtab"   , 'f' , 1 , '\0' },
        {"-n"        , 's' , 1 , '\0' },
        {"-nmaxp"    , 'f' , 1 , '\0' },
        {"-noext"    , 'b' , 0 , '\0' },
        {"-nosort"   , 'b' , 0 , '\0' },
        {"-o"        , 's' , 1 , 'y'  },
        {"-p"        , 's' , 1 , 'y'  },
        {"-plotn"    , 'b' , 0 , '\0' },
        {"-profil"   , 's' , 1 , '\0' },
        {"-queryname", 's' , 1 , '\0' },
        {"-S"        , 'f' , 1 , '\0' },
        {"-s"        , 'b' , 0 , '\0' },
        {"-sens"     , 'f' , 1 , '\0' },
        {"-smooth"   , 'd' , 1 , '\0' },
        {"-t"        , 'f' , 1 , '\0' },
        {"-table"    , 'b' , 0 , '\0' }
        };

/*****************************************************************/
/*** Initialisation                                           ****/
/*****************************************************************/

if (initargs (argv,argc,arg_def,NUMARGS) == -1) {
        printusage(argv[0],HELPDIR);
        exit(1);
}

tablefile  = NULL;
anchorfile = NULL;

getargchar ("-o",&outfilename);

/*
if (getargfloat ("-msf",&msfseuil) == NULL) msfseuil = MSFSEUIL;
*/
if (getargfloat ("-maxp",&msfseuil) == NULL) msfseuil = DEFMAXP;

if (getargfloat ("-maxtab",&tableseuil) == NULL) tableseuil = msfseuil;

blhd1.prog = NULL;
blhd1.query= NULL;
blhd1.dbase= NULL;
blhd2.prog = NULL;
blhd2.query= NULL;
blhd2.dbase= NULL;

seqres = (SeqHSP *) malloc (sizeof (SeqHSP));
first  = seqres;
seqres->prev = NULL;
seqres->next = NULL;

getargchar ("-p",&infilename);
if (!(infile = fopen(infilename,"r"))) {printf ("\n*** ERROR: %s file not found ***\n\n",infilename); exit(1);}

length = initresfile (infile,curline,&blhd1);

conserved = (char *) malloc (length+1);
maxprofile = (double *) malloc (sizeof(double) * length);

for (i=0;i<length;i++)
        {
        ptrstr = (char *)(conserved +i);
        *ptrstr = '.';
        ptrdbl = (double *) (maxprofile +i);
        *ptrdbl = 0.0;
}
ptrstr = (char *) (conserved + length);
*ptrstr = '\0';

/*****************************************************************/



/*****************************************************************/
/*** Builds 'profile' from BlastP search                      ****/
/*****************************************************************/

if (length != 0)
        {
        i=1;
        profiltotal  = loadHSP (seqres,infile,curline,length,conserved,maxprofile,'p');

        while (curline[0] != '\0')
                {
                seqres->rank=i++;
                seqres->next = (SeqHSP *) malloc (sizeof (SeqHSP));
                (seqres->next)->prev = seqres;
                seqres = seqres->next;
                seqres->next = NULL;
                contribution = loadHSP (seqres,infile,curline,length,conserved,maxprofile,'p');
                if (strcmp(seqres->prev->sim->hsp,seqres->sim->hsp) != 0 ) addprofils (profiltotal,contribution,length);
                }
        seqres->rank=i++;
        }
else
        {
#ifdef WWW
        outfile = fopen(outfilename,"w");
#else
        outfile = stdout;
#endif
        fprintf (outfile,"\n *********************************");
        fprintf (outfile,"\n ***     FATAL BLASTP error    ***");
        fprintf (outfile,"\n ***                           ***");
        fprintf (outfile,"\n *** No HSP to build \'profile\' ***");
        fprintf (outfile,"\n *********************************\n\n");
        if (outfile != stdout) fclose (outfile);
        exit (1);
        }

fclose (infile);

/*****************************************************************/



/*****************************************************************/
/*** Builds 'profile' from TBlastN search (if any)            ****/
/*****************************************************************/

if (getargchar ("-n",&infilename2) != NULL)
        {
        if (!(infile = fopen(infilename2,"r")))
                {
                printf ("\n*****************************************\n");
                printf ("***            WARNING                ***\n");
                printf ("***                                   ***\n");
                printf ("***        TBLASTN file not found     ***\n");
                printf ("***  Only BLASTP search will be used  ***\n");
                printf ("*****************************************\n\n");
                }
        else
                {
                tblastnsearch = 1;
                }
        }

if (tblastnsearch == 1)
        {
        length2 = initresfile (infile,curline,&blhd2);
        conserved2 = (char *) malloc (length2);
        maxprofile2 = (double *) malloc (sizeof(double) * length2);
        for (i=0;i<length2;i++)
                {
                ptrstr = (char *)(conserved2 +i);
                *ptrstr = '.';
                ptrdbl  = (double *) (maxprofile2 +i);
                *ptrdbl = 0.0;
                }

        if (length2 != 0)
                {
                i=1;

                profiltotal2  = loadHSP (seqres,infile,curline,length2,conserved2,maxprofile2,'n');

                while (curline[0] != '\0')
                        {
                        seqres->rank=i++;
                        seqres->next = (SeqHSP *) malloc (sizeof (SeqHSP));
                        (seqres->next)->prev = seqres;
                        seqres = seqres->next;
                        seqres->next = NULL;
                        contribution = loadHSP (seqres,infile,curline,length2,conserved2,maxprofile2,'n');
                        addprofils (profiltotal2,contribution,length2);
                        if (getargbool ("-both") == 1 )
                                {
                                if (strcmp(seqres->prev->sim->hsp,seqres->sim->hsp) != 0) addprofils (profiltotal,contribution,length);
                                }
                        }
                seqres->rank=i++;
                }
        else
                {
                printf ("\n*****************************************");
                printf ("\n***            WARNING                ***");
                printf ("\n***                                   ***\n");
                printf ("*** No HSP produced by TBLASTN search ***\n");
                printf ("***  Only BLASTP search will be used  ***");
                printf ("\n*****************************************\n\n");
                }

        fclose (infile);
        }

/*****************************************************************/


/*****************************************************************/
/*** Reads an already existing profile if required            ****/
/*****************************************************************/

if (getargchar ("-profil",&nomprofil) != NULL)
        {
        readprofil (nomprofil, profiltotal);
        }

/*****************************************************************/


/*****************************************************************/
/*** Masks profile if required                  ******************/
/*** otherwise treats and uses the full profile ******************/
/*****************************************************************/

if (getargchar ("-mask",&maskfilename) != NULL)
        {
        readmask (maskfilename, &mask);
        trimmed = maskprofil (profiltotal,length,mask);
        }
else
        {
        /*****************************************************************/
        /*** Smoothes 'profiles'                                      ****/
        /*****************************************************************/

        smoothed = smoothprofil (profiltotal, length,conserved);
        if ((getargbool("-plotn") == 1 ) && (tblastnsearch == 1))
                {smoothed2 = smoothprofil (profiltotal2, length2,conserved2);}

        /*****************************************************************/
        /*****************************************************************/
        /*** Trims 'profiles'                                         ****/
        /*****************************************************************/

        if (getargbool ("-noext") == 0)
                {
                trimmed = trimprofilext (profiltotal, smoothed, length,conserved);
                }
        else
                {
                trimmed = trimprofil (profiltotal, smoothed, length,conserved);
                }

        if ((getargbool("-plotn") == 1 ) && (tblastnsearch == 1))
                {trimmed2 = trimprofil (profiltotal2, smoothed2, length2,conserved);}

        /*****************************************************************/
        }

/*****************************************************************/


/*****************************************************************/
/*** Extracts motifs *********************************************/
/*****************************************************************/

motif = extractmotifs (trimmed,length,conserved);

if (motif == NULL)
        {
#ifdef WWW
        outfile = fopen(outfilename,"w");
#else
        outfile = stdout;
#endif
        fprintf (outfile,"\n *********************************");
        fprintf (outfile,"\n ***      FATAL Error          ***");
        fprintf (outfile,"\n ***                           ***");
        fprintf (outfile,"\n ***    Words too short        ***");
        fprintf (outfile,"\n *** could not extract motifs  ***");
        fprintf (outfile,"\n ***                           ***");
        fprintf (outfile,"\n *********************************\n\n");
        if (outfile != stdout) fclose (outfile);

/*** Tester ici trimmed : si somme = 0 => exit(1) sinon continuer avec MINWORD = 0 ***/

        exit (1);
        }

/*****************************************************************/


/*****************************************************************/
/*** Determine maximum possible score ****************************/
/*****************************************************************/

motifptr = motif;

while (motifptr != NULL)
        {
        maxscore += motifptr->maxscore;
        if (motifptr->maxscore > maxmotifscore) maxmotifscore = motifptr->maxscore;
        motifptr = motifptr->next;
        }

/*****************************************************************/

motifsfilename = (char *) malloc (strlen (outfilename) + 8);
strcpy (motifsfilename, outfilename);
strcat (motifsfilename,".motifs");

if (!(motifsfile = fopen (motifsfilename,"w")))
        {
        motifsfile = stdout;
        printf ("****************** W A R N I N G **********************\n");
        printf ("Cannot write %s file, printing suggested words to Standard Output\n\n", motifsfilename);
        printf ("*******************************************************\n");
        }

fprintf (motifsfile,"\nSuggested Words : \n\n");

motifptr = motif;

while (motifptr != NULL)
        {
        fprintf (motifsfile,"\t%4d - %4d\t: ",motifptr->begin+1,motifptr->end);
        for (i = motifptr->begin; i< motifptr->end+1;i++)
                {
                fprintf (motifsfile,"%c",*(conserved+i));
                }
        fprintf (motifsfile,"  \t:%8.2f\n\n",motifptr->maxscore);
        motifptr = motifptr->next;
        }

if (motifsfile != stdout) fclose (motifsfile);


/*****************************************************************/
/*** Plots main 'profile' to *.prof file                      ****/
/*****************************************************************/

maxvalue = profilplot (profiltotal,trimmed,length,outfilename,conserved,first,maxprofile);

/*****************************************************************/


/*****************************************************************/
/*** Plots 'profile' from TBlastN if required                 ****/
/*****************************************************************/

if ((getargbool("-plotn") == 1) && (tblastnsearch == 1))
        {
        outfilename2 = (char *) malloc (strlen (outfilename) + 4);
        strcpy (outfilename2,outfilename);
        strcat (outfilename2,"-nt");
        profilplot (profiltotal2,trimmed2,length2,outfilename2,conserved2,first,maxprofile2);
        }

/*****************************************************************/


/*****************************************************************/
/*** Writes consensus sequence in FastA format                ****/
/*****************************************************************/

if (getargbool("-s") == 1) consseq (profiltotal,length,outfilename,conserved);

/*****************************************************************/


/*****************************************************************/
/*** Rescores each of the sequences producing HSPs            ****/
/*****************************************************************/

if (getargbool ("-anchors") == 1)
        {
        anchorfilename = (char *) malloc (strlen (outfilename) + 9);
        strcpy (anchorfilename, outfilename);
        strcat (anchorfilename,".anchors");

        if (!(anchorfile = fopen (anchorfilename,"w")))
                {
                printf ("****************** W A R N I N G **********************\n");
                printf ("*** Cannot write %s anchors file ***\n\n",anchorfilename);
                printf ("*******************************************************\n");
                anchorfile = NULL;
                }
        if (anchorfile) fprintf (anchorfile,"Ballast %f\n",maxmotifscore);

        }

if (getargbool ("-table") ==1 )
        {
        tablefilename = (char *) malloc (strlen (outfilename) + 7);
        strcpy (tablefilename, outfilename);
        strcat (tablefilename, ".table");
        if (!(tablefile = fopen (tablefilename,"w")))
                {
                printf ("****************** W A R N I N G **********************\n");
                printf ("*** Cannot write %s table file ***\n\n",tablefilename);
                printf ("*******************************************************\n");
                tablefile = NULL;
                }
        else
                {
                for(motifptr = motif; motifptr != NULL; motifptr = motifptr->next)
                        {
                        fprintf(tablefile,"%d ",motifptr->n);
                        nmotifs = motifptr->n;
                        }
                }
        }

seqres = first;
while (seqres != NULL)
        {
        seqres->aligned = NULL;

#ifdef DEBUG
        printf ("***%s\n",seqres->desc);
#endif

        /*****************************************************************/
        /*** Identifies overlapping motifs/HSPs **************************/
        /*****************************************************************/

        motifdb = getsbjmotifs (motif,seqres->sim,trimmed,seqres->type);


        if (motifdb != NULL)
                {

                /********************************************************/
                /*** Sorts HSPs by order of position                 ****/
                /********************************************************/

                firstmotdb = sortbybegdb (motifdb);


#ifdef DEBUG
                motifdb= firstmotdb;
                while (motifdb!= NULL)
                        {
                        printf ("%5d-%5d (%d|%d) <=> %5d-%5d : %f/%f\n",motifdb->begdb,motifdb->enddb,motifdb->begin,motifdb->end,motifdb->motif->begin,motifdb->motif->end,motifdb->score,motifdb->motif->maxscore);
                        motifdb = motifdb->sortednext;
                        }
#endif

                /*******************************************************/
                /*** Align motifs and HSPs and rescores the sequence ***/
                /*******************************************************/

                seqres->sc = MHalign (motif,firstmotdb,trimmed,seqres->type,&firstalnmotdb);


        if (seqres->type=='p')
                {
                if ((tablefile) && (seqres->p < tableseuil))
                        {
                        prevn = 0;
                        fprintf (tablefile,"\n%-15s ",seqres->name);
                        }

                seqres->aligned = (char *) malloc (length+1);
                for (i=0;i<length;i++)
                        {
                        ptrseq = (char *) (seqres->aligned + i);
                        *ptrseq = '.';
                        }
                ptrseq = (char *) (seqres->aligned + length );
                *ptrseq = '\0';

                motifdb= firstalnmotdb;
                while (motifdb!= NULL)
                        {
                        ptrstr = (char *) malloc (motifdb->enddb-motifdb->begdb + 2);
                        strncpy (ptrstr,motifdb->hsp->hsp + motifdb->begdb - motifdb->hsp->begdb ,motifdb->enddb-motifdb->begdb+1);
                        lastchar = (char *) (ptrstr + motifdb->enddb-motifdb->begdb+1);
                        *lastchar = '\0';
                        /*weight = MAX (0,1-motifdb->hsp->p);*/
                        weight = motifdb->hsp->score;
                        consmotif (motifdb->motif,ptrstr,motifdb->begin - motifdb->motif->begin, weight, weight);
                        if (((seqres->p) < msfseuil))
                                {
                                if (anchorfile)
                                        {
                                        if (getargchar ("-queryname",&queryname))
                                                {
                                                fprintf (anchorfile,"seq: %15s %15s\tpos: %10d\tbeg: %10d %10d\tlen: %6d\tweight: %6.2f\n",queryname,seqres->name,motifdb->motif->peak,motifdb->begin+1,motifdb->begdb+1,motifdb->enddb-motifdb->begdb+1,100.0*motifdb->score/maxmotifscore);
                                                }
                                        else
                                                {
                                                if (seqres != first) {
                                                        fprintf (anchorfile,"seq: %15s %15s\tpos: %10d\tbeg: %10d %10d\tlen: %6d\tweight: %6.2f\n",first->name,seqres->name,motifdb->motif->peak,motifdb->begin+1,motifdb->begdb+1,motifdb->enddb-motifdb->begdb+1,100.0*motifdb->score/maxmotifscore);
                                                        }
                                                }
                                        }
                                for (i=0;i<(motifdb->enddb-motifdb->begdb+1);i++)
                                        {
                                        ptrseq = (char *) (seqres->aligned + motifdb->begin + i);
                                        *ptrseq=*(ptrstr+i);
                                        }
                                }
                        free (ptrstr);

                        if ((tablefile) && (seqres->p < tableseuil))
                                {
                                mismatches = motifdb->motif->n - prevn - 1;
                                for (i=0; i<mismatches;i++)
                                        {
                                        fprintf (tablefile,"-1.00 ");
                                        }
                                fprintf (tablefile,"%5.2f ",motifdb->score/motifdb->motif->maxscore);
                                prevn = motifdb->motif->n;
                                }

                        motifdb = motifdb->alnnext;
                        }

#ifdef DEBUG
                        if ((seqres->type=='p') && (seqres->p < msfseuil))
                                {
                        printf ("%14s %s\n",seqres->name,seqres->aligned);
                        }
#endif

                }
                freesbjmotifs (firstmotdb);

                if ((tablefile) && (seqres->p < tableseuil) && (prevn < nmotifs))
                        {
                        mismatches = nmotifs - prevn ;

                        for (i=0; i<mismatches;i++)
                                {
                                fprintf (tablefile,"-1.00 ");
                                }
                        }

                seqres->nmatch = countmatches(seqres->sim);
                }

        seqres->twins = NULL;
        seqres=seqres->next;
        }

if (anchorfile) fclose (anchorfile);

if (tablefile) {
        fprintf (tablefile,"\n");
        fclose (tablefile);
        }

#ifdef DEBUG
        printf ("         Query %s\n",conserved);
#endif

/*****************************************************************/
/*** Writes MSF file                                          ****/
/*****************************************************************/

msffilename = (char *) malloc (strlen (outfilename) + 5);
strcpy (msffilename, outfilename);
strcat (msffilename,".msf");

if (!(msffile = fopen (msffilename,"w")))
        {
        printf ("**** Could not write %s MSF file ****\n\n",msffilename);
        }
else
        {
        printmsf (conserved, length, first, msfseuil, msffile);
        fclose (msffile);
        }

/*****************************************************************/


/*****************************************************************/
/*** Writes results to output file                            ****/
/*****************************************************************/


if (!(outfile = fopen(outfilename,"w")))
        {
        printf ("**** Could not write %s MSF file ****\n",msffilename);
        printf ("**** Sending results to Standard Output\n\n");
        outfile = stdout;
        }

printheader(blhd1, blhd2, outfile, outfilename);

printstats (first, maxvalue, outfile);

#ifndef WWW
printqueryseq (outfile, conserved, profiltotal, length);

printqueryseq (outfile, conserved, trimmed, length);
#endif

first  = (SeqHSP *) sortbysc(first);

redundout (first);

if (getargchar ("-mask",&maskfilename) == NULL)
        {
        riseup (&first);
        }

printresout (first,outfile,maxscore);

fprintf (outfile,"Done...\n");
if (outfile != stdout) fclose (outfile);

showmotifs (motif,'p', 0.75, 0.10);

printf ("Done...\n");

/*****************************************************************/

return(0);
}