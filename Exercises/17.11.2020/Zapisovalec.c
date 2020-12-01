/*
Zapisovalec:

• Odpre datoteko za pisanje – ime dobi kot
vhodni argument. Pridobi indentifikator
sporočilne vrste.

• Bere sporočila iz vrste in jih zapisuje v
datoteko.

• Ko prejme prazno sporočilo, zapre datoteko,
pobriše sporočilno vrsto in se konča.
*/
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>
#include <fcntl.h>

#define KEY 2020
#define SHM_SIZE 1024

int main(int argc, char* argv[]){
    int file_out, msg_id;

    struct mymsg {
        long mtype;
        char mtext[SHM_SIZE];
    } message;

    // program mora imeti podano ime datoteke
    if(argc != 2){
        printf("Uporaba: ./Zapisovalec <ime_datoteke>\n");
        exit(1);
    }
    
    // pridobitev identifikatorja sporocilne vrste
    if( (msg_id = msgget(KEY, 0644)) == -1 ){
        perror("msgget");
        exit(2);
    }

    // odpiranje izhodne datoteke
    if( (file_out = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1 ){ // rabim prava dovoljenja za odpiranje fila
        perror("open");
        exit(3);
    }

    message.mtype = 0;
    while(1){
        // branje sporocila iz vrste
        if( msgrcv(msg_id, &message, SHM_SIZE, 1, 0) == -1){
            perror("msgrcv");
            exit(4);
        }

        printf("MSQ->file '%s'\n", message.mtext);

        if(message.mtext[0] == '\0') break;
        
        // zapisovanje v datoteko
        if( write(file_out, message.mtext, strlen(message.mtext)) == -1 ){
            perror("write");
            exit(5);
        }
    }

    // zapiranje datoteke
    if( close(file_out) ){
        perror("close");
        exit(6);
    }

    // pobrisanje sporocilne vrste
    if( msgctl(msg_id, IPC_RMID, NULL) ){
        perror("msgctl");
        exit(7);
    }

    return 0;
}