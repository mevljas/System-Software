#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>


#define MAXLINE 1024
#define ARGV_N 64


void pr_exit(int status);

void main()
{
    char buf[MAXLINE];
    char *args[ARGV_N];  
    pid_t pid;
    int status, i;


    // Print promp
    printf("%%> ");

    // Reads while read != NULL
    while( fgets(buf, MAXLINE, stdin)){
        printf("-------------------------------\n");
        // fputs( buf, stdout);        
        

        // Change newline smybol to null
        buf[strlen(buf) - 1] = 0;

        // Create a child
        pid = vfork();
        if ( pid < 0)
        {
            // Error
            perror("fork");
            exit(1);
        }
        else if ( pid == 0)
        {
            // child
            printf("Izpis argumentov:\n");
            
            // Split and save the first word
            args[i] = strtok(buf," ");

            i = 0;

            // While there are other words, split and save
            while(args[i]!=NULL)
            {
                args[++i] = strtok(NULL," ");
            }

            // Save null at the end of the string
            args[i + 1] = 0;

            i = 0;
            // Output arguments
            while(args[i]!=NULL)
            {
                printf("args[%d]: ",i);
                printf("%s\n", args[i++]);
            }

            printf("-------------------------------\n");
            printf("\n");

            // Run command
            execvp( args[0], args);

            // If this part of the code gets accessed, there must be an error.
            perror("exec");
            exit(1);
        }
        else
        {
            // parent

            


            if ( (pid = waitpid(pid, &status, 0)) < 0)
            {
                perror("waitpid");
            }

            printf("\n-------------------------------\n");

            printf("Otrok je vrnil status %d (0x%X)\n", status, status);
            pr_exit(status);

            printf("-------------------------------\n");


            printf("%%> ");
            
        }

    }

    exit(0);

    

}


void pr_exit(int status)
{
    if (WIFEXITED(status))
        printf("normal termination, exit status = %d\n", 
        WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        printf("abnormal termination, signal number = %d%s\n", 
        WTERMSIG(status), WCOREDUMP(status) ? " (core file generated)" : "");
    else if (WIFSTOPPED(status))
        printf("child stopped, signal number = %d\n", 
        WSTOPSIG(status));
}



    



    

