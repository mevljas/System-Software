#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 1024
#define err(mess) { printf("Error: %s\n", mess); exit(1); }
#define SERV_PORT 9876

void main()
{
        int sockfd, n;
        struct sockaddr_in servaddr;
        char buf[BUFSIZE], buf_recv[BUFSIZE];
        
        if ( (sockfd = socket( AF_INET, SOCK_STREAM, 0)) < 0)
                err("socket")

        // Napolnimo strukturo servaddr
        bzero( &servaddr, sizeof(servaddr));
        servaddr.sin_family        = AF_INET;
        servaddr.sin_port = htons(SERV_PORT);
        if ( inet_pton( AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0)
                err("inet_pton")
                                
        if ( connect( sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
                err("connect")
                
        while ( fgets( buf, BUFSIZE, stdin) ) // != NULL
        {
                if ( write( sockfd, buf, strlen(buf)) != strlen(buf))
                        err("write")
                        
                n = read( sockfd, buf_recv, BUFSIZE);
                buf_recv[n] = '\0';
                puts(buf_recv);
        }
}
