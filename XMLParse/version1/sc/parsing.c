#include <ctype.h>
#include "xmlparser.h"
#include "argstype.h"

int main(int argc, char **argv){
    //Si on dépasse 2 arguments
    if(argc > 3){
        return moreThanTwoArgs();
    }
    else if(argc == 3){
        
        char *next;
        long value = strtol(argv[1], &next,10);
        if(next == argv[1] || (*next != '\0')){
            char *compare;
            compare = strstr(argv[1], ".xml");
            if(compare == NULL){
                printf("it's not a XML file\n");
                return 1;
            }
            //parser un seul fichier 
            xmlDoc *f = xmlReadFile(argv[1],NULL, 0);
            if(f==NULL){
                printf("File is empty\n");
                return 1;
            }   
        }
     //accéder au dossier contenant les fichiers xml pour parser le tout.
        else{
            printf("It's a number\n");
            //accès au fichier et on le parse solo
            
        }
        return 0;
    }
    else if(argc ==1){
        return noArgument();
    }
}
