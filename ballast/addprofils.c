/*** addprofils **************************************************/
/*								 */
/*   Adds profile 2 to profile 1.                                */
/*   Result is stored in profile 1.                              */
/*   No return value.                                            */
/*								 */
/*****************************************************************/
/* double *profil1	: 1st profile				 */
/* double *profil2	: 2nd profile				 */
/* int     length  	: length of profiles			 */
/*								 */
/* double *ptr    	: pointer to 1st profile successive      */
/*			  positions to which we add 2nd profile	 */
/*			  successive values			 */
/*****************************************************************/

void    addprofils  (double *profil1, double *profil2, int length)
{
int i;
double *ptr;

for (i=0;i<length;i++)
	{
	ptr = (double *) (profil1 + i);
	(*ptr) += *(profil2 + i);
	}
}	
/*****************************************************************/
