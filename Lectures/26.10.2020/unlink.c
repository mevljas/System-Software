#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


void main()
{

    // Check if file can be opened
    if (open("file1", O_WRONLY) < 0)
        perror("open file1");

    if (unlink("file1") < 0)
        perror("unlink");

    printf("File unlinked.\n");
    sleep(10);
    printf("End of sleep.\n");


}