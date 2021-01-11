#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "w-socket.c"
#include "w-file.c"
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#define error(message) { fprintf(stderr, "%s\n", message); exit(1); }
#define PORT 1948
#define LISTEN_QUEUE 1000
#define READ_BUFFER 5000
#define NR_THR 2 // število niti

int threadUsed = 0;

void *processClientMessage(void *connectedFd);
bool tryCalculate(double numberFirst, double numberLast, char operator, double *result, char **errorMessage);

int main(int argc, char const *argv[]) {
    int sockFd, connectedFd, optval = 1;
    struct sockaddr_in servaddr;
    struct sockaddr_in clientaddr;
    pthread_t id[NR_THR];
    char str[INET_ADDRSTRLEN]; 

    // create socket
    sockFd = Socket(AF_INET, SOCK_STREAM, 0);

    // local port reuse
    if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        perror("setsockopt");
    }

    // bind
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    Bind(sockFd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    // listen
    Listen(sockFd, LISTEN_QUEUE);

    printf("Server is running on port: %d\n", PORT);

    for (;;) {

        printf("Waiting to client to connect...\n");

        // accept
        connectedFd = Accept(sockFd, (struct sockaddr *) &clientaddr, sizeof(clientaddr));

        // wait until all threads are finshed
        if (threadUsed >= NR_THR) {
            printf("All threads are used!\n");
            for (int i=0; i < NR_THR; i++) {
                pthread_join(id[i], NULL);
            }
        }

        // client connection is accepted, process..
        printf("Client connection accepted. IP: %s, port:%d\n", 
            inet_ntop(AF_INET, &clientaddr.sin_addr, str, INET_ADDRSTRLEN), 
            ntohs(clientaddr.sin_port));
        pthread_create(&id[threadUsed++], NULL, processClientMessage, &connectedFd);
    }

    return 0;
}

void *processClientMessage(void *arg) {
    int connectedFd = *((int *) arg);
    char readBuff[READ_BUFFER], resultBuff[32];
    double numberFirst, numberLast, result;
    char *errorMessage, operator;
    int resultSize, size = 0;

    do {    
        // read client message
        size = Read(connectedFd, readBuff, READ_BUFFER);

        // parse client message
        sscanf(readBuff, "%lf %c %lf", &numberFirst, &operator, &numberLast);
        printf("N1: %f, N2: %f, O: %c\n", numberFirst, numberLast, operator);

        if (tryCalculate(numberFirst, numberLast, operator, &result, &errorMessage)) {
            // success result
            resultSize = snprintf(resultBuff, sizeof(resultBuff), "%f", result);
            Write(connectedFd, resultBuff, resultSize);
        } else {
            // error result
            Write(connectedFd, errorMessage, strlen(errorMessage));
        }
    } while (size > 0);

    --threadUsed;
    Close(connectedFd);
    printf("\n");
    
    return NULL;
}

bool tryCalculate(double numberFirst, double numberLast, char operator, double *result, char **errorMessage) {
    switch (operator)
    {
        case '+':
            *result = numberFirst + numberLast;
            break;    
        case '-':
            *result = numberFirst - numberLast;
            break;   
        case '*':
            *result = numberFirst * numberLast;
            break;   
        case '/':
            if (numberLast == 0) {
                *errorMessage = "Cannot divide by zero!";
                return false;
            }

            *result = numberFirst / numberLast;
            break;           
        default:
            *errorMessage = "Operation is not valid. Only +, -, *, / are allowed.";
            return false;
            break;
    }

    return true;
}