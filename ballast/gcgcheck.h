#include <ctype.h>   /* because of toupper() */

int SeqGCGCheckSum(char *seq, int len)
{
        int  i;
        long check;

/*
        for( i=0, check=0; i< len; i++,seq=(char *)(seq+1))
*/
        for( i=0, check=0; i< len; i++)
                check += ((i % 57)+1) * (int)(*((char *)(seq+i)));
/*
                check += ((i % 57)+1) * toupper(*((char *)(seq+i)));
*/

        return(check % 10000);
}

