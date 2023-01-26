#include "blastInfo.h"

/// @brief get the version of blast
/// @param node 
void getBlastVersion(xmlNode *node){
    const char *name = "BlastOutput_version";
    if (strcmp(name, (const char *)node->name) == 0)
    {
        fprintf(output, "{\t\"blast-version\" : \"%s\",\n", xmlNodeGetContent(node));
    }
}

/// @brief get the db of this blast
/// @param node 
void getBlastDB(xmlNode *node){
    const char *name = "BlastOutput_db";
    if (strcmp(name, (const char *)node->name) == 0)
    {
        fprintf(output, "\t\"db\" : \"%s\",\n", xmlNodeGetContent(node));
    }
}

/// @brief get the Definition of query
/// @param node
void getQueryDef(xmlNode *node) //ajouter une autre variable pour récupérer le nom de la query
{
    const char *name = "BlastOutput_query-def";
    if (strcmp(name, (const char *)node->name) == 0)
    {
        fprintf(output, "\t\"blast_output\":[\n\t {\n\t\t\"query-name\" : \"%s\",\n", xmlNodeGetContent(node));
        //speciesName =(char *)xmlNodeGetContent(node);
    }
}

char *retrieveDef(xmlNode *node){
    char *query_def;
    const char *name = "BlastOutput_query-def";
    if (strcmp(name, (const char *)node->name) == 0)
    {
        query_def =(char *)xmlNodeGetContent(node);
    }
    return query_def;
}

/// @brief get the Length of the query
/// @param node
void getQueryLength(xmlNode *node, char *speciesName, int query_length)
{
    //printf("name is %s\n", speciesName);

    const char *name = "BlastOutput_query-len";
    if (strcmp(name, (const char *)node->name) == 0)
    {
        fprintf(output, "\t\t\"query-length\" : \"%s\",\n", xmlNodeGetContent(node));
        query_length = atoi((const char *)xmlNodeGetContent(node));
        //appeler ici les fonctions species et id
    }
}

void displayQuerySpecies(char *species, char *buffer){
    char *querySpeciesName = getQuerySpeciesName(species);
    fprintf(output, "\t\t\"species\": {\n");
    fprintf(output,"\t\t\t\"taxid\" : \"%s\",\n\t\t\t\"name\" : \"%s\"\n\t\t},\n", getQuerySpeciesId(querySpeciesName, buffer), querySpeciesName);
}

/// @brief récupérer le nom de l'espèce
/// @param species 
/// @return 
char *getQuerySpeciesName(char *species){
    char *content=species;
    
    int debut = 0, fin = 0;
    for (int i = 0; i < strlen(content); i++)
    {
        if (content[i] == 'O' && content[i+1]=='S')
        {
            debut = i+2;            
        }
        if (content[i] == 'O' && content[i+1]=='X')
        {
            fin = i-1;
            break;
        }
    }
    int j = 0;
    char *name_species=(char *)malloc(fin-debut);
    for (int i = debut + 1; i < fin; i++)
    {
        name_species[j] = content[i];
        j++;
    }
    name_species[j] = '\0';

    return name_species;
}

//récupérer l'id de cette espèce
char *getQuerySpeciesId(char *species, char *buffer)
{
    char *taxid = "";
    char speciesN[70];
    for (int i = 0; i < strlen(species); i++)
    {
        if (species[i] == '(')
        {
            speciesN[i-1] = '\0';
            break;
        }
        else
        {
            speciesN[i] = species[i];
        }
    }
    printf("species is :%s\n", speciesN);
    char *line = strtok(strdup(buffer), "\n");
    char id_species[MIN_SIZE], name_species[MIN_SIZE], rank_species[MIN_SIZE], id_parent_species[MIN_SIZE];

    while (line != NULL)
    {
        sscanf(line, "%[^	]	%[^	]	%[^	]	%[^\n]", id_species, name_species, rank_species, id_parent_species);
        if(strcmp(speciesN,name_species) == 0)
        {
            taxid=id_species;
            break;
        }
        line = strtok(NULL, "\n");
    }
    return taxid;
}