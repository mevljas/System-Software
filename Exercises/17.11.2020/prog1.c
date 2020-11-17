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
    int num=1, fd;
  
    mkfifo(FIFO_NAME, S_IFIFO | 0666);

    printf("Cakam na bralce ...\n");
    fd = open(FIFO_NAME, O_WRONLY);
    printf("Bralec je prisel, napisi mu nekaj:\n");

    while( num > 0){
      num=read(STDIN_FILENO,s,300);
      s[num-1]='\0';
      write(fd, s, num);
    }
    close(fd);
    return 0;
}