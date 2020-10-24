#include <fcntl.h>
#include <unistd.h>

// Add imports for perror below
#include <stdio.h>
#include <errno.h>


void main()
{
    int fd;

    fd = open("file1", O_RDONLY);

    if (fd < 0)
        // output error
        // Output: "open: No such file or directory"
        perror("open");

    // ...


    close(fd);



}