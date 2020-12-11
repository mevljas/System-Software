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
#include <errno.h>
#include <pthread.h>

#define BUFSIZE 1024
#define SERV_PORT 9876
// Stevilo threadov.
#define N_THR 2

struct arg_struct
{
   int connfd;
   struct sockaddr_in client_addr;
} *args;

// Thread function
void *funkcija1(void *arguments)
{
        struct arg_struct *args = arguments;
        int connfd = args->connfd;
        struct sockaddr_in client_addr = args->client_addr;
        float a, b, solution;
        char op;
        int n;
        char buf[BUFSIZE], buf_send[BUFSIZE + 100];

        while ((n = read(connfd, buf, BUFSIZE)) > 0)
        {
                buf[n - 1] = '\0'; // namesto '\n'
                sscanf(buf, "%f %c %f", &a, &op, &b);
                // printf("a %f\n", a);
                // printf("op  %c\n", op);
                // printf("b %f\n", b);
                printf("Streznik: racunam izraz \"%s\"\n", buf);

                switch (op)
                {
                case '+':
                        solution = a + b;
                        break;

                case '-':
                        solution = a - b;
                        break;

                case '*':
                        solution = a * b;
                        break;
                case '/':
                        solution = a / b;
                        break;
                default:
                        printf("Napaka v izrazu!\n");
                        break;
                }

                sprintf(buf_send, "%f", solution);
                if (write(connfd, buf_send, strlen(buf_send)) != strlen(buf_send))
                        perror("write");
        }
        close(connfd);
        printf("Konec niti.\n");

        return NULL;
}

void main(int argc, char const *argv[])
{
        int port = SERV_PORT;
        char *ip = (char *)malloc(50);
        int listenfd, connfd;
        struct sockaddr_in servaddr;
        // pid_t childpid;
        int optval = 1;
        struct sockaddr_in client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
        pthread_t tid[N_THR];
        args = malloc(sizeof(struct arg_struct) * 1);

        if (argc == 3)
        {
                printf("Custom arguments provided.\n");
                strcpy(ip, argv[1]);
                printf("2");
                port = atoi(argv[2]);
        }
        {
                printf("Using default arguments.\n");
                strcpy(ip, "127.0.0.1");
        }
        printf("IP: %s\n", ip);
        printf("Port: %d\n", port);

        if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                perror("socket");

        // Napolnimo strukturo servaddr
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port);

        if (inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0)
                perror("inet_pton");

        if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
                perror("bind");
        if (listen(listenfd, SOMAXCONN) < 0)
                perror("listen");

        while (1)
        {
                for (int i = 0; i < N_THR; i++)
                {
                        if ((connfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_addr_size)) < 0)
                                perror("accept");

                        printf("Sprejel povezavo z odjemalcem. Njegov IP: %s, vrata: %d\n", inet_ntoa(client_addr.sin_addr), (int)ntohs(client_addr.sin_port));
                        printf("zaganjam nit %d\n", i);

                        args->connfd = connfd;
                        args->client_addr = client_addr;

                        if (pthread_create(&tid[i], NULL, funkcija1, args))
                        { 
                                perror("pthread_create");
                                exit(1);
                        }
                }

                // Joinamo, da bo pocakal.
                for (int i = 0; i < N_THR; i++){
                        if ( pthread_join(tid[i], NULL) )
                        {
                                perror("Napaka: pthread_join");
                                exit(1);
                        
                        }
                }
                printf("Vse niti združene.\n");
        }

        close(listenfd);
}