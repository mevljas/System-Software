#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>


union semun {
    int val;
    struct semid_ds *ptr;
    unsigned short *array;
};

void main( int argc, char *argv[])
{
    key_t key;
    int id;
    union semun arg;
    struct sembuf operacije[1];

    if (argc != 2)
    {
        printf("Uporaba: %s n\n", argv[0]);
        exit(0);
    }
    

    if ((key = ftok("/home", 'b')) < 0)
        perror("ftok");
    
    if ( (id = semget( key, 1, 0666 | IPC_CREAT)) < 0)
        perror("semget");

    operacije[0].sem_num = 0;    //stevilka semaforja
    operacije[0].sem_op = atoi(argv[1]);    //sprememba semaforja
    // operacije[0].sem_flg = 0;    //brez flag
     operacije[0].sem_flg = IPC_NOWAIT;    //da ne bo blokiral 

    if (semop( id, operacije, 1) < 0)
        perror("semop");

    // Pogledamo vrednost semaforja
    int vrednost;
    if ( (vrednost = semctl( id, 0, GETVAL, arg)) < 0)
        perror("semctl");
    
    printf("vrednost semaforja: %d\n", vrednost);



}