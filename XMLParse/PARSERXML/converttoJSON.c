// Conversion en JSON
#include "converttoJSON.h"

char name_hit[128];
char name_species[30];


/// @brief Retrieve the name of species from hit_def
/// @param content 
/// @return name_species
char *getSpecies(char *content)
{
    
    printf("content is %s\n", content);
    int debut=0, fin=0;
    for(int i =0; i<strlen(content); i++){
        if(content[i]=='['){ 
            debut = i;
            
        }
        if(content[i]==']'){
            fin =i;
            break;
        }
    }
    int j =0;
    for(int i = debut+1; i< fin ; i++){
        name_species[j]=content[i];
        j++;
    }
    name_species[j]='\0';
    printf("species is : %s\n", name_species);
    return name_species;
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
    case 1: //query name
        fprintf(output, "{\n\t\"query-name\" : \"%s\",\n", content);
        strcpy(content, state.query);
        state.access = 0;
        break;
    case 2: //query length
        fprintf(output, "\t\"query-len\" : \"%u\",\n\t\"hits\":\n\t[\n", atoi(content));
        query_length = atoi(content);
        strcpy(content, state.query);
        state.access = 0;
        break;
    case 3: //hit-accession
        strcpy(name_hit, content); //stock hit accession in name_hit
        fprintf(output, "\t\t\t\"hit_accession\" : \"%s\",\n", content);
        strcpy(content, state.query);
        state.access = 0;
        break;
    case 4: 
        if (atoi(content) == 1) //if it's the first hit
        {
            fprintf(output, "\t\t\t\"nb_hit\" : \"%u\",\n", atoi(content));
            strcpy(content, state.query);
            state.access = 0;
            break;
        }
        else
        { //otherwise, we add the hit_accession before nb of hit
            fprintf(output, "\n{\n\t\t\t\"hit_accession\" : \"%s\",\n", name_hit);
            fprintf(output, "\t\t\t\"nb_hit\" : \"%u\",\n", atoi(content));
            strcpy(content, state.query);
            state.access = 0;
            break;
        }
    case 5: //bitscore
        fprintf(output, "\t\t\t\"bitscore\" : \"%u\",\n", atoi(content));
        strcpy(content, state.query);
        state.access = 0;
        break;
    case 6: //score
        fprintf(output, "\t\t\t\"score\" : \"%u\",\n", atoi(content));
        strcpy(content, state.query);
        state.access = 0;
        break;
    case 7: //evalue
        fprintf(output, "\t\t\t\"evalue\" : \"%u\",\n", atoi(content));
        strcpy(content, state.query);
        state.access = 0;
        break;
    case 8: //query from
        fprintf(output, "\t\t\t\"query-from\" : \"%u\",\n", atoi(content));
        strcpy(content, state.query);
        state.access = 0;
        break;
    case 9://query to
        fprintf(output, "\t\t\t\"query-to\" : \"%u\",\n", atoi(content));
        strcpy(content, state.query);
        state.access = 0;
        break;
    case 10://target from
        fprintf(output, "\t\t\t\"target-from\" : \"%u\",\n", atoi(content));
        t_from = atoi(content);
        strcpy(content, state.query);
        state.access = 0;
        break;
    case 11://target to
        fprintf(output, "\t\t\t\"target-to\" : \"%u\",\n", atoi(content));
        t_to = atoi(content);
        strcpy(content, state.query);
        state.access = 0;
        break;
    case 12: //identities
        fprintf(output, "\t\t\t\"identities\" : \"%u\",\n", atoi(content));
        strcpy(content, state.query);
        state.access = 0;
        break;
    case 13: //positives
        fprintf(output, "\t\t\t\"positives\" : \"%u\",\n", atoi(content));
        strcpy(content, state.query);
        int query_cover = 100 * (t_to - t_from) / query_length;
        fprintf(output, "\t\t\t\"query-cover\" : \"%u\",\n", query_cover);
        state.access = 0;
        break;
    case 14://gaps
        fprintf(output, "\t\t\t\"gaps\" : \"%u\",\n", atoi(content));
        strcpy(content, state.query);
        state.access = 0;
        break;
    case 15: //alignment length
        fprintf(output, "\t\t\t\"align-len\" : \"%u\"\n\t\t}", atoi(content));
        strcpy(content, state.query);
        state.access = 0;
        break;
   case 16: //hit_def we want to get the lineage but first we try to retrieve the species name
   //the difference between them is about writing the JSON
        if (strcmp(name_species, "") != 0) //if the name isn't empty
        {
            //printf("le content est %s\n", content);
            fprintf(output, ",\n\t\t{\n\t\t\t\"lineage\" : \"%s\",\n",  getSpecies(content));
            strcpy(content, state.query);
            state.access = 0;
        }
        else if(strcmp(name_species, "") ==0) 
        {
            fprintf(output, "\t\t{\n\t\t\t\"lineage\" : \"%s\",\n",  getSpecies(content));
            strcpy(content, state.query);
            state.access = 0;
        }
        break;
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
