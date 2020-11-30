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
        int listenfd, connfd, n;
        struct sockaddr_in servaddr;
        char buf[BUFSIZE], buf_send[BUFSIZE+100];
        
        if ( (listenfd = socket( AF_INET, SOCK_STREAM, 0)) < 0)
                err("socket")

        // Napolnimo strukturo servaddr
        bzero( &servaddr, sizeof(servaddr));
        servaddr.sin_family        = AF_INET;
        servaddr.sin_port = htons(SERV_PORT);
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        
        if ( bind( listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
                err("bind")
        if ( listen( listenfd, 1000) < 0)
                err("listen")

        while(1) {                
                if ( (connfd = accept( listenfd, (struct sockaddr *)NULL, NULL)) < 0)
                        err("accept")
                                                
                while( (n = read( connfd, buf, BUFSIZE)) > 0) {
                        // procesiranje                        
                        sprintf( buf_send, "Dobil sem %s.", buf);
                        if ( write( connfd, buf_send, strlen(buf_send)) != strlen(buf_send))
                                err("write")
                }
                close(connfd);
        }
        
}
