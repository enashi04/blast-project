typedef struct BlastHeader
        {
        char          *prog;
        char          *query;
        char          *dbase;
        } BlastHeader;

typedef struct SIMPRF
{
        double        *prf;
        double         score;
        double         maxscore;
        int            nmatch;
        int            begin;
        int            end;
        int            begmot;
        int            endmot;
        int            begdb;
        int            enddb;
        char           sens;
        struct SIMPRF *next;
        struct SIMPRF *sortedprev;
        struct SIMPRF *sortednext;
        char          *text;
        char          *queryseq;
        char          *hsp;
        char          *aln;
        float          p;
        int            pcid;
        int            nid;
} SimPrf;


typedef struct SEQHSP
        {
        char          *name;
        char          *access;
        char          *desc;
        int            rank;
        double         sc;
        int            nmatch;
        float          prob;
        float          p;
        struct SIMPRF *sim;
        char          *aligned;
        char          *outtext;
        char          *twins;
        struct SEQHSP *prev;
        struct SEQHSP *next;
        struct SEQHSP *sortedprev;
        struct SEQHSP *sortednext;
        char           type;
        char           risedup;
        int            check;
        } SeqHSP;
typedef struct PSSM
{
        float          w[26];
        float          n;
        struct PSSM   *next;
        struct PSSM   *prev;
} PSSM;

typedef struct MOTIF
        {
        int            begin;
        int            end;
        int            peak;
        int            n;
        struct SIMPRF *hsp;
        struct SIMPRF *alnhsp;
        double         score;
        double         alnscore;
        double maxscore;
        struct MOTIF  *prev;
        struct MOTIF  *next;
        struct PSSM   *profile;
        } Motif;

typedef struct SBJMOT
        {
        struct SIMPRF *hsp;
        struct MOTIF  *motif;
        int            begin;
        int            end;
        int            begdb;
        int            enddb;
        struct SBJMOT *next;
        struct SBJMOT *sortedprev;
        struct SBJMOT *sortednext;
        double         score;
        double         alnscore;
        struct SBJMOT *alnnext;
        struct SBJMOT *alnprev;
        } Sbjmot;




