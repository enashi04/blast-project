#include <ctype.h>
#include <expat.h>
#include "options.h"

#define BUFSIZE 8192

char buf[BUFSIZE];
char content[BUFSIZE];
FILE *output;
int t_from, t_to, query_length,number_of_hit=0;

struct
{
    char query[128];
    char access;
} state;

/** @brief Open the tag for the Bronze Mode
    @param data
    @param name
    @param attrs
*/
void bronze_tag_start(void *data, const char *name, const char **attrs)
{
    if (strcmp(name, "Hit_accession") == 0)
    {
        state.access = 5;
    }
    if (strcmp(name, "Hsp_num") == 0)
    {
        state.access = 6;
    }
    if (strcmp(name, "Hsp_score") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_evalue") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_align-len") == 0)
    {
        state.access = 2;
    }
}

/** @brief Open the tag for the Silver Mode
    @param data
    @param name
    @param attrs
*/
void silver_tag_start(void *data, const char *name, const char **attrs)
{

    if (strcmp(name, "Hit_accession") == 0)
    {
        state.access = 5;
    }
    if (strcmp(name, "Hsp_num") == 0)
    {
        state.access = 6;
    }
    if (strcmp(name, "Hsp_score") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_evalue") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_identity") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_positive") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_gaps") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_align-len") == 0)
    {
        state.access = 2;
    }
}
/** @brief Open the tag for the Gold Mode
    @param data
    @param name
    @param attrs
*/
void gold_tag_start(void *data, const char *name, const char **attrs)
{

    if (strcmp(name, "Hit_accession") == 0)
    {
        state.access = 5;
    }
    if (strcmp(name, "Hsp_num") == 0)
    {
        state.access = 6;
    }
    if (strcmp(name, "Hsp_bit-score") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_score") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_evalue") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_query-from") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_query-to") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_hit-from") == 0)
    {
        state.access = 3;
    }
    if (strcmp(name, "Hsp_hit-to") == 0)
    {
        state.access = 4;
    }
    if (strcmp(name, "Hsp_identity") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_positive") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_gaps") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Hsp_align-len") == 0)
    {
        state.access = 2;
    }
    if (strcmp(name, "Iteration_query-len") == 0)
    {
        state.access = 7;
    }
}

/** @brief Closing the tag
    @param data
    @param name
*/
void tag_end(void *data, const char *name)
{
}

/** @brief Read the tag content
    @param data
    @param text
    @param len
*/
void tag_value(void *data, const char *text, int len)
{
    strncpy(content, text, len);
    content[len] = '\0';

    if (state.access == 1)
    {
        fprintf(output, "%s,", content);
        strcpy(content, state.query);
        state.access = 0;
    }
    if (state.access == 2) //pour la longueur d'alignement
    {
        
        int query_cover = 100 * (t_to - t_from)/query_length;
       // fprintf(stdout, "t_from : %u-t_to : %u /query_length %d\n",t_from, t_to, query_cover);
        fprintf(output, "%u,%s\n", query_cover,content);
        strcpy(content, state.query);
        state.access = 0;
    }
    if(state.access==3){
        t_from=atoi(content);
       // fprintf(stdout, "target from is %u\n", t_from);
        fprintf(output, "%s,", content);
        strcpy(content, state.query);
        state.access = 0;
    }
    if(state.access==4){
        t_to=atoi(content);
       // fprintf(stdout, "target to is %u\n", t_to);
        fprintf(output, "%s,", content);
        strcpy(content, state.query);
        state.access = 0;
    }
    if (state.access == 5) // pour le nom de l'hit
    {
        fprintf(output, "%s,", content);
        strcpy(content, state.query);
        state.access = 0;
    }
    if (state.access == 6) // pour le numéro du hit
    {
        if (strncmp(content, "1", 1) == 0)
        {
            fprintf(output, "%s,", content);
            strcpy(content, state.query);
            state.access = 0;
        }
        else
        {
            fprintf(output, "-,%s,", content);
            strcpy(content, state.query);
            state.access = 0;
        }
    }
    if(state.access ==7){
        query_length=atoi(content);
        //fprintf(stdout, "la query length est %u\n", query_length);
        strcpy(content, state.query);
        state.access = 0;
    }
}

