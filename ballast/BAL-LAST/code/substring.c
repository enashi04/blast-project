#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *substr(const char *src, int m, int n)
{
    int len = n - m;
    char *dest = (char *)malloc(sizeof(char) * (len + 1));
    strncpy(dest, (src + m), len);
    return dest;
}