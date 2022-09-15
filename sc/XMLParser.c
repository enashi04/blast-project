#include <stdio.h>
#include <stdlib.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
#include <string.h>

void query_Def(xmlNode *node);
void query_Length(xmlNode *node);

int main(int argc,char **argv){

    xmlDoc *fichier;
    xmlNode *root, *child, *node;

    char *filename;

    //condition pour savoir si il y'a un argument
    if(argc!=2){
        printf("Oupsi... you need to add a XML file!\n");
        return 1;
    }
   
    printf("File : %s\n\n",argv[1]);

    filename= argv[1];
    
    char *compare;
    compare = strstr(filename,".xml");

    //vérifier si il y'a bien un fichier xml
    if(compare==NULL){
        printf("It's not a XML file!\n");
        return 1;
    }
    
    //ouverture du fichier en mode lecture donc pas de modification possible dessus.
    fichier=xmlReadFile(filename,NULL, 0);

    if(fichier==NULL){
        printf("The file is empty\n");
        return 1;
    }

    root = xmlDocGetRootElement(fichier);//là on est au premier noeud //donc en pure logique on doit etre au noeud  BlastOutput 

    //afficher le root
   // fprintf(stdout, "Root is <%s> (%i)\n", root->name, root->type);

    //boucle qui va parcourir le fichier 
    child = root->children;
    
    //for query
    for(node=child; node; node=node->next){
         //fprintf(stdout, "\t Child is <%s> (%i)\n", node->name, node->type);
        query_Def(node);
        query_Length(node);
        break;
    }

    //entrer dans blastoutIteration
    const char *nodeName, *iteration; 
    nodeName = "BlastOutput_iterations";

    for(node=child; node; node=node->next){
        if(strcmp(nodeName,(const char *)node->name)==0){
            xmlNode *childnode;
            childnode = node->children;
            for(child=childnode; child; child=child->next){
                //il faut rentrer dans iteration et ensuite on parcourt avec les autres fonctions.
                iteration = "Iteration";
                if(strcmp(iteration,(const char *)child->name)==0){
                    fprintf(stdout, "child is <%s> \n", child->name);
                }
            }           
        }
    }



    //renvoie du résultat dans un autre fichier sous forme de tableau ou de liste (à voir).
    return 0;
}

//query def 
void query_Def(xmlNode *node){

    const char *name;
    name = "BlastOutput_query-def";
    
    if(strcmp(name, (const char *)node->name)==0){
        //affichage        
        fprintf(stdout, "Query def : %s\n", xmlNodeGetContent(node));
    }
}

//query length
void query_Length(xmlNode *node){
    const char *name;
    name = "BlastOutput_query-len";
    
    if(strcmp(name, (const char *)node->name)==0){
        //affichage        
        fprintf(stdout, "Length : %s\n", xmlNodeGetContent(node));
    }
} 

//récupérer le hit

//identifier le score

//identifier la evalue

//identifier les gap

//identifier le bitscore

//identifier la valeur positive

//identifier le début de la query 

//identifier la fin de la query

//identifier le début de la target 

//identifier la fin de la target

//identifier le numéro du hit 

//identifier l'id de la target