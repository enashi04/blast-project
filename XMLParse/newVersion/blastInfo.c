#include "blastInfo.h"

/// @brief get the version of blast
/// @param node
void getBlastVersion(xmlNode *node)
{
    const char *name = "BlastOutput_version";
    if (strcmp(name, (const char *)node->name) == 0)
    {
        fprintf(output, "{\t\"blast-version\" : \"%s\",\n", xmlNodeGetContent(node));
    }
}

/// @brief get the db of this blast
/// @param node
void getBlastDB(xmlNode *node)
{
    const char *name = "BlastOutput_db";
    if (strcmp(name, (const char *)node->name) == 0)
    {
        fprintf(output, "\t\"db\" : \"%s\",\n", xmlNodeGetContent(node));
    }
}

/// @brief get the Definition of query
/// @param node
void getQueryDef(xmlNode *node, char species[MIN_SIZE]){ // ajouter une autre variable pour récupérer le nom de la query
    const char *name = "BlastOutput_query-def";
    char *query_def;
    if(strcmp(name, (const char *)node->name) == 0)
    {
        query_def = (char *)xmlNodeGetContent(node);
        strcpy(species, query_def);
        fprintf(output, "\t\"blast_output\":[\n\t {\n\t\t\"query-name\" : \"%s\",\n", xmlNodeGetContent(node));
    }
}

/// @brief get the Length of the query
/// @param node
void getQueryLength(xmlNode *node, int query_length)
{
    const char *name = "BlastOutput_query-len";
    if (strcmp(name, (const char *)node->name) == 0)
    {
        fprintf(output, "\t\t\"query-length\" : \"%s\",\n", xmlNodeGetContent(node));
        query_length = atoi((const char *)xmlNodeGetContent(node));
    }
}

/// @brief récupérer le nom de l'espèce
/// @param species
/// @return species Name of the query
char *getQuerySpeciesName(char *species)
{
    char content[MIN_SIZE];
    strcpy(content, species);
    int len = strlen(content); //pb
    int start = 0, end = 0;
    for (int i = 0; i < len; i++)
    {
        if (content[i] == 'O' && content[i + 1] == 'S') //pb
        {
            start = i + 2;
        }
        if ((content[i] == 'O' && content[i + 1] == 'X') ) //pb
        {
            end = i - 1;
            break;
        }
    }
    char name_species[MIN_SIZE];

    int j = 0;
    for (int i = start + 1; i < end; i++)
    {
        name_species[j] = content[i];
        j++;
    }
    name_species[j] = '\0';
    return strdup(name_species);
}

/// @brief
/// @param species
/// @param buffer
void displayQuerySpecies(char *species, char *buffer)
{
    char *querySpeciesName = getQuerySpeciesName(species);
    char speciesName[MIN_SIZE];
    int queryNameLen= strlen(querySpeciesName);
    int j =0;
    for(int i =0; i<queryNameLen; i++){
        if(querySpeciesName[i]=='('){
            j=i-1;
            break;
        }
    }
    for(int i =0; i<j;i++){
        speciesName[i]=querySpeciesName[i];
    }
    speciesName[j]='\0';
    char *line = strtok(strdup(buffer), "\n");
    char id_species[MIN_SIZE], name_species[MIN_SIZE];

    while (line != NULL)
    {
        sscanf(line, "%[^	] %*[^	] %[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^\n]", id_species, name_species);
        if (strcmp(speciesName, name_species) == 0)
        {
            fprintf(output, "\t\t\"species\": {\n");
            fprintf(output, "\t\t\t\"taxid\" : \"%s\",\n\t\t\t\"name\" : \"%s\"\n\t\t},\n", id_species, querySpeciesName);

            break;
        }
        line = strtok(NULL, "\n");
    }
    free(querySpeciesName);
    
}

char *getRank(char *line)
{
    char rank_species[MIN_SIZE];
    int len = strlen(line);
    int iteration = 0;
    for (int i = 0; i < len + 1; i++)
    {
        if (line[i] == '	')
        {
            iteration++;
        }
        if (iteration == 8)
        {
            int k = 0;
            for (int j = i + 1; j < len; j++)
            {
                if (line[j] == '	')
                {
                    rank_species[k] = '\0';
                    break;
                }
                else
                {
                    rank_species[k] = line[j];
                }
                k++;
            }
            break;
        }
    }

    return strdup(rank_species);
}
/// @brief 
/// @param id_parent_species 
/// @param buffer 
/// @return 
char *getParentName(char *id_parent_species, char *buffer)
{
    char *line = strtok(strdup(buffer), "\n");
    char id_species[MIN_SIZE], name_species[MIN_SIZE];


    while (line != NULL)
    {
        sscanf(line, "%[^	] %*[^	] %[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^	] %*[^\n]", id_species, name_species);
        if (strcmp("77643", id_species) == 0)
        {
            return strdup(name_species);
        }
        line = strtok(NULL, "\n");
    }
    return "no parent";
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