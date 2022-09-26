#define TRUE 1
#define FALSE 0

#define INT 1
#define FLOAT 2
#define CHAR 3
#define BOOL 4

typedef struct ARGDEF
{
    char *flag;
    char type;
    int nval; /* number of values NOT IMPLEMENTED */
    char compulsory;
} ARGDEF;

#ifndef _READARGS

extern int initargs(char *argv[], int argc, ARGDEF *arg_def, int numargs);
extern int *getargint(char *flag, int *i);
extern float *getargfloat(char *flag, float *f);
extern float *getargdouble(char *flag, double *f);
extern int getargbool(char *flag);
extern char *getargchar(char *flag, char **val);

#else

typedef struct ARG
{
    char *flag;
    char *val;
    char type;
    struct ARG *next;
    struct ARG *prev;
} Argument;

static Argument *args;

int initargs(char *argv[], int argc, ARGDEF *arg_def, int numargs);
int *getargint(char *flag, int *i);
float *getargfloat(char *flag, float *f);
int getargbool(char *flag);
char *getargchar(char *flag, char **val);

#endif