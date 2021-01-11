#include <sys/socket.h> // socket 
#include <netinet/in.h> 
#include <unistd.h> // read 
#include <arpa/inet.h> // pton 
#include <string.h> 
#include <strings.h> // bzero 
#include <stdio.h> 
#include <stdlib.h>

#define SERV_PORT 9876
#define MAXLINE 1024

/*
gcc client.c -o client
*/


int main(int argc, char const *argv[])
{
    int sockfd, n;
    char buf[MAXLINE + 1];
    struct sockaddr_in servaddr;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket"); exit(1);
    }

    if(memset(&servaddr, 0, sizeof(servaddr)) == (void *)-1)
    {
        perror("memset"); exit(1);
    }

    //napolnemo servaddr
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT); //port na katerem se nahaja server
    if(inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) < 0)
    {
        perror("inet_pton"); exit(1);
    }

    if ((connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))) < 0)
    {
        perror("connect"); exit(1);
    }
   
    while ( fgets( buf, MAXLINE, stdin) ) // != NULL
    {
        if ( write( sockfd, buf, strlen(buf)) != strlen(buf))
        {
            perror("write"); exit(1);
        }
                
        n = read( sockfd, buf, sizeof(buf));
        buf[n] = '\0';
        puts(buf);
    }

    return 0;
}
