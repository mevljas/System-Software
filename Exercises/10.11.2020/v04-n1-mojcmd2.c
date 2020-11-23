#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#define BUFFSIZE 1024
#define err_sys(mess)                          \
    {                                          \
        perror(mess);                          \
        exit(1);                               \
    }

void pr_exit(int);
static void sig_alrm(int);

int main()
{
    int n, status;
    char buf[BUFFSIZE];
    pid_t pid;
    char *token;
    int nsecs = 3;
    char *argv[64];
    if(signal(SIGINT, sig_alrm) == SIG_ERR){
        err_sys("SIG INT");
    }
    if (signal(SIGALRM, SIG_IGN) == SIG_ERR)
        err_sys("SIGALRM");
    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
    {
        printf("-------------------------\n");
        if ((pid = fork()) < 0)
        {
            err_sys("fork");
        }
        else if (pid == 0)
        {
            int x = 0;
            token = strtok(buf, "\n");
            token = strtok(token, " ");
            while (token != NULL)
            {
                argv[x] = token;
                x++;
                token = strtok(NULL, " ");
            }
            argv[x] = NULL;
            if (execvp(argv[0], argv) < 0)
            {
                err_sys("Exec error");
            };
        }
        if(wait(&status) < 0){
            err_sys("Wait");
        }
        printf("-------------------------\n");
        pr_exit(status);
        printf("-------------------------\n");
    }
    if (n < 0)
    {
        err_sys("Read error");
    }
    return 0;
}

void pr_exit(int status)
{
    if (WIFEXITED(status))
        printf("normal termination, exit status = %d\n",
               WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        printf("abnormal termination, signal number = %d%s\n",
               WTERMSIG(status),
               WCOREDUMP(status) ? " (core file generated)" : "");
    else if (WIFSTOPPED(status))
        printf("child stopped, signal number = %d\n",
               WSTOPSIG(status));
}

static void sig_alrm(int nsec){
    int x = alarm(3);
    if(x > 0){
        printf("Nasvidenje!\n");
        exit(0);
    }
    else{
        printf("Ce v roku od 3 sekundah pritisnes Control C se bo ustavil\n");
    }
}