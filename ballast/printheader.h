/*** printheader *************************************************/
/*                                                               */
/*   Prints Header to Ballast result file                        */
/*   No return value.                                            */
/*                                                               */
/*****************************************************************/
/* BlastHeader  blhd1       : BLASTP header                      */
/* BlastHeader  blhd2       : TBLASTN header                     */
/* FILE        *outfile     : output file                        */
/* char        *outfilename : output file name                   */
/*                                                               */
/* char        *prffilename : pointer to 'profile' files names   */
/* float        threshold   : threshold for output               */
/* double       maxp        : max E(N) to include in profile     */
/*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readargs.h"
#include "types.h"

void printheader (BlastHeader blhd1, BlastHeader blhd2, FILE *outfile, char *outfilename)
{
char *prffilename;
float threshold;
double maxp;

fprintf (outfile,"\nBALLAST on the following BLAST jobs : \n");

/*****************************************************************/
/**** Prints out Blast searches information **********************/
/*****************************************************************/

fprintf (outfile,"-------------------------------------------------------------------------------\n");
if (blhd1.prog != NULL) fprintf (outfile,"%s%s%s\n",blhd1.prog,blhd1.query,blhd1.dbase);
if (blhd2.prog != NULL) fprintf (outfile,"%s%s%s\n",blhd2.prog,blhd2.query,blhd2.dbase);
fprintf (outfile,"-------------------------------------------------------------------------------\n");

/*****************************************************************/


/*****************************************************************/
/**** Prints 'profile' file information  *************************/
/*****************************************************************/

prffilename  = (char *) malloc (strlen (outfilename) + 6);
sprintf (prffilename,"%s.prof",outfilename);

#ifndef WWW
if (getargbool("-both") == TRUE)
        {
        fprintf (outfile,"The \'profile\' derived from both blastp and tblastn searches is in file %s\n",prffilename);
        }
else
        {
        fprintf (outfile,"The \'profile\' derived from blastp search is in file %s\n",prffilename);
        }

if (getargbool("-plotn") == TRUE)
        {
        prffilename  = (char *) realloc (prffilename,strlen (outfilename) + 9);
        sprintf (prffilename,"%s-nt.prof",outfilename);
        fprintf (outfile,"The \'profile\' derived from tblastn search is in file %s\n",prffilename);
        }
#endif

if (getargdouble ("-maxp",&maxp) == NULL) maxp = 0.1;
fprintf (outfile,"\nOnly sequences with E(N) < %f were used to build the \'profile\'\n",maxp);

/*****************************************************************/


/*****************************************************************/
/**** Prints out Threshold information  **************************/
/*****************************************************************/
#ifndef WWW
if (getargfloat ("-t",&threshold) == NULL) threshold = 0.0;
fprintf (outfile,"\n\nOnly sequences with threshold > %f are presented below\n",threshold);
#endif

/*****************************************************************/

/*****************************************************************/
/**** Were the HSPs orted by position ?  *************************/
/*****************************************************************/

if (getargbool("-nosort") == FALSE)
        {
        fprintf (outfile,"\nHSPs are sorted for every Subject Sequence by ascending start position\n");
        }

fprintf (outfile,"\n===============================================================================\n");

/*****************************************************************/

free (prffilename);
return ;
}