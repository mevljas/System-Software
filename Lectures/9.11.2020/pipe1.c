#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define err_sys(mess) { fprintf(stderr,"Error: %s.", mess); exit(1); }
#define MAXLINE 512

int main(void)
{
    int n, fd[2];
    pid_t pid;
    char line[MAXLINE];
    if (pipe (fd) < 0)
        err_sys("pipe error");
    
    
    if ( (pid = fork ()) < 0 )
        err_sys("fork error")
    else if (pid > 0) { //starš
        close(fd[0]);   // Zapre bralni konec
        write(fd[1], "hello world \n", 12);
    }
    else {      // otrok
        close(fd[1]);
        n = read (fd[0], line, MAXLINE);
        write(STDOUT_FILENO, line, n);
    }
    exit(0);
}