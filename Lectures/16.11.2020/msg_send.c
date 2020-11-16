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
        perror("msget");
    
    buf.mtype = 8;
    //buf.mtype = 5;
    strcpy( buf.mtext, "abcd efgh ijkl");



    // +1 je, da bo še ničla
    // ne bo flagov
    if ( msgsnd( id, &buf, strlen(buf.mtext) + 1, 0) < 0){
        perror("msgsnd");
    }
}