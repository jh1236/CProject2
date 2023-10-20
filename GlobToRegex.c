#include <stdlib.h>
#include <string.h>
#include "GlobToRegex.h"

char *glob2regex(char *glob)
{
    char *re = NULL;

    if(glob != NULL) {
        re	= calloc(strlen(glob)*2 + 4, sizeof(char));
        if(re == NULL) {
            return NULL;
        }

        char *r	= re;

        *r++	= '^';
        while(*glob != '\0')
            switch (*glob) {
                case '.' :
                case '\\':
                case '$' : *r++ = '\\'; *r++ = *glob++;	break;
                case '*' : *r++ = '.';  *r++ = *glob++;	break;
                case '?' : *r++ = '.'; glob++;		break;
                case '/' : free(re);
                    re	= NULL;
                    break;
                    //This code is given, but it doesn't seem to work?
                    //We think this loop needs to check that re is not null, because it gets stuck infinitely looping!!
                    //We don't want to mess around with code that isn't ours
                default  : *r++ = *glob++;
                    break;
            }
        if(re) {
            *r++	= '$';
            *r		= '\0';
        }
    }
    return re;
}