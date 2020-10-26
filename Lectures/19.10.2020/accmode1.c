#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define err(mess) { fprintf(stderr,"Error: %s.", mess); exit(1); }

int main(int argc, char *argv[]) {
    int accmode, val, fd;
    if (argc != 2)
        err("usage: ime_programa datoteka")

        fd = open(argv[1], O_RDONLY);

    if ( (val = fcntl(fd, F_GETFL, 0)) < 0)
        err("fcntl error")
    
    accmode = val & O_ACCMODE;// maska O_ACCMODEje 3
    if (accmode == O_RDONLY)
        printf("read only");
    else if (accmode == O_WRONLY)
        printf("write only");
    else if (accmode == O_RDWR)
        printf("read write");
    else 
        err("unknown access mode")
    if (val & O_APPEND)
        printf(", append");
    if (val & O_NONBLOCK)
        printf(", nonblocking");
    
    putchar('\n');
    return 0;
}