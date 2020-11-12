#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>


#define MAXLINE 1024
#define ARGV_N 64


void pr_exit(int status);

void signal_handler( int sig_number) 
{
    
    switch (sig_number)
    {
    case SIGINT:
        printf("\nza izhod iz programa se enkrat pritisnite CTRL+C v roku 3 sekund.\n");
        int time_left = alarm(3);
        if (time_left > 0)
        {
            printf("Nasvidenje!\n");
            exit(0);
        }
        printf(">>> ");
        fflush(stdout);     // flush buffer - force print
        break;
    

    case SIGALRM:
        break;
    
    default:
        break;
    }
    
    
    
    
}

void main()
{
    char buf[MAXLINE];
    char *args[ARGV_N];  
    pid_t pid;
    int status, i;


    // Set signal handler
    signal( SIGINT, signal_handler);
    signal( SIGALRM, signal_handler);


    // Print prompt
    printf(">>> ");

    // Reads while read != NULL
    while( fgets(buf, MAXLINE, stdin)){ 
  
        // Change newline smybol to null
        buf[strlen(buf) - 1] = 0;

        // Check if user entered anything
        if (strlen(buf) > 0)
        {
            i = 0;
            
            // Split and save the first word
            args[i] = strtok(buf," ");

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

                

                

                // While there are other words, split and save
                while(args[i]!=NULL)
                {
                    args[++i] = strtok(NULL," ");
                }

                // Save null at the end of the string
                args[i + 1] = 0;

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


                printf(">>> ");
                
            }
        }
        else
        {
            // Print prompt
            printf(">>> ");
        }
        
        


        

    }

    exit(0);

    

}






    



    

