#include <stdio.h>
#include <signal.h>

void signal_handler( int sig_number) 
{
    printf("Prejel sem signal %d.\n", sig_number);
}

void main()
{
    signal( SIGUSR1, signal_handler);

    // Da se program ne zakljuci
    while (1)
        ;

    
}
