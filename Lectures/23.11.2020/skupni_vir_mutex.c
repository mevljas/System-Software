#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int globalna;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void *funkcija1( void *argument)
{
        int j;

        // for ( int i=0; i<10; i++) {
        for ( int i=0; i<2000; i++) {

            pthread_mutex_lock( &mut);
                j = globalna;
                // sleep(1);
                j++;
                printf("-");
                fflush(stdout);
                globalna = j;
                pthread_mutex_unlock( &mut);
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

        for ( int i=0; i<2000; i++) {
        // for ( int i=0; i<10; i++) {
                printf("+");
                fflush(stdout);
                pthread_mutex_lock( &mut);
                globalna++;
                pthread_mutex_unlock( &mut);        //tle se konca kriticno
                // sleep(1);
        }

        if ( pthread_join( tid, NULL) ) {
                printf("Napaka: pthread_join");
                exit(1);
        }

        printf("globalna na koncu: %d.\n", globalna);

        return 0;
}
