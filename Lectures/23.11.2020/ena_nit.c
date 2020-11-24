#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

void *funkcija1( void *argument)
{
    printf("Pozdrav od niti!\n");
    // return NULL;

    return ((void *)1);     // Pravieln pristop, ampak dela tud brez castanja
}

int main()
{
    pthread_t tid;
    void *ret_status;  //return status

    // Poiter na funckijo je vedno kar ime funckije.
    if ( pthread_create( &tid, NULL, funkcija1, NULL) ) {
        printf("Napaka: pthread_create");
        exit(1);
    }

    // Joinamo, da bo pocakal.
    // if ( pthread_join(tid, NULL) ) // ce nas ne zanima izh. status
    // Naslov od pointerja je dvojni pointer?
    if ( pthread_join(tid, &ret_status) )
    {
        printf("Napaka: pthread_join");
        exit(1);
        
        

    }


    printf("Izhodni status: %ld.\n", (long)ret_status);

    return 0;
        
}