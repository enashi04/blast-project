#include "tagParse.h"


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
        fprintf(output, "%u,%s\n", query_cover,content);
        strcpy(content, state.query);
        state.access = 0;
    }
    if(state.access==3){
        t_from=atoi(content);
        fprintf(output, "%s,", content);
        strcpy(content, state.query);
        state.access = 0;
    }
    if(state.access==4){
        t_to=atoi(content);
        fprintf(output, "%s,", content);
        strcpy(content, state.query);
        state.access = 0;
    }
    if (state.access == 5) // pour le nom de l'hit
    {
        name_hit= content;
        fprintf(stdout, "name hit is %s\n", name_hit);
        fprintf(output, "%s,", name_hit);
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
            fprintf(stdout, "name hit is %s\n", name_hit);
            fprintf(output, "-,%s,", content);
            strcpy(content, state.query);
            state.access = 0;
        }
    }
    if(state.access ==7){
        query_length=atoi(content);
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
