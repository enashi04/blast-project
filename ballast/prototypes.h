#include "types.h"
#include <stdio.h>
#include <stdlib.h>


double *loadHSP(SeqHSP *seqres, FILE *infile, char *line, int length, char *conserved, double *maxprofile, char type);
void addprofils(double *profil1, double *profil2, int length);
double colinear(SeqHSP *seqres);
double scalaire(double *profil1, double *profil2, int begin, int end, int hspbegin, int hspend);
SeqHSP *sortbysc(SeqHSP *seqres);
SeqHSP *redundout(SeqHSP *seqres);
double profilplot(double *profil, double *profil2, int length, char *filename, char *conserved, SeqHSP *first, double *maxprofile);
void printresout(SeqHSP *first, FILE *outfile, double maxscore);
void printqueryseq(FILE *outfile, char *conserved, double *profiltotal, int length);
void printstats(SeqHSP *first, double maxvalue, FILE *outfile);
void printmsf(char *conserved, int length, SeqHSP *first, float threshold, FILE *outfile);
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
Motif *extractmotifs(double *trimmed, int length, char *conserved);
void freesbjmotifs(Sbjmot *first);
int countmatches(SimPrf *first);
int overlap(Motif *motif, SimPrf *sim);
double MHalign(Motif *motif, Sbjmot *sbjmot, double *profil, char type, Sbjmot **first);
Sbjmot *sortbybegdb(Sbjmot *sim);
Sbjmot *getsbjmotifs(Motif *motif, SimPrf *sim, double *trimmed, char type);
void riseup(SeqHSP **first);
void consmotif(Motif *motif, char *ptrstr, int begin, float weight, float maxweight);
void showmotifs(Motif *motif, char type, float thr1, float thr2);

void printusage(char *program, char *path);