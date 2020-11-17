#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define FIFO_NAME "myfifo"

int main(void){
    char s[300];
    int num, fd;
    
    mkfifo(FIFO_NAME, S_IFIFO | 0666);

    printf("Cakam na pisatelje ...\n");
    fd = open(FIFO_NAME, O_RDONLY); // klic blokira, če ni nikogar, ki bi pisal.
    printf("Pisatelj je prisel ...\n");

    do {
        num = read(fd, s, 300);
        s[num-1] = '\0';
        printf("%s\n", s);
    } while (num > 0);

    close(fd);
    return 0;
}