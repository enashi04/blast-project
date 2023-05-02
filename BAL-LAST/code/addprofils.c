/**
 * @file addprofils.c
 * @author J.Thompson
 * @brief
 * @version 0.1
 * @date 2000-03
 *
 * @copyright Copyright (c) 2022
 *
 */

/*
 *Addprofils : 
 
*/
void addprofils(double *profil1, double *profil2, int length)
{
    int i;
    double *ptr;

    for (i = 0; i < length; i++)
    {
        ptr = (double *)(profil1 + i);
        (*ptr) += *(profil2 + i);
    }
}