#include "types.h"
#include <stdio.h>
#include <stdlib.h>


double *loadHSP(SEQHSP *seqres, FILE *infile, char *line, int length, char *conserved, double *maxprofile, char type);
void addprofils(double *profil1, double *profil2, int length);
double colinear(SEQHSP *seqres);
double scalaire(double *profil1, double *profil2, int begin, int end, int hspbegin, int hspend);
SEQHSP *sortbysc(SEQHSP *seqres);
SEQHSP *redundout(SEQHSP *seqres);
double profilplot(double *profil, double *profil2, int length, char *filename, char *conserved, SEQHSP *first, double *maxprofile);
void printresout(SEQHSP *first, FILE *outfile, double maxscore);
void printqueryseq(FILE *outfile, char *conserved, double *profiltotal, int length);
void printstats(SEQHSP *first, double maxvalue, FILE *outfile);
void printmsf(char *conserved, int length, SEQHSP *first, float threshold, FILE *outfile);
void printheader(BlastHeader blhd1, BlastHeader blhd2, FILE *outfile, char *outfilename);
int initresfile(FILE *infile, char *line, BlastHeader *blhd);
double *trimprofil(double *profiltotal, double *profil, int length, char *cons);
double *trimprofilext(double *profiltotal, double *profil, int length, char *cons);
double *smoothprofil(double *profil, int length, char *cons);
double *maskprofil(double *profil, int length, char *mask);
void readprofil(char *nomprofil, double *profil);
char *readmask(char *maskfilename, char **mask);
int filter(char *SEQHSP, char *seq, int offset);
void consseq(double *profiltotal, int length, char *outfilename, char *conserved);
MOTIF *extractmotifs(double *trimmed, int length, char *conserved);
void freesbjmotifs(SBJMOT *first);
int countmatches(SIMPRF *first);
int overlap(MOTIF *motif, SIMPRF *sim);
double MHalign(MOTIF *motif, SBJMOT *sbjmot, double *profil, char type, SBJMOT **first);
SBJMOT *sortbybegdb(SBJMOT *sim);
SBJMOT *getsbjmotifs(MOTIF *motif, SIMPRF *sim, double *trimmed, char type);
void riseup(SEQHSP **first);
void consmotif(MOTIF *motif, char *ptrstr, int begin, float weight, float maxweight);
void showmotifs(MOTIF *motif, char type, float thr1, float thr2);

void printusage(char *program, char *path);