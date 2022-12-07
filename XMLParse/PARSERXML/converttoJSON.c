// Conversion en JSON
#include "converttoJSON.h"

char name_hit[128];
char name_species[128];



char *getSpecies(char *content)
{
    char *def = (char *)malloc(strlen(content));
    for (int i = 0; i < strlen(content); i++)
    {
        if (content[i] == '[')
        {
            memmove(def, content + i, strlen(content));
            break;
        }
    }
    char *species = (char *)malloc(strlen(def));
    int j = 0;
    for (int i = 0; i < strlen(def); i++)
    {
        if (def[i] != '[' && def[i] != ']')
        {
            species[j] = def[i];
            j++;
        }
        else if (def[i] == ']')
        {
            break;
        }
    }
    // printf("%s : is the word \n", newword);
    return species;
}

/*****************************************************************/
/***********************Retrieve hit info*************************/
/*****************************BRONZE******************************/
/*****************************************************************/
void bronze_tag_start(void *data, const char *name, const char **attrs)
{
    if (strcmp(name, "Iteration_query-def") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Iteration_query-len") == 0)
    {
        state.access = 2;
    }
    if (strcmp(name, "Hit_accession") == 0)
    {
        state.access = 3;
    }
    if (strcmp(name, "Hsp_num") == 0)
    {
        state.access = 4;
    }
    if (strcmp(name, "Hsp_bit-score") == 0)
    {
        state.access = 5;
    }
    if (strcmp(name, "Hsp_score") == 0) // mettre la query cover après le score
    {
        state.access = 6;
    }
    if (strcmp(name, "Hsp_evalue") == 0)
    {
        state.access = 7;
    }
    if (strcmp(name, "Hsp_query-from") == 0)
    {
        state.access = 8;
    }
    if (strcmp(name, "Hsp_query-to") == 0)
    {
        state.access = 9;
    }
    if (strcmp(name, "Hsp_hit-from") == 0)
    {
        state.access = 10;
    }
    if (strcmp(name, "Hsp_hit-to") == 0)
    {
        state.access = 11;
    }
    if (strcmp(name, "Hsp_identity") == 0)
    {
        state.access = 12;
    }
    if (strcmp(name, "Hsp_positive") == 0)
    {
        state.access = 13;
    }
    if (strcmp(name, "Hsp_gaps") == 0)
    {
        state.access = 14;
    }
    if (strcmp(name, "Hsp_align-len") == 0)
    {
        state.access = 15;
    }
    if (strcmp(name, "Hit_def") == 0)
    { // pour récupérer le nom de l'espèce entre []
        state.access = 16;
    }
}

/*****************************************************************/
/***********************Retrieve hit info*************************/
/*****************************SILVER******************************/
/*****************************************************************/

void silver_tag_start(void *data, const char *name, const char **attrs)
{
    if (strcmp(name, "Iteration_query-def") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Iteration_query-len") == 0)
    {
        state.access = 2;
    }
    if (strcmp(name, "Hit_accession") == 0)
    {
        state.access = 3;
    }
    if (strcmp(name, "Hsp_num") == 0)
    {
        state.access = 4;
    }
    if (strcmp(name, "Hsp_bit-score") == 0)
    {
        state.access = 5;
    }
    if (strcmp(name, "Hsp_score") == 0) // mettre la query cover après le score
    {
        state.access = 6;
    }
    if (strcmp(name, "Hsp_evalue") == 0)
    {
        state.access = 7;
    }
    if (strcmp(name, "Hsp_query-from") == 0)
    {
        state.access = 8;
    }
    if (strcmp(name, "Hsp_query-to") == 0)
    {
        state.access = 9;
    }
    if (strcmp(name, "Hsp_hit-from") == 0)
    {
        state.access = 10;
    }
    if (strcmp(name, "Hsp_hit-to") == 0)
    {
        state.access = 11;
    }
    if (strcmp(name, "Hsp_identity") == 0)
    {
        state.access = 12;
    }
    if (strcmp(name, "Hsp_positive") == 0)
    {
        state.access = 13;
    }
    if (strcmp(name, "Hsp_gaps") == 0)
    {
        state.access = 14;
    }
    if (strcmp(name, "Hsp_align-len") == 0)
    {
        state.access = 15;
    }
    if (strcmp(name, "Hit_def") == 0)
    { // pour récupérer le nom de l'espèce entre []
        state.access = 16;
    }
}

/*****************************************************************/
/***********************Retrieve hit info*************************/
/******************************GOLD*******************************/
/*****************************************************************/

void gold_tag_start(void *data, const char *name, const char **attrs)
{
    if (strcmp(name, "Iteration_query-def") == 0)
    {
        state.access = 1;
    }
    if (strcmp(name, "Iteration_query-len") == 0)
    {
        state.access = 2;
    }
    if (strcmp(name, "Hit_accession") == 0)
    {
        state.access = 3;
    }
    if (strcmp(name, "Hsp_num") == 0)
    {
        state.access = 4;
    }
    if (strcmp(name, "Hsp_bit-score") == 0)
    {
        state.access = 5;
    }
    if (strcmp(name, "Hsp_score") == 0) // mettre la query cover après le score
    {
        state.access = 6;
    }
    if (strcmp(name, "Hsp_evalue") == 0)
    {
        state.access = 7;
    }
    if (strcmp(name, "Hsp_query-from") == 0)
    {
        state.access = 8;
    }
    if (strcmp(name, "Hsp_query-to") == 0)
    {
        state.access = 9;
    }
    if (strcmp(name, "Hsp_hit-from") == 0)
    {
        state.access = 10;
    }
    if (strcmp(name, "Hsp_hit-to") == 0)
    {
        state.access = 11;
    }
    if (strcmp(name, "Hsp_identity") == 0)
    {
        state.access = 12;
    }
    if (strcmp(name, "Hsp_positive") == 0)
    {
        state.access = 13;
    }
    if (strcmp(name, "Hsp_gaps") == 0)
    {
        state.access = 14;
    }
    if (strcmp(name, "Hsp_align-len") == 0)
    {
        state.access = 15;
    }
    if (strcmp(name, "Hit_def") == 0)
    { // pour récupérer le nom de l'espèce entre [] ou OS
        state.access = 16;
    }
}
void tag_end(void *data, const char *name)
{
}

/*****************************************************************/
/***********************Put info in JSON**************************/
/*****************************Format******************************/
/*****************************************************************/

void tag_value(void *data, const char *text, int len)
{
    strncpy(content, text, len);
    content[len] = '\0';

    switch (state.access)
    {
    case 1:
        fprintf(output, "{\n\t\"query-name\" : \"%s\",\n", content);
        strcpy(content, state.query);
        state.access = 0;
        break;
    case 2:
        fprintf(output, "\t\"query-len\" : \"%u\",\n\t\"hits\":\n\t[\n", atoi(content));
        query_length = atoi(content);
        strcpy(content, state.query);
        state.access = 0;
        break;
    case 3:
        if (strcmp(name_hit, "") != 0)
        {
            strcpy(name_hit, content);
            fprintf(output, ",\n\t\t{\n\t\t\t\"hit_accession\" : \"%s\",\n", content);
            strcpy(content, state.query);
            state.access = 0;
        }
        else
        {
            strcpy(name_hit, content);
            fprintf(output, "\t\t{\n\t\t\t\"hit_accession\" : \"%s\",\n", content);
            strcpy(content, state.query);
            state.access = 0;
        }
        break;
    case 4:
        if (atoi(content) == 1)
        {
            fprintf(output, "\t\t\t\"nb_hit\" : \"%u\",\n", atoi(content));
            strcpy(content, state.query);
            state.access = 0;
            break;
        }
        else
        {
            fprintf(output, "\n,{\n\t\t\t\"hit_accession\" : \"%s\",\n", name_hit);
            fprintf(output, "\t\t\t\"nb_hit\" : \"%u\",\n", atoi(content));
            strcpy(content, state.query);
            state.access = 0;
            break;
        }
    case 5:
        fprintf(output, "\t\t\t\"bitscore\" : \"%u\",\n", atoi(content));
        strcpy(content, state.query);
        state.access = 0;
        break;
    case 6:
        fprintf(output, "\t\t\t\"score\" : \"%u\",\n", atoi(content));
        strcpy(content, state.query);
        state.access = 0;
        break;
    case 7:
        fprintf(output, "\t\t\t\"evalue\" : \"%u\",\n", atoi(content));
        strcpy(content, state.query);
        state.access = 0;
        break;
    case 8:
        fprintf(output, "\t\t\t\"query-from\" : \"%u\",\n", atoi(content));
        strcpy(content, state.query);
        state.access = 0;
        break;
    case 9:
        fprintf(output, "\t\t\t\"query-to\" : \"%u\",\n", atoi(content));
        strcpy(content, state.query);
        state.access = 0;
        break;
    case 10:
        fprintf(output, "\t\t\t\"target-from\" : \"%u\",\n", atoi(content));
        t_from = atoi(content);
        strcpy(content, state.query);
        state.access = 0;
        break;
    case 11:
        fprintf(output, "\t\t\t\"target-to\" : \"%u\",\n", atoi(content));
        t_to = atoi(content);
        strcpy(content, state.query);
        state.access = 0;
        break;
    case 12:
        fprintf(output, "\t\t\t\"identities\" : \"%u\",\n", atoi(content));
        strcpy(content, state.query);
        state.access = 0;
        break;
    case 13:
        fprintf(output, "\t\t\t\"positives\" : \"%u\",\n", atoi(content));
        strcpy(content, state.query);
        int query_cover = 100 * (t_to - t_from) / query_length;
        fprintf(output, "\t\t\t\"query-cover\" : \"%u\",\n", query_cover);
        state.access = 0;
        break;
    case 14:
        fprintf(output, "\t\t\t\"gaps\" : \"%u\",\n", atoi(content));
        strcpy(content, state.query);
        state.access = 0;
        break;
    case 15:
        fprintf(output, "\t\t\t\"align-len\" : \"%u\"\n\t\t}", atoi(content));
        strcpy(content, state.query);
        state.access = 0;
        break;
    case 16:
        strcpy(name_species, getSpecies(content)); // ici on va mettre la fonction qui permet de récupérer que le nom de l'espèce.
        fprintf(stderr, "l'espèce est : %s et %s\n", name_species, ht_search(table, name_species));
        strcpy(content, state.query);

        state.access=0;
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
