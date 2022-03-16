
/** \file common.c
 * \brief Miscellaneous functions
 * \author Thibault DOUCET
 */

/* =============== DEPENDANCES =============== */

#include <stdio.h>
#include <ctype.h>
#include <string.h>

/* =============== FONCTIONS =============== */

char * strToUpper(char *chaine)
{
    char * temp = chaine;

    for (int i = 0 ; temp[i] != '\0' ; i++)
    {
        temp[i] = toupper(temp[i]);
    }

    return temp;
}