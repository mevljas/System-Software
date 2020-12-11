#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BUFSIZE 1024
#define SERV_PORT 9876


void main(int argc, char const *argv[])
{
        int port = SERV_PORT;
        char *ip = (char *) malloc(50);
        int sockfd, n;
        struct sockaddr_in servaddr;
        char buf[BUFSIZE], buf_recv[BUFSIZE];

        if (argc == 3)
        {
                printf("Custom arguments provided.\n");
                strcpy(ip, argv[1]);  
                port = atoi(argv[2]);
                
        }
        {
                printf("Using default arguments.\n");
                strcpy(ip, "127.0.0.1");  
        }
        
        printf("IP: %s\n", ip);
        printf("Port: %d\n", port);
        
        if ( (sockfd = socket( AF_INET, SOCK_STREAM, 0)) < 0)
                perror("socket");

        // Napolnimo strukturo servaddr
        memset( &servaddr, 0,sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port);
        if ( inet_pton( AF_INET, ip, &servaddr.sin_addr) <= 0)
                perror("inet_pton");
                                
        if ( connect( sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
                perror("connect");
                
        while ( fgets( buf, BUFSIZE, stdin) ) // != NULL
        {
                if ( write( sockfd, buf, strlen(buf)) != strlen(buf))
                        perror("write");
                        
                if ((n = read( sockfd, buf_recv, BUFSIZE)) < 0 ){
                        perror("read");
                }
                buf_recv[n] = '\0';
                puts(buf_recv);
        }


        close(sockfd);
}
