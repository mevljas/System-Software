/*
Testni program za modul (branje niza)

*/


#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>


#define BUFFSIZE 1024

int main()
{
    int fd, result, len;
    char buf[BUFFSIZE];
    const char *str;
    if ((fd = open("/dev/ogrodje", O_RDONLY)) == -1) {
        perror("open failed");
        return -1;
    }
    while (1)
    {
        if ((result = read(fd, &buf, BUFFSIZE)) < 0) {
            perror("read failed");
            return -1;
        }
        if (!result)
        {
            break;
        }
        // printf("Prebrano: %s\nbajtov: %d\n", buf, result);
        printf("%s", buf);
        
    }

    close(fd);
    return 0;    
    
}
