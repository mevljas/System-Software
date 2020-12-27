#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFSIZE 256


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
    pid_t pid;
    char buf[BUFSIZE];
    char *args[50];

    printf("$ "); // prompt

    while ( fgets(buf, BUFSIZE, stdin) != NULL)
    {
        buf[strlen(buf)-1] = '\0';
        parser( buf, args);

        for (int i = 0; args[i]; i++)
        {
            puts(args[i]);
        }
        

        if ( (pid = fork()) < 0)
        {
            perror("fork");
        }
        else if (pid == 0){ //otrok
            execvp( args[0], args);
            printf("ne morem izvesti ukaza.\n");
            exit(127);
        }
        
    }
    
    
}