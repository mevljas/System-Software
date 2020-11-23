#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int globalna;

void *funkcija1( void *argument)
{
        int j;

        for ( int i=0; i<10000; i++) {
                j = globalna;
                j++;
                printf("-");
                fflush(stdout);
                globalna = j;
        }

        return NULL;
}

int main()
{
        pthread_t tid;        
        
        globalna = 0;

        if ( pthread_create( &tid, NULL, funkcija1, NULL) )        {
                printf("Napaka: pthread_create");
                exit(1);
        }

        for ( int i=0; i<10000; i++) {
                printf("+");
                fflush(stdout);
                globalna++;
        }

        if ( pthread_join( tid, NULL) ) {
                printf("Napaka: pthread_join");
                exit(1);
        }

        printf("globalna na koncu: %d.\n", globalna);

        return 0;
}
