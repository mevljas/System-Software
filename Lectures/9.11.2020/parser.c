#include <stdio.h>


void parser( char *p, char **args)
{
    // kazalc p bomo povecevali
    while ( *p != '\0') // Lahko bi dali tudi kar 0
    {
        while ( *p == ' ' || *p == '\t') // Prazen znak
            *p++ = '\0';

        if (*p != 0)
            *args++ = p;

        while ( *p != ' ' && *p != '\t' && *p != '\0') //  Poln znak
            p++;

        // printf("%s\n", *args);
        
        
    }
    *args = (char *)0;
       
}

void main()
{
    char niz[] = " abc de \tgh ";
    char *args[32];

    parser(niz, args);

    for (int i = 0; args[i] != (char *)0; i++)
    {
        printf("%d: %s\n", i, args[i]);
    }
    
}