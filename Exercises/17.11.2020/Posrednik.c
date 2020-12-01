/*
Posrednik:

• Ustvari novo sporočilno vrsto. Pridobi
identifikatorja semaforja in deljenega pomnilnika.

• Na eno sekundo dostopa do deljenega pomnilnika
in prebere vsebino. Dostop je usklajen z
generatorjem s semaforji.

• Vsebino nato pošlje v sporočilno vrsto, vendar
samo, če je bilo drugačno od prejšnjega. S tem
želimo zagotoviti, da bodo v sporočilni vrsti samo
unikatna sporočila.

• Zanka branja in pošiljanja sporočil se zaključi, ko
prebere prazno sporočilo (5. sporočilo
generatorja). Prazno sporočilo prav tako pošlje
naprej v sporočilno vrsto. Nato izbriše semaforno
polje in deljeni pomnilnik ter se konča.
*/
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>

#define KEY 2020
#define SHM_SIZE 1024

int main(int argc, char* argv[]){
    int shm_id, sem_id, msg_id;
    char *shm_addr;
    char text[SHM_SIZE], previous_text[SHM_SIZE];


    struct mymsg {
        long mtype;
        char mtext[SHM_SIZE];
    } message;

    // odpiranje segmenta velikosti SHM_SIZE deljenega pomnilnika
    if( (shm_id = shmget(KEY, SHM_SIZE, 0644)) == -1 ){
        perror("shmget");
        exit(1);
    }

    // odpiranje mnozice semaforjev z enim semaforjem
    if( (sem_id = semget(KEY, 1, 0664)) == -1 ){
        perror("semget");
        exit(2);
    }

    struct sembuf sem_lock[1];
    struct sembuf sem_unlock[1];
    
    sem_lock[0].sem_num = 0;
    sem_lock[0].sem_op = -1;
    sem_lock[0].sem_flg = 0;

    sem_unlock[0].sem_num = 0;
    sem_unlock[0].sem_op = +1;
    sem_unlock[0].sem_flg = 0;

    // priklop na deljeni pomnilnik (pointer na ta segment)
    if( (shm_addr = (char*) shmat(shm_id, NULL, 0)) == NULL ){
        perror("shmat");
        exit(3);
    }

    // ustavarjanje sporocilne vrste
    if( (msg_id = msgget(KEY, 0644 | IPC_CREAT | IPC_EXCL)) == -1 ){
        perror("msgget CREAT");
        exit(4);
    }

    // branje iz deljenega pomnilnika
    message.mtype = 1;
    while(text[0] != '\0'){

        // zaklepanje semaforja
        if( semop(sem_id, sem_lock, 1) ){
            perror("semop -1");
            exit(5);
        }

        strncpy(text, shm_addr, SHM_SIZE);

        // ce je sporocilo razlicno od prejsnjega
        if( strcmp(previous_text, text) ){
            strncpy(previous_text, text, SHM_SIZE);
            
            strncpy(message.mtext, text, SHM_SIZE);
            printf("SHM->MSQ '%s'\n", text);

            // posiljanje v sporocilno vrsto
            if( msgsnd(msg_id, &message, strlen(message.mtext)+1, 0) ){
                perror("msgsnd");
                exit(6);
            }
        }
        else{
            printf("Prebran je bil duplikat sporocila.\n");
        }

        // odklepanje semaforja
        if( semop(sem_id, sem_unlock, 1) ){
            perror("semop +1");
            exit(6);
        }

        sleep(1);
    }

    // odklop od segmenta deljenega pomnilnika
    if( shmdt(shm_addr) ){
        perror("shmat");
        exit(7);
    }

    // izbris deljenega pomnilnika
    if( shmctl(shm_id, IPC_RMID, NULL) ){
        perror("shmctl");
        exit(8);
    }

    // izbris semafornega polja
    if( semctl(sem_id, 0, IPC_RMID) == -1 ){
        perror("semctl");
        exit(9);
    }

    return 0;
}