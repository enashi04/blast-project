/**************************************************************************************************************/
/*                                         BLASTINFO.C:                                                       */
/** This file allows to get the information of BLAST                                                          */
/**************************************************************************************************************/

#include "blastInfo.h"

/**************************************************************************************************************/
/*                               getBlastVersion: version of blast                                            */
/** Parameters : node : Node where we are now                                                                 */
/**************************************************************************************************************/
void blastVersion(xmlNode *node)
{
    const char *name = "BlastOutput_version";
    if (strcmp(name, (const char *)node->name) == 0)
    {
        fprintf(output, "{\n\t\"blast-version\" : \"%s\",\n", xmlNodeGetContent(node));
    }
}

/**************************************************************************************************************/
/*                               getBlastDB: the db of this blast                                             */
/** Parameters : node : Node where we are now                                                                 */
/**************************************************************************************************************/
void blastDB(xmlNode *node)
{
    const char *name = "BlastOutput_db";
    if (strcmp(name, (const char *)node->name) == 0)
    {
        fprintf(output, "\t\"db\" : \"%s\",\n", xmlNodeGetContent(node));
    }
}

/**************************************************************************************************************/
/*                              getQueryInfo: getQueryInfo                                                    */
/** Parameters : node : Node where we are now                                                                 */
/**            : species : name of the query's species (complete name)                                        */
/**            : query_length : length of the query                                                           */
/**            : iteration_num : number of the blast_iteration                                                */
/**************************************************************************************************************/

void getQueryInfo(xmlNode *child, char species[MIN_SIZE], int query_length, char* iteration_num){ 
     for(xmlNode *childnode=child->children; childnode; childnode=childnode->next){
        if(strcmp("Iteration_query-def", (const char *)node->name) == 0)
        {
            query_def = (char *)xmlNodeGetContent(node);
            strcpy(species, query_def);
            fprintf(output, " \t{\n\t\t\"query-name\" : \"%s\",\n", xmlNodeGetContent(node));
        }
        else if(strcmp("Iteration_iter-num", (const char *)childnode->name) == 0){
            iteration_num=(char *)xmlNodeGetContent(childnode);
        }
        else if (strcmp("Iteration_query-len", (const char *)childnode->name) == 0)
        {
            fprintf(output, "\t\t\"query-length\" : \"%s\",\n", xmlNodeGetContent(childnode));
            query_length = atoi((const char *)xmlNodeGetContent(childnode));
        }
    }
}

/**************************************************************************************************************/
/*                         getQuerySpeciesName: name of the query's species                                   */
/** Parameters : species : name of the query's species                                                        */
/**************************************************************************************************************/
char *getQuerySpeciesName(char *query_Species)
{
    char content[MIN_SIZE];
    strcpy(content, query_Species);
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

/**************************************************************************************************************/
/*                         getQuerySpeciesName: id of the query's species                                     */
/** Parameters : species : query's species                                                                    */
/**************************************************************************************************************/
char *getQuerySpeciesID(char *query_Species){
    char content[MIN_SIZE];
    strcpy(content, query_Species);
    int len = strlen(content); //pb
    int start = 0, end = 0;
    for (int i = 0; i < len; i++)
    {

        if ((content[i] == 'O' && content[i + 1] == 'X') ) //pb
        {
            start = i + 2;
        }

         if ((content[i] == 'G' && content[i + 1] == 'N') ) //pb
        {
            end = i - 1;
            break;
        }
    }
    char id[MIN_SIZE];
    int j = 0;
    for (int i = start + 1; i < end; i++)
    {
        id[j] = content[i];
        j++;
    }
    id[j] = '\0';
    return strdup(id);
}

/// @brief
/// @param species
/// @param buffer
/**************************************************************************************************************/
/*                         displayQuerySpecies: display informations about the query                          */
/** Parameters : species : name of the query's species                                                        */
/**            : buffer : taxonomy.dat                                                                        */
/**************************************************************************************************************/
void displayQuerySpecies(char *query_species)
{
    char *querySpeciesName = getQuerySpeciesName(query_species);
    char *id_Species = getQuerySpeciesID(query_species);
    
    fprintf(output, "\t\t\"species\": {\n");
    fprintf(output, "\t\t\t\"taxid\" : \"%s\",\n\t\t\t\"name\" : \"%s\"\n\t\t},\n", id_Species, querySpeciesName);
    free(querySpeciesName);
    
}

char *getRank(char *line)
{
    char rank_species[MIN_SIZE];
    int len = strlen(line);
    int iteration = 0;
    for (int i = 0; i < len + 1; i++)
    {
        if (line[i] == '	') //change this part cause 2 bumps 
        {
            iteration++;
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
       
    }
    return strdup(rank_species);
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
    char *line = strtok(strdup(buffer), "\n");
    char id_species[MIN_SIZE], name_species[MIN_SIZE];

    while (line != NULL)
    {
        sscanf(line, "%[^	]	%*[^	]	%[^	]	%*[^\n]", id_species, name_species);
        if(strcmp(speciesN,name_species) == 0)
        {
            taxid=id_species;
            break;
        }
        line = strtok(NULL, "\n");
    }
    return taxid;
}
