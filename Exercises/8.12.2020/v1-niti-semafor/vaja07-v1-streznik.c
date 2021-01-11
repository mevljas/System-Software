#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h> 

#define BUFSIZE 1024
#define err(mess) { perror(mess); exit(1); }
#define SERV_PORT 9876
#define N_THREADS 2

struct params {
    int fd;
    int port;
};
struct params *argumenti;
sem_t semafor;

float calculate(double a, double b, char operator);
void *funkcija_niti(void *arg);

/*
gcc server.c -o server
*/


void main()
{
    int listenfd, connfd, n;
    struct sockaddr_in servaddr;
    struct sockaddr_in clientaddr;
    socklen_t clientaddr_len;
    char buf[BUFSIZE], buf_send[BUFSIZE+100];
    pid_t childpid;
    int optval = 1;
    
    sem_init(&semafor, 0, N_THREADS);

    //niti
    argumenti = (struct params *)calloc(N_THREADS*2+10, sizeof(struct params));
    pthread_t *nit = (pthread_t *)calloc(N_THREADS*2+10, sizeof(pthread_t));
    

    if ( (listenfd = socket( AF_INET, SOCK_STREAM, 0)) < 0)
        err("socket")

    setsockopt( listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    // Napolnimo strukturo servaddr
    //bzero( &servaddr, sizeof(servaddr)); deprecated
    if(memset(&servaddr, 0, sizeof(servaddr)) == (void *)-1)
        err("memset");
    if(memset(&clientaddr, 0, sizeof(clientaddr)) == (void *)-1)
        err("memset");
    clientaddr_len = sizeof(clientaddr);
    

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if ( bind( listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        err("bind");

    if ( listen( listenfd, 1000) < 0)
        err("listen");

    
    printf("Listening for connections on port %d..\n", SERV_PORT);

    while(1) {        

        //semafor (zagotovimo, da je naenkrat v uporabi le N_THREADS niti in da tega ne delamo z busy wait)
        sem_wait(&semafor);

        usleep(100);

        if ( (connfd = accept( listenfd, (struct sockaddr *)&clientaddr, &clientaddr_len)) < 0)
            err("accept");

        //ustvarimo nit, ki handla povezavo
        argumenti[connfd].port = clientaddr.sin_port;
        argumenti[connfd].fd = connfd;

        pthread_create(&nit[connfd], NULL, funkcija_niti, (void *)&argumenti[connfd]);
    }     
}


float calculate(double a, double b, char operator)
{
    switch(operator)
    {
        case '+':
            return a + b;
            break;
        case '-':
            return a - b;
            break;
        case '*':
            return a * b;
            break;
        case '/':
            return a / b;
            break;
    }
    return 0.0;
}

void *funkcija_niti(void *arg)
{
    //od zdaj je nit samosvoja, obstaja, dokler je povezava odprta
    if(pthread_detach(pthread_self()))
        err("pthread_detach")

    int port = ((struct params *)arg)->port;
    int myfd = ((struct params *)arg)->fd;
    int n;
    char buf[BUFSIZE], buf_send[BUFSIZE+100];

    printf("Accepted connection with client from port %d, fd = %d\n", port, myfd);

    //strezemo klientu
    while( (n = read(myfd, buf, BUFSIZE)) > 0) {
        buf[n-1] = '\0'; // namesto '\n'
        
        float a, b, result;
        char operator;

        a=0;b=0;result=0;
        // procesiranje
        sscanf(buf, "%f %c %f", &a, &operator, &b);
        result = calculate(a, b, operator);

        sprintf(buf_send, " = %f", result);
        printf("fd[%d]: Prejel sporočilo: '%s', pošiljam odgovor: '%f'..\n",myfd, buf, result);
        if ( write(myfd, buf_send, strlen(buf_send)) != strlen(buf_send))
            err("write");
    }        
    close(myfd);

    //release resource
    sem_post(&semafor);

    pthread_exit(0);
}