/** @brief display errors and close file f
    @param f
    @param parser
*/
void test_error(FILE *f, XML_Parser parser)
{
    int done;
    do
    {
        int len = fread(buf, 1, BUFSIZE, f);

        if (ferror(f))
        {
            fprintf(stderr, "Read error\n");
            break;
        }
        done = feof(f);

        if (XML_Parse(parser, buf, len, done) == XML_STATUS_ERROR)
        {
            fprintf(stderr, "Error while parsing XML\n");
            break;
        }
    } while (!done);

    XML_ParserFree(parser);
    fclose(f);
}

int main(int argc, char **argv)
{
    // si on n'a pas d'argument
    if (argc == 1)
    {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "\t./parsing -option <argument>\n\n");
        fprintf(stderr, "-h or help to see options\n");
        exit(1);
    }

    // Affichage du help si besoin
    help(argv);
    char *inputName = NULL;

    inputName = inputRecovery(argc, argv, inputName);
    // récupération des valeurs passées en arguments
    char *mode = '\0';
    mode = modeChoice(argc, argv, mode);
    char *outName = NULL;
    outName = outputName(argc, argv, outName);
    //printf("l'outname est %s\n", outName);
    // si l'user met des options non valides
    invalidOptions(argc, argv);
    // ouverture du fichier pour mettre le résultat
    FILE *f = fopen(inputName, "r");

    if (outName != NULL)
    {
        output = fopen(outName, "w");
    }
    else
    {
        output = stdout;
    }

    // récupérer le nom du fichier sans extension

    if (strcmp(mode, "bronze") == 0)
    {
        fprintf(stderr, "You chose the Bronze mode !\n");

        //fprintf(output, "Bronze results \n\n");
        fprintf(output, "id, num, score, evalue,align-length \n");

        XML_Parser parser = XML_ParserCreate(NULL);
        XML_SetElementHandler(parser, bronze_tag_start, tag_end);
        XML_SetCharacterDataHandler(parser, tag_value);

        state.access = 0;
        test_error(f, parser);
        if (outName != NULL)
        {
            fprintf(stderr, "You can open the file: %s\n", outName);
        }
    }
    else if (strcmp(mode, "silver") == 0)
    {
        fprintf(stdout, "You chose the Silver mode !\n");
        //fprintf(output, "Silver results\n\n");
        fprintf(output, "id,hit,score,evalue,identity,positive,gaps,align-length \n");

        // fprintf(stdout, "Silver results of %s\n\n", name);
        // fprintf(stdout, "id,hit,score,evalue,identity,positive,gaps,align-length \n");

        XML_Parser parser = XML_ParserCreate(NULL);
        XML_SetElementHandler(parser, silver_tag_start, tag_end);
        XML_SetCharacterDataHandler(parser, tag_value);

        state.access = 0;
        test_error(f, parser);

        if (outName != NULL)
        {
            fprintf(stderr, "You can open the file: %s\n", outName);
        }
    }
    else if (strcmp(mode, "gold") == 0)
    {
        fprintf(stderr, "You chose the Gold mode !\n");

        //fprintf(output, "Gold results\n\n");
        fprintf(output, "id,hit,bitscore,score,evalue,query-from,query-to,target-from,target-to,identity,positive,gaps,query-cover,align-length \n");

        // fprintf(stdout, "Gold results of %s\n\n", name);
        // fprintf(stdout, "id,hit,bitscore,score,evalue,query-from,query-to,target-from,target-to,identity,positive,gaps,align-length \n");

        XML_Parser parser = XML_ParserCreate(NULL);
        XML_SetElementHandler(parser, gold_tag_start, tag_end);
        XML_SetCharacterDataHandler(parser, tag_value);

        state.access = 0;
        test_error(f, parser);

        if (outName != NULL)
        {
            fprintf(stderr, "You can open the file: %s\n", outName);
        }
    }

    return 0;
}