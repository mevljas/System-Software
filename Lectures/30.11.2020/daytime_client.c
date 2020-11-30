#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define err(mess) { printf("Error: %s\n", mess); exit(1); }
#define BUFSIZE 1024


int main()
{
    int sockfd, n;
    struct sockaddr_in servaddr;
    char buf[BUFSIZE];

    if ((sockfd = socket( AF_INET, SOCK_STREAM, 0)) < 0) {
        err("Socket");

    }
    // Napolnimo strukturo servaddr
    bzero(&servaddr, sizeof(servaddr)); // To ni nujno, da damo se na nic
    servaddr.sin_family = AF_INET;
    // Treba pazit, da je prov obrjeno
    servaddr.sin_port = htons(13);   // standard daytime server port
    if ( (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr)) <= 0)
        err("inet_pton");


    // Poslje SYN paket
    if (connect( sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 )
        err("connect");

    n = read( sockfd, buf, BUFSIZE);
    buf[n] = '\0'; // Dodamo nicli termiantor, da bo puts vedla, kje je konec niza.
    puts(buf);
    
}

