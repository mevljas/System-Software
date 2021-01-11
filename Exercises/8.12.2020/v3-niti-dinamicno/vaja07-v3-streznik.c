#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define ST_NITI 2

int client_fds[ST_NITI];

struct my_client {
    int fd;
    int id;
};

void *handle_client(void *arg) {
    struct my_client* mc = (struct my_client*)arg;  

    char buf[512];
    int n = read(mc->fd, buf, 512);
    buf[n] = 0;
    
    double a, b, rez;
    char znak;
    sscanf(buf, "%lf %c %lf", &a, &znak, &b);
    printf("Racunam %0.2lf %c %0.2lf \n", a, znak, b);
    
    switch (znak) {
        case '+':
            rez = a + b;
            break;
        case '-':
            rez = a - b;
            break;
        case '/':
            rez = a / b;
            
            if (b == 0) {
                rez = 0;
                break;
            }

            break;
        case '*':
            rez = a * b;
            break;
        default:
            rez = 0;
    }

    printf("Vracam rezultat: %.2lf \n", rez);
    sprintf(buf, "Rezultat: %.2lf", rez);
    write(mc->fd, buf, 512);

    client_fds[mc->id] = -1;
    free(mc);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Uporaba: ./server port_num");

        return 1;
    }

    int socket_id = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    struct sockaddr_in clientaddr;

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&clientaddr, 0, sizeof(clientaddr));

    uint16_t port;
    sscanf(argv[1], "%hu", &port);

    servaddr.sin_family      = AF_INET;
    servaddr.sin_port        = htons(port);
    // 127.0.0.1 desetiško = 2130706433
    servaddr.sin_addr.s_addr = htonl(2130706433);

    bind(
        socket_id,
        (struct sockaddr*) &servaddr,
        sizeof(servaddr)
    );

    listen(socket_id, SOMAXCONN);

    for (int i = 0; i < ST_NITI; i++) {
        client_fds[i] = -1;
    }

    pthread_t id_niti[ST_NITI];
    socklen_t clilen = sizeof(clientaddr);

    char buf[512];
    struct my_client* mc;
                
    while (1) {
        for (int i = 0; i < ST_NITI; i++) {
            if (client_fds[i] == -1) {
                client_fds[i] = accept(
                    socket_id,
                    (struct sockaddr *) &clientaddr,
                    &clilen
                ); 
                
                printf(
                    "Nov client povezan (%s:%d)\n",
                    inet_ntop(
                        AF_INET,
                        &clientaddr.sin_addr,
                        buf,
                        sizeof(buf)
                    ),
                    clientaddr.sin_port
                );

                mc = malloc(sizeof(struct my_client));
                mc->fd = client_fds[i];
                mc->id = i;

                pthread_create(
                    &id_niti[i],
                    NULL,
                    *handle_client,
                    (void *) mc
                );
            }
        }
        
        usleep(1000);
    }

    pthread_exit(NULL);
}