#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <errno.h>
#include <pthread.h>

#define KEY 1111
#define SHM_SIZE 7
#define BUFSIZE 4

// Stevilo threadov.
#define N_thr 3

struct mymesg
{
    long mtype;
    char mtext[10];
};

union semun
{
    int val;
    struct semid_ds *ptr;
    unsigned short *array;
};

int globalna;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void *funkcija1(void *argument)
{
    sleep(2);

    int port = (int)argument;
    // SOCKETS
    char *ip = (char *)malloc(50);
    int sockfd, n;
    struct sockaddr_in servaddr;
    char buf_send[BUFSIZE], buf_recv[BUFSIZE];

    strcpy(ip, "127.0.0.1");

    strcpy(buf_send, "DoS");

    printf("IP: %s\n", ip);
    printf("Port: %d\n", port);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror("socket");

    // Napolnimo strukturo servaddr
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0)
        perror("inet_pton");

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        perror("connect");

    if (write(sockfd, buf_send, strlen(buf_send)) != strlen(buf_send))
        perror("write");

    if ((n = read(sockfd, buf_recv, BUFSIZE)) < 0)
    {
        perror("read");
    }
    buf_recv[n] = '\0';
    puts(buf_recv);

    pthread_mutex_lock(&mut);
    globalna += atoi(buf_recv);
    pthread_mutex_unlock(&mut);

    close(sockfd);
    return NULL;

    // return ((void *)1);     // Pravieln pristop, ampak dela tud brez castanja
}

int main(int argc, char *argv[])
{
    int shm_id, msg_id;
    char *shm_addr;
    char text[SHM_SIZE];
    struct mymesg msg_buf;

    // odpiranje segmenta velikosti SHM_SIZE deljenega pomnilnika
    if ((shm_id = shmget(KEY, SHM_SIZE, 0644)) == -1)
    {
        perror("shmget");
        exit(1);
    }

    // priklop na deljeni pomnilnik (pointer na ta segment)
    if ((shm_addr = (char *)shmat(shm_id, NULL, 0)) == NULL)
    {
        perror("shmat");
        exit(3);
    }

    strncpy(text, shm_addr, SHM_SIZE);
    printf("%s\n", text);

    char *pt;
    pt = strtok(text, ",");
    int stevil[2];
    int i = 0;

    while (pt != NULL)
    {
        int a = atoi(pt);
        stevil[i++] = a;
        pt = strtok(NULL, ",");
    }

    printf("%d\n", stevil[0]);
    printf("%d\n", stevil[1]);

    memset(shm_addr, 0, SHM_SIZE);

    // odklop od segmenta deljenega pomnilnika
    if (shmdt(shm_addr))
    {
        perror("shmdt");
        exit(7);
    }

    // izbris deljenega pomnilnika
    if (shmctl(shm_id, IPC_RMID, NULL))
    {
        perror("shmctl");
        exit(8);
    }

    if ((msg_id = msgget(stevil[0], 0666)) < 0)
        perror("msgget");

    if (msgrcv(msg_id, &msg_buf, sizeof(msg_buf.mtext), stevil[1] /*tip*/, 0 /*flag*/) < 0)
        perror("msgget");

    puts(msg_buf.mtext);

    if (msgctl(msg_id, IPC_RMID, NULL) < 0)
        perror("msgctl");

    printf("%s\n", msg_buf.mtext);

    int port = atoi(msg_buf.mtext);

    printf("%d\n", port);

    pthread_t tid[N_thr];
    void *ret_status; //return status

    for (int i = 0; i < N_thr; i++)
    {
        // Poiter na funckijo je vedno kar ime funckije.
        if (pthread_create(&tid[i], NULL, funkcija1, (void *)port))
        {
            printf("Napaka: pthread_create");
            exit(1);
        }
    }

    // Joinamo, da bo pocakal.
    // if ( pthread_join(tid, NULL) ) // ce nas ne zanima izh. status
    // Naslov od pointerja je dvojni pointer?
    for (int i = 0; i < N_thr; i++)
    {
        if (pthread_join(tid[i], NULL))
        {
            printf("Napaka: pthread_join");
            exit(1);
        }
    }

    printf("semfaro: %d\n", globalna);

    // TODO: Semafor

    return 0;
}