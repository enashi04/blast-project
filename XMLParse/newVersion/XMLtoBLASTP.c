#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/libxml/parser.h"
#include "lib/libxml/tree.h"

void blast_version(xmlNode *node, FILE *output) {
    const char *name;
    name = "BlastOutput_version";
    
    if(strcmp(name, (const char *)node->name)==0){
        //affichage  
        fprintf(output, "%s\n\n", xmlNodeGetContent(node));
    }
}

void blast_reference(xmlNode *node, FILE *output) {
    const char *name;
    name = "BlastOutput_reference";
    
    if(strcmp(name, (const char *)node->name)==0){
        char content[12] ="Reference: ";
        strcat(content, (const char*)xmlNodeGetContent(node));
        strcat(content, "\0");
        int len = strlen(content);

        for(int i = 0; i < len+1; i++) {
            printf("%c",content[i]);
            if ((i + 1) % 66 == 0) {
                printf("\n");
            }
        }
    }
}
/**
 * RÉCUPÉRER LE NOM DE LA QUERY , début du formatage en json
*/
void query_Def(xmlNode *node, FILE *output){
    const char *name;
    name = "BlastOutput_query-def";
    
    if(strcmp(name, (const char *)node->name)==0){
        //affichage  
        fprintf(output, "Query :%s\n", xmlNodeGetContent(node));
        printf("ici");

    }
}

/**
 * RÉCUPÉRER LA LONGUEUR DE LA QUERY
*/
void query_Length(xmlNode *node, FILE *output){
    const char *name;
    name = "BlastOutput_query-len";
    
    if(strcmp(name, (const char *)node->name)==0){
        //affichage        
        fprintf(output, "Length : %s\n\n", xmlNodeGetContent(node));
        //ici on met la query length pour le calcul de la query cover
    }
} 

//récupération du blastoutput version 
void blastInfo(xmlDoc *xmlfile, FILE *output,xmlNode *child){
    xmlNode *node;
    fprintf(stdout,"le noeud ou je suis est %s\n", child->name);
        
     for(node=child; node; node=node->next){
        fprintf(stderr,"les noeuds sont : %s\n", node->name);
        //récupérer la version, la référence, et la base de données
        blast_version(node, output);
        blast_reference(node, output);
    //     query_Def(node, output);
    //     query_Length(node, output);
    }
}

int main(int argc, char **argv){
    //ouvrir le fichier en mode lecture
    xmlDoc *xmlfile = xmlReadFile("stdin.xml", NULL,0);
    FILE *output = fopen("output.txt","w");
    fprintf(output,"fpfje");
    xmlNode*child, *root ;
    root = xmlDocGetRootElement(xmlfile);
    child = root->children;
    //mettre la boucle ici
    blastInfo(xmlfile, output, child);

   
    //Récupérer le blastouput version 

    //récupérer le blastoutput reference

    //récupérer le blastouput db

    //récupérer la db complete

    //récupérer la query et la longueur de la db
    /* 
    <Iteration_iter-num>1</Iteration_iter-num>
    <Iteration_query-ID>Query_808952</Iteration_query-ID>
    <Iteration_query-def>sp|P96368|TRCS_MYCTU Sensor histidine kinase TrcS OS=Mycobacterium tuberculosis (strain ATCC 25618 / H37Rv) OX=83332 GN=trcS PE=1 SV=1</Iteration_query-def>
    <Iteration_query-len>509</Iteration_query-len>
  */

    //séquence jsp quoi on laisse tomber je crois (à vérif)

    /*****************************************************************************/
    //verif du nombre de hit si y'en a deux alors on prend en compte le score en *2 et le reste des params en *2.
    //on rentre dans les hit
    //récupération de hit_def
    //récup de la longueur
    //récup du score, expect, Method
    //identitie, positives (en % et en normal)

    //Viens la query midline subject où il faut qu'il y est 64 caracteres 

    //à la fin on prends les blastoup_param
    //-> param : matrix / expect/gap_pen/ gap_extends/filter / on s'arretera là peut etre

}