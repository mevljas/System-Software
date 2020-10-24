#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>


#define BUFSIZE 100

void main()
{
    int fd, n;
    char buf[BUFSIZE];

    fd = open("file1", O_RDONLY);

    if (fd < 0)
        perror("open");


    // Return number of read bytes
    n = read(fd, buf, 30);
    printf("n: %d\n", n);

    n = read(fd, buf, 30);
    printf("n: %d\n", n);

    // Returns 0 --> End of the file
    n = read(fd, buf, 30);
    printf("n: %d\n", n);
    



    close(fd);



}