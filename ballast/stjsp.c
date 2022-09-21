#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int size;
    char str1[] = "adazc";
    char str2[] = "kfc";

    size = strcspn(str1, str2);
  
    printf("The unmatched characters before first matched character :  %d\n", size);

}