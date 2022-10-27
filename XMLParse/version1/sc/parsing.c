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
        return noneArgument();
    }
}
