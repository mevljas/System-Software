#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFSIZE 256

void handler( int sig_num) 
{
    printf("Dobil sem SIGINT - hvala!\n");
}
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

int main()
{
    pid_t pid;
    char buf[BUFSIZE];
    char *args[50]; 

    // Signal handler
    // Lovili bomo ctrl+c
    if (signal(SIGINT, handler) == SIG_ERR){
        perror("handler");
    }

    printf("$ "); //prompt

    while (fgets(buf, BUFSIZE, stdin) != NULL)
    {
        // zadnji znak v bufferjo bu newline. To odstranimo.
        buf[strlen(buf) - 1] = '\0';
        // strlen ni dolzina  bufferja, ampak stringa.

        parser( buf, args);

        // Izpis argumentov.
        for (int i = 0; args[i]; i++)
        {
            puts(args[i]);
        }
        

        if ( (pid = fork()) < 0)
            perror("fork");
        else if (pid == 0) // otrok
        { 
            // execv ni delal, execvp pa ja
            execvp( args[0], args);
            printf("Ne morem izvesti ukaza.\n");
            exit(127);
        } 
        

        // Stars
        // Wait pocaka, da en bo zombie.
        if (wait(NULL) < 0)
        {
            perror("wait");
        }

        printf("$ "); //prompt
        
        
    }

    return 0;
    

    
    
}