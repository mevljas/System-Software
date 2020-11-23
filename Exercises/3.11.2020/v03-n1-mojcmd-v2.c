#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1024
#define MAXWORDS 64

int executeParallel;

void pr_exit(int status);


int main(void)
{
    char buf[MAXLINE];
    char *args[MAXWORDS];
    pid_t pid;
    int status;

    printf("mojPrompt> "); // izpiše prompt string

    while (fgets(buf, MAXLINE, stdin) != NULL)
    {      

        buf[strlen(buf) - 1] = '\0'; // spremeni znak za novo vrstico v ničti znak
            
        if(buf[strlen(buf) - 1] == '&') // ce je v ukazu znak za paralelno izvedbo
        {
            buf[strlen(buf) - 2] = '\0'; // znak za paralelno izvedbo ni argument ukaza ampak prompta (z -2 odstranimo se presledek)
            executeParallel = 1;
        }
        else
        {
            executeParallel = 0;
        }

        //FORK
        if ( (pid = fork()) < 0){ //error
            perror("fork error");
        }
        else if (pid == 0) { // otrok
            
            
            //printf("dolzina ukaza: %d\n", (int)strlen(buf));
            
            //cheat code:
            //system(buf);
            //_exit(0);

            //obdelamo string v bufferju buf da bo v primerni obliki
            args[0] = buf;
            int argnum = 1; //stevilo argumentov
            
            int i = 0;
            while (buf[i] != '\0') {       /* Stop looping when we reach the null-character. */
                if(buf[i] == ' ')
                {
                    buf[i] = '\0';
                    args[argnum] = buf+i+1;
                    argnum++;
                }
                i++;
            }
            args[argnum] = NULL;

            printf("Ukaz, ki ga izvajamo:\n");
            for(int i=0; args[i] != NULL; i++)
            {
                printf("%d: %s\n", i, args[i]);
            }
            printf("--------------------------------------------------\n");

            //pozenemo ukaz
            execvp(args[0], (char *const*)args);    
            
            printf("couldn't execute: %s\n", buf);
            perror(buf);
            exit(127);
        }
        //koda starša
        if(executeParallel)
        {
            //zacasna resitev, dokler 'ne znamo' obdelovati signalov, naj bi preprecila, da se koda starsa izvede pred kodo otroka
            sleep(1);
            //printf("paralelno cakanje\n");
            if ( (pid = waitpid(pid, &status, WNOHANG)) < 0) 
            {
                perror("waitpid error");
                exit(1);
            }   
        }
        else
        {
            //printf("normalno cakanje\n");
            if ( (pid = waitpid(pid, &status, 0)) < 0) 
            {
                perror("waitpid error");
                exit(1);
            }
        }

        printf("--------------------------------------------------\n");

        pr_exit(status); 
        
        printf("\nmojPrompt> "); // izpiše prompt string
    }

    return 0;
} 


void pr_exit(int status)
{
    if (WIFEXITED(status))
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        printf("abnormal termination, signal number = %d%s\n", WTERMSIG(status), WCOREDUMP(status) ? " (core file generated)" : "");
    else if (WIFSTOPPED(status))
        printf("child stopped, signal number = %d\n", WSTOPSIG(status));
}