#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <errno.h>

union semun {
    int val;
    struct semid_ds *ptr;
    unsigned short *array;
};

void main()
{
    key_t key;
    int id;
    union semun arg;

    if ((key = ftok("/home", 'b')) < 0)
        perror("ftok");
    
    if ( (id = semget( key, 1, 0666 | IPC_CREAT)) < 0)
        perror("semget");

    arg.val = 0; // zacetna vrednost semaforja
    if ( semctl( id, 0, SETVAL, arg) < 0)
        perror("semctl");
}