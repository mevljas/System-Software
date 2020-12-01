/*
Generator:

• Ustvari kos deljenega pomnilnika in semafor. Tega
tudi nastavi na začetno vrednost.

• Na vsaki 2 sekundi zapiše sporočilo v deljeni
pomnilnik. Sporočilo naj ima lastno številko in
nekaj poljubnega besedila. Primer dveh sporočil:
• ID 1: To je sporocilo.
• ID 2: To je sporocilo.

• Pošlje naj 5 sporočil. Zadnje (5.) naj bo prazno ('\0').

• Pred pisanjem v deljeni pomnilnik mora program
zakleniti semafor in po pisanju ga mora odkleniti.
Pred odklepom naj čaka še 2 sekundi. Torej:
program 2 sekundi preživi izven semaforjev in 2
sekundi znotraj semaforjev (varovanega območja).
*/
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define KEY 2020
#define SHM_SIZE 1024

int main(int argc, char* argv[]){
    int shm_id, sem_id;
    char *shm_addr;
    char text[SHM_SIZE];

    union semun {
        int val; // za SETVAL
        struct semid_ds *buf;
        unsigned short *array;
    } arg;

    // ustvarjanje segmenta velikosti SHM_SIZE deljenega pomnilnika
    if( (shm_id = shmget(KEY, SHM_SIZE, 0644 | IPC_CREAT | IPC_EXCL)) == -1 ){
        perror("shmget");
        exit(1);
    }

    // ustvarjanje mnozice semaforjev z enim semaforjem
    if( (sem_id = semget(KEY, 1, 0664 | IPC_CREAT | IPC_EXCL)) == -1 ){
        perror("semget");
        exit(2);
    }
    
    // inicializacija semaforja
    arg.val = 1;
    if( semctl(sem_id, 0, SETVAL, arg) ){
        perror("semctl");
        exit(3);
    }

    // priklop na deljeni pomnilnik (pointer na ta segment)
    if( (shm_addr = (char*) shmat(shm_id, NULL, 0)) == NULL ){
        perror("shmat");
        exit(4);
    }
    
    struct sembuf sem_lock[1];
    struct sembuf sem_unlock[1];
    
    sem_lock[0].sem_num = 0;
    sem_lock[0].sem_op = -1;
    sem_lock[0].sem_flg = 0;

    sem_unlock[0].sem_num = 0;
    sem_unlock[0].sem_op = +1;
    sem_unlock[0].sem_flg = 0;

    // pisanje v deljeni pomnilnik
    for(int i=1; i<=5; i++){

        // zaklepanje semaforja
        if( semop(sem_id, sem_lock, 1) ){
            perror("semop -1");
            exit(5);
        }

        if(i<5){
            sprintf(text, "ID %d: To je sporocilo.", i);
            strncpy(shm_addr, text, SHM_SIZE);
        }
        else shm_addr[0] = '\0';
        
        printf("->SHM '%s'\n", shm_addr);
        sleep(2);

        // odklepanje semaforja
        if( semop(sem_id, sem_unlock, 1) ){
            perror("semop +1");
            exit(6);
        }      

        sleep(2);  
    }

    // odklop od segmenta deljenega pomnilnika
    if( shmdt(shm_addr) ){
        perror("shmdt");
        exit(7);
    }

    return 0;
}