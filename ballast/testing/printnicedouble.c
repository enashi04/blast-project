/*** printnicedouble *********************************************/
/*                                                               */
/*   Prints double floating values nicely                        */
/*   No return value.                                            */
/*                                                               */
/*****************************************************************/
/*  double 	p    : value to print                            */
/*  FILE    *outfile : output file                               */
/*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readargs.h"
#include "types.h"

void printnicedouble(FILE *outfile, double p)
{
    char str[7];

    if ((int)(p) == p)
    {
        if (p < 10)
        {
            fprintf(outfile, " %2.1f     ", p);
            return;
        }
        else
        {
            fprintf(outfile, " %5.0f   ", p);
            return;
        }
    }

    if (p < 1.0e-4)
    {
        fprintf(outfile, " %2.1e ", p);
        return;
    }

    sprintf(str, "%5.4f", p);

    /*
    if (strcmp(&str[3],"000") == 0) {fprintf (outfile," %2.1f     ",p);return;}

    if (strcmp(&str[4], "00") == 0) {fprintf (outfile," %3.2f    " ,p);return;}

    if (strcmp(&str[5],  "0") == 0) {fprintf (outfile," %4.3f   "  ,p);return;}
    */

    fprintf(outfile, " %5.4f  ", p);
    return;
}
