#include <stdio.h>
#include <stdlib.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
#include <string.h>

void query_Def(xmlNode *node, FILE *fp);
void query_Length(xmlNode *node, FILE *fp);
void hit_Iteration(xmlNode *node, FILE *fp);
void hit_ID(xmlNode *node, FILE *fp);
void HSP_Enter(xmlNode *node, FILE *fp);
void getHSP(xmlNode *node, const char *name, FILE *fp);

int main(int argc,char **argv){
    FILE *fp;
    fp=fopen("result.csv", "w");
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

    root = xmlDocGetRootElement(fichier);

    //boucle qui va parcourir le fichier 
    child = root->children;
    
    //for query
    for(node=child; node; node=node->next){
        query_Def(node, fp);
        query_Length(node,fp);
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
                    hit_Iteration(child,fp);
                }
            }           
        }
    }
    return 0;
}

//query def 
void query_Def(xmlNode *node, FILE *fp){

    const char *name;
    name = "BlastOutput_query-def";
    
    if(strcmp(name, (const char *)node->name)==0){
        //affichage        
        fprintf(fp, "Query def : %s\n", xmlNodeGetContent(node));
    }
}

//query length
void query_Length(xmlNode *node, FILE *fp){
    const char *name;
    name = "BlastOutput_query-len";
    
    if(strcmp(name, (const char *)node->name)==0){
        //affichage        
        fprintf(fp, "Length : %s\n", xmlNodeGetContent(node));
    }
} 

//entrer dans le noeud hit
void hit_enter(xmlNode *node, FILE *fp){
    const char *name;
    name = "BlastOutput_query-len";
    
    if(strcmp(name, (const char *)node->name)==0){
        //affichage        
        fprintf(fp, "Length : %s\n", xmlNodeGetContent(node));
    }
}

//récupérer le hit
void hit_Iteration(xmlNode *node, FILE *fp){
    xmlNode *child;
    const char *iteration, *hit; 
    child = node->children;
    iteration = "Iteration_hits";
    hit= "Hit";
    int i=0; 
    for(node = child; node; node =node->next){
        if(strcmp(iteration, (const char *)node->name)==0){
            xmlNode *childNode;
            childNode = node->children; 
            for(child=childNode; child; child=child->next){
                if(strcmp(hit, (const char *)child->name)==0){
                   i++;
                    //on met les fonctions qui va permettre de récupérer les données qu'on veut
                    fprintf(fp, "%u : ", i);
                    hit_ID(child,fp);
                    HSP_Enter(child, fp);
                    //break;
                }
            }
        }
    }
}

//identifier le hit id (hit id or hit def???)
void hit_ID(xmlNode *node, FILE *fp){
    xmlNode *child;
    const char *name; 
    child = node ->children;
    name= "Hit_id";
    for(node = child; node; node=node->next){
        if(strcmp(name, ( const char *)node->name)==0){
            fprintf(fp, "Hit id : %s\n", xmlNodeGetContent(node));
            fprintf(fp, "bit-score,score,evalue,query-from,query-to,target-from,target-to,identity,positive,gaps,align-length \n");
        }
    }
}
//rentrer dans hsp
void HSP_Enter(xmlNode *node, FILE *fp){
    xmlNode *child;
    const char *name, *name2; 
    child = node ->children;
    name= "Hit_hsps";
    name2 = "Hsp";
    //printf("test \n");

    for(node = child; node; node=node->next){
        if(strcmp(name, ( const char *)node->name)==0){ 
            xmlNode *childNode;
            childNode = node->children;
           
            for(child = childNode; child; child=child->next){

                if(strcmp(name2, (const char *)child->name)==0){
                    //dernière boucle et on mets les fonctions à l'intérieur pour récupérer les éléments qu'on veut !
                    xmlNode *lastchild; 
                    lastchild=child->children;

                    for(childNode=lastchild; childNode; childNode=childNode->next){

                        //les méthodes pour chercher les infos qu'on veut 
                        getHSP(childNode, "Hsp_identity", fp);
                        getHSP(childNode, "Hsp_align-len", fp);
                        getHSP(childNode, "Hsp_gaps", fp);
                        getHSP(childNode, "Hsp_query-from",fp);
                        getHSP(childNode, "Hsp_query-to",fp);
                        getHSP(childNode, "Hsp_hit-from",fp);
                        getHSP(childNode, "Hsp_hit-to",fp);
                        getHSP(childNode, "Hsp_positive",fp);
                        getHSP(childNode, "Hsp_evalue",fp);
                        getHSP(childNode, "Hsp_score",fp);
                        getHSP(childNode, "Hsp_bit-score",fp);
                   
                    }
                }
            }
        }
    }
    fprintf(fp, "\n \n");

}
//identifier l'identité
void getHSP(xmlNode *node, const char *name, FILE *fp){
    if(strcmp(name, (const char *)node->name)==0){
        fprintf(fp," %s,",xmlNodeGetContent(node));
    }
   
}