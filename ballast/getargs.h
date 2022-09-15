#define _READARGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readargs.h"

int initargs(char *argv[], int argc, Argdef *arg_def, int numargs)
{
        int i, j, s;
        int found;
        Argument *ptr;

        if (args != NULL)
                free(args);
        args = NULL;

        for (i = 0; i < numargs; i++)
        {
                found = 0;
                for (j = 0; j < argc; j++)
                {
                        if (strcmp(argv[j], arg_def[i].flag) == 0)
                        {
                                found = 1;
                                if (args == NULL)
                                {
                                        args = (Argument *)malloc(sizeof(Argument));
                                        args->prev = NULL;
                                        args->next = NULL;
                                        ptr = args;
                                }
                                else
                                {
                                        ptr->next = (Argument *)malloc(sizeof(Argument));
                                        (ptr->next)->prev = ptr;
                                        ptr = ptr->next;
                                        ptr->next = NULL;
                                }

                                ptr->flag = argv[j];

                                if ((arg_def[i].nval) == 0)
                                {
                                        ptr->val = ptr->flag;
                                }
                                else
                                {
                                        ptr->val = argv[j + 1];
                                }
                        }
                }
                if ((arg_def[i].compulsory != NULL) && (found == 0))
                        return (-1);
        }
        return (1);
}

char *getargchar(char *flag, char **val)
{
        Argument *ptr;

        ptr = args;
        while (ptr != NULL)
        {
                if (strcmp(ptr->flag, flag) == 0)
                {
                        *val = ptr->val;
                        return (*val);
                }
                ptr = ptr->next;
        }
        return (NULL);
}

int *getargint(char *flag, int *i)
{
        Argument *ptr;

        ptr = args;
        while (ptr != NULL)
        {
                if (strcmp(ptr->flag, flag) == 0)
                {
                        sscanf(ptr->val, "%d", i);
                        return (i);
                }
                ptr = ptr->next;
        }
        return (NULL);
}

float *getargfloat(char *flag, float *f)
{
        Argument *ptr;

        ptr = args;
        while (ptr != NULL)
        {
                if (strcmp(ptr->flag, flag) == 0)
                {
                        sscanf(ptr->val, "%f", f);
                        return (f);
                }
                ptr = ptr->next;
        }
        return (NULL);
}

double *getargdouble(char *flag, double *f)
{
        Argument *ptr;

        ptr = args;
        while (ptr != NULL)
        {
                if (strcmp(ptr->flag, flag) == 0)
                {
                        sscanf(ptr->val, "%lf", f);
                        return (f);
                }
                ptr = ptr->next;
        }
        return (NULL);
}

int getargbool(char *flag)
{
        Argument *ptr;

        ptr = args;
        while (ptr != NULL)
        {
                if (strcmp(ptr->flag, flag) == 0)
                        return (TRUE);
                ptr = ptr->next;
        }
        return (FALSE);
}