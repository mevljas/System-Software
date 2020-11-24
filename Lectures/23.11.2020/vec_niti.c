#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

// Stevilo threadov.
#define N_thr 5

void *funkcija1( void *argument)
{
    printf("Pozdrav od niti %ld!\n", (long) argument);
    return NULL;

    // return ((void *)1);     // Pravieln pristop, ampak dela tud brez castanja

}

int main()
{
    pthread_t tid[N_thr];
    void *ret_status;  //return status

    for (int i = 0; i < N_thr; i++)
    {
        // Poiter na funckijo je vedno kar ime funckije.
        if ( pthread_create( &tid[i], NULL, funkcija1, (void *)i) ) {
            printf("Napaka: pthread_create");
            exit(1);
        }
    }
    
    // Joinamo, da bo pocakal.
    // if ( pthread_join(tid, NULL) ) // ce nas ne zanima izh. status
    // Naslov od pointerja je dvojni pointer?
    for (int i = 0; i < N_thr; i++){
        if ( pthread_join(tid[i], NULL) )
        {
            printf("Napaka: pthread_join");
            exit(1);
            
            

        }
    }

   
    


    printf("Izhodni status: %ld.\n", (long)ret_status);

    return 0;
        
}