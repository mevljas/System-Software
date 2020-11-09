#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define err_sys (mess) { fprintf(stderr,"Error: %s.", mess); exit(1);
#define MAXLINE 512

int main(void)
{
    int n, fd[2];
    pid_t
    pid;
    char
    line[MAXLINE];
    if (
    pipe (fd) <
    err_sys
    ("pipe
    if ( (pid =
    fork ()) <
    err_sys
    ("fork
    else if (pid > 0) {
    starš
    close
    (fd[
    write
    (fd[1], "hello world n", 12);
    }
    else {
    otrok
    close
    (fd[
    n =
    read (fd[0], line,
    write
    (STDOUT_FILENO, line,
    }
    exit(0);
}