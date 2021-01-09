#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

void main()
{
    key_t key;
    int id;
    char *addr;

    if ((key = ftok("/usr", 'x')) < 0)
        perror("ftok");

    if ( ( id = shmget( key, 1024, 0666 | IPC_CREAT)) < 0 )
        perror("shmget");

    // Nulo castamo
    if ((addr = (char *) shmat( id, (void *)0, 0)) < 0)
        perror("shmat");

    
    // branje

    puts(addr);


    // Detach
    shmdt( addr);   // bi blo treb preverti, ce uspe..

    // lahko bi tudi to nrdili
    // shmctl IPC_RMID



}
