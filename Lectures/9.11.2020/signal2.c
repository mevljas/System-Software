#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void signal_handler( int sig_number) 
{
    printf("Prejel sem signal %d.\n", sig_number);
}

void main()
{
    signal( SIGALRM, signal_handler);
    alarm(10);

    // Da se program ne zakljuci
    while (1)
        ;

    
}
