//
// Created by healy on 20/10/2023.
//

#include <stdio.h>
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