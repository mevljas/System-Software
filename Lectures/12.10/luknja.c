#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define err(message) { printf("Napaka: %s\n", message); exit(0); }

int main(){
    int fd;
    char buf1[] = "abcdefghij";
    char buf2[] = "ABCDEFGHIJ";
    
    

    if ( ( fd = open("luknja.txt", O_WRONLY | O_CREAT)) < 0)
        err("open")

    if ( write( fd, buf1, strlen(buf1)) != strlen(buf1) )
        err("write")


    if ( lseek( fd, 30, SEEK_SET) < 0)
        err("lseek")


    if ( write( fd, buf2, strlen(buf2)) != strlen(buf2) )
        err("write2")

    if ( close(fd) < 0)
        err("close")

}