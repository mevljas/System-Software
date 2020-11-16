#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>



// Profesorjeva resitev.

void main()
{
        key_t key;
        int id;
        char *addr;

        if ((key = ftok("/usr", 'x')) < 0)
                perror("ftok");

        if ( (id = shmget( key, 0, 0666)) < 0)
                perror("shmget");

        if ((addr = (char *)shmat( id, (void *)0, 0)) < 0)
                perror("shmat");

        puts(addr);

        shmdt( addr); // if ...

        // shmctl ... IPC_RMID
}