/*** printstats **************************************************/
/*                                                               */
/*   Prints statistics about how many sequences were included    */
/*   into 'profile'                                              */
/*   No return value.                                            */
/*                                                               */
/*****************************************************************/
/* SeqHSP *first        : first sequence producing HSPs          */
/* double  maxvalue     : maximum value of the 'profile'         */
/* FILE   *outfile      : output file                            */
/*                                                               */
/* SeqHSP *seqres       : current sequence producing HSPs        */
/* int     both         : flag indicating whether both blastp &  */
/*                        tblastn results were used              */
/* int     a            : number of seq             P(N) > 0.75  */
/* int     b            : number of seq     0.5   > P(N) > 0.75  */
/* int     c            : number of seq     0.1   > P(N) > 0.5   */
/* int     d            : number of seq     0.07  > P(N) > 0.1   */
/* int     e            : number of seq     0.05  > P(N) > 0.07  */
/* int     f            : number of seq     0.01  > P(N) > 0.05  */
/* int     g            : number of seq     0.001 > P(N) > 0.01  */
/* int     h            : number of seq     0     > P(N) > 0.001 */
/* int     i            : number of seq             P(N) ~ 0     */
/* double  A            : contribution              P(N) > 0.75  */
/* double  B            : contribution      0.5   > P(N) > 0.75  */
/* double  C            : contribution      0.1   > P(N) > 0.5   */
/* double  D            : contribution      0.07  > P(N) > 0.1   */
/* double  E            : contribution      0.05  > P(N) > 0.07  */
/* double  F            : contribution      0.01  > P(N) > 0.05  */
/* double  G            : contribution      0.001 > P(N) > 0.01  */
/* double  H            : contribution      0     > P(N) > 0.001 */
/* double  I            : contribution              P(N) ~ 0     */
/*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "readargs.h"
#include "types.h"

void    printstats (SeqHSP *first, double maxvalue, FILE *outfile)
{
SeqHSP *seqres ;
int a=0,b=0,c=0,d=0,e=0,f=0,g=0,h=0,i=0;
double A=0,B=0,C=0,D=0,E=0,F=0,G=0,H=0,I=0;
int both;

both = getargbool ("-both");

for (seqres = first; seqres != NULL; seqres=seqres->next)
	{
	if ((seqres->type == 'n') && (both == FALSE)) seqres->p = 1;
	if (seqres->p == 1) continue;

	if (seqres->p >  0.75  ) {a++; A+=1-seqres->p; continue;}
	if (seqres->p >  0.5   ) {b++; B+=1-seqres->p; continue;}
	if (seqres->p >  0.1   ) {c++; C+=1-seqres->p; continue;}
	if (seqres->p >  0.07  ) {d++; D+=1-seqres->p; continue;}
	if (seqres->p >  0.05  ) {e++; E+=1-seqres->p; continue;}
	if (seqres->p >  0.01  ) {f++; F+=1-seqres->p; continue;}
	if (seqres->p >  0.0001) {g++; G+=1-seqres->p; continue;}
	if (seqres->p >  0     ) {h++; H+=1-seqres->p; continue;}
	if (seqres->p == 0     ) {i++; I+=1-seqres->p;}
	}

if ((a+b+c+d+e+f+g+h+i) != 0)
	{
	fprintf (outfile, "\nContributions to 'profile' :\n");

	fprintf (outfile, "----------------------------------------------------------\n");
	fprintf (outfile, "                   |    n    | Total Cont. | Average Cont.\n");
	fprintf (outfile, "----------------------------------------------------------\n");
	fprintf (outfile, "       P(N) ~ 0.00 |   %3d   | %8.3f",i,I);
	if (i) fprintf (outfile, "    | %8.3f",I/(double)i);

	fprintf (outfile,"\n");
	fprintf (outfile, "0.00 < P(N) < .0001|   %3d   | %8.3f",h,H);
	if (h) fprintf (outfile, "    | %8.3f",H/(double)h);

	fprintf (outfile,"\n");
	fprintf (outfile, ".0001< P(N) < 0.01 |   %3d   | %8.3f",g,G);
	if (g) fprintf (outfile, "    | %8.3f",G/(double)g);

	fprintf (outfile,"\n");
	fprintf (outfile, "0.01 < P(N) < 0.05 |   %3d   | %8.3f",f,F);
	if (f) fprintf (outfile, "    | %8.3f",F/(double)f);

	fprintf (outfile,"\n");
	fprintf (outfile, "0.05 < P(N) < 0.07 |   %3d   | %8.3f",e,E);
	if (e) fprintf (outfile, "    | %8.3f",E/(double)e);

	fprintf (outfile,"\n");
	fprintf (outfile, "0.07 < P(N) < 0.1  |   %3d   | %8.3f",d,D);
	if (d) fprintf (outfile, "    | %8.3f",D/(double)d);

	fprintf (outfile,"\n");
	fprintf (outfile, "0.1  < P(N) < 0.5  |   %3d   | %8.3f",c,C);
	if (c) fprintf (outfile, "    | %8.3f",C/(double)c);

	fprintf (outfile,"\n");
	fprintf (outfile, "0.5  < P(N) < 0.75 |   %3d   | %8.3f",b,B);
	if (b) fprintf (outfile, "    | %8.3f",B/(double)b);

	fprintf (outfile,"\n");
	fprintf (outfile, "0.75 < P(N)        |   %3d   | %8.3f",a,B);
	if (a) fprintf (outfile, "    | %8.3f",A/(double)a);

	fprintf (outfile, "\n----------------------------------------------------------\n");

	if (maxvalue == 0.0)
		{
		fprintf (outfile, "===============================================================================");
		fprintf (outfile,"\n***************************************************************************\n");
		fprintf (outfile,"***                                                                     ***\n");
		fprintf (outfile,"***                          WARNING                                    ***\n");
		fprintf (outfile,"***                                                                     ***\n");
		fprintf (outfile,"*** There was not enough information to build the profile (maximum is 0)***\n");
		fprintf (outfile,"*** You should try to modify the \'-maxp\' and/or use \'-both\' parameters. ***\n");
		fprintf (outfile,"***                                                                     ***\n");
		fprintf (outfile,"***************************************************************************");

		printf ("\n***************************************************************************\n");
		printf ("***                          WARNING                                    ***\n");
		printf ("***                                                                     ***\n");
		printf ("*** There was not enough information to build the profile (maximum is 0)***\n");
		printf ("*** You should try to modify the \'-maxp\' and/or use \'-both\' parameters. ***\n");
		printf ("***************************************************************************\n\n");
		}
	}
else
	{
        fprintf (outfile,"\n***************************************************************************\n");
        fprintf (outfile,"***                                                                     ***\n");
        fprintf (outfile,"***                          WARNING                                    ***\n");
        fprintf (outfile,"***                                                                     ***\n");
        fprintf (outfile,"*** There was no sequence satisfying the criteria to build the profile! ***\n");
        fprintf (outfile,"*** You should try to modify the \'-maxp\' and/or use \'-both\' parameters. ***\n");
        fprintf (outfile,"***                                                                     ***\n");
        fprintf (outfile,"***************************************************************************\n");
	}
fprintf (outfile, "\n===============================================================================\n\n");
}
