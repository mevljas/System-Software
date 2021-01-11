// CLIENT

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Uporaba: ./client port_num");

        return 1;
    }

    int socket_id = socket(AF_INET, SOCK_STREAM, 0);

    uint16_t port;
    sscanf(argv[1], "%hu", &port);

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family      = AF_INET;
    servaddr.sin_port        = htons(port);
    // 127.0.0.1 desetiško = 2130706433
    servaddr.sin_addr.s_addr = htonl(2130706433);

    int conn_status = connect(
        socket_id,
        (struct sockaddr*) &servaddr,
        sizeof(servaddr)
    );

    char buf[512];

    printf("Vpiši račun: ");

    fgets(buf,511,stdin);
    write(socket_id, buf, strlen(buf));

    int n  = read(socket_id, buf, 512);
    buf[n] = 0;

    printf("%s\n", buf);

    close(socket_id);

    return 0;
}