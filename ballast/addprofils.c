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