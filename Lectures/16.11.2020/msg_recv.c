#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

struct mymesg {
    long mtype;
    char mtext[128];
};

void main()
{

    // buf je primerek te stutkure
    struct mymesg buf;
    key_t key;
    int id;

    // Kljuc si lahko zmislimo
    // key = 12345;

    // lahko uporabimo ftok - boljse Ker bo zelo erjetno  unique ( ni pa ziher)
    if ( (key = ftok("/home", 'a')) < 0){
        perror("ftok");
    }

    // mode je read write za vse
    // Ce ne obstaja, ga ustvari
    if ( (id = msgget( key, 0666 | IPC_CREAT)) < 0)
                perror("msgget");

    //if ( msgrcv( id, &buf, sizeof(buf.mtext), 8/*tip*/, 0/*flag*/) < 0)
    //if ( msgrcv( id, &buf, sizeof(buf.mtext), 5/*tip*/, 0/*flag*/) < 0)
    if ( msgrcv( id, &buf, sizeof(buf.mtext), 0/*tip*/, 0/*flag*/) < 0)




    if ( msgrcv( id, &buf, sizeof(buf.mtext), 5/*tip*/, IPC_NOWAIT) < 0)
                perror("msgrcv");

        puts(buf.mtext);

        if ( msgctl( id, IPC_RMID, NULL) < 0)
                perror("msgctl");
    
}