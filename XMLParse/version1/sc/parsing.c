#include <ctype.h>
#include "xmlparser.h"
#include "argstype.h"

int main(int argc, char **argv){
    //Si on dépasse 2 arguments
    if(argc > 3){
        return moreThanTwoArgs();
    }
    else if(argc == 3){
        return twoArgs(argv);
    }
    else if(argc ==1){
        return noArgument();
    }
}
