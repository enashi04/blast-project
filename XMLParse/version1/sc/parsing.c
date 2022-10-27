#include <ctype.h>
#include "xmlparser.h"


int moreThanTwoArgs();
void modechoice(int mode, xmlDoc *fichier, FILE *fp, char *filename, char *name);

int main(int argc, char **argv){
    //Si on dépasse 2 arguments
    if(argc > 3){
        return moreThanTwoArgs();
    }
    else if(argc == 3){
        
        char *next;
        long value = strtol(argv[1], &next,10);
        if(next == argv[1] || (*next != '\0')){
            printf("it's a number\n");
            //accéder au dossier contenant les fichiers xml pour parser le tout.

        }
        else{
            char *compare;
            compare = strstr(argv[2], ".xml");
            if(compare == NULL){
                printf("it's not a XMLfile\n");
                return 1;
            }
            //accès au fichier et on le parse solo
        }
        return 0;
    }
    else if(argc ==1){
        FILE *fp = NULL;
        printf("Pas de fichier en argument ...On analysera stdin.xml\n");
        xmlDoc *f = xmlReadFile("../XMLFiles/stdin.xml",NULL, 0);
        if(f==NULL){
            printf("Le fichier est vide\n");
            return 1;
        }

        modechoice(1,f,fp,"../results/BronzeResults_stdin.csv","stdin");
        modechoice(2,f,fp,"../results/SilverResults_stdin.csv","stdin");
        modechoice(3,f,fp,"../results/GoldResults_stdin.csv","stdin");
        
        return 0;
    }
}

int moreThanTwoArgs(){
    printf("You need to put 0 or 2 arguments\n");
    printf("0 argument: The program will parse stdin.xml on three differents modes\n");
    printf("2 arguments\targ1 : The program will parse either the number of XMLfiles or the XMLfile\n");
    printf("\t\targ2 : Mode choice: 1 (Bronze) | 2 (Silver) | 3 (Gold) | 4 (Personal)\n");
    return 1;
}

void noneArgument(){
    //ici on va analyser un seul fichier qui se trouve dans 
}

void modechoice(int mode, xmlDoc *fichier, FILE *fp, char *filename, char *name){
    
     switch(mode){
/***************************************************************************************************/
/******************************************BRONZE***************************************************/
/***************************************************************************************************/   
        case 1 :
            fp=fopen(filename, "w");
            fprintf(fp,"Bronze mode for %s\n\n",name);
            blastOutPut_iteration(fichier, fp, 1);
            printf("You chose the Bronze mode\nYou can open the file : BronzeResult.csv\n");   
            break;
/***************************************************************************************************/
/******************************************ARGENT***************************************************/
/***************************************************************************************************/  
        case 2: 
            fp=fopen(filename, "w");
            fprintf(fp,"Silver mode for %s\n\n",name);

            blastOutPut_iteration(fichier, fp, 2);
            printf("You chose the Silver mode\nYou can open the file : SilverResult.csv\n");
            break;
/***************************************************************************************************/
/******************************************GOLD*****************************************************/
/***************************************************************************************************/  
        case 3 :
            fp=fopen(filename, "w"); 
            fprintf(fp,"Gold mode for %s\n\n", name);
            blastOutPut_iteration(fichier, fp, 3);
            printf("You chose the Gold mode\nYou can open the file : GoldResult.csv\n");

            break;
/***************************************************************************************************/
/***********************************CHOIX DE L'UTILISATEUR******************************************/
/***************************************************************************************************/  
       default: break;
     
    }
}