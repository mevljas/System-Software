#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MOD_PATH "/dev/ogrodje"
#define BUF_SIZE 512

int main(int argc, char* argv[]){
    int read_fd, write_fd, read_bytes, written_bytes;
    char buffer[BUF_SIZE];

    if( (read_fd = open(MOD_PATH, O_RDONLY)) == -1 ){
        perror("open");
        exit(1);
    }

    if( (write_fd = open(MOD_PATH, O_WRONLY)) == -1 ){
        perror("open");
        exit(1);
    }
    memcpy(buffer, "string", 6);

    if( (written_bytes = write(write_fd, buffer, 6)) == -1 ){
        perror("write");
        exit(2);
    }

    if( (written_bytes = write(write_fd, buffer, 6)) == -1 ){
        perror("write");
        exit(2);
    }

    if( (read_bytes = read(read_fd, buffer, BUF_SIZE)) == -1 ){
        perror("read");
        exit(2);
    }

    buffer[read_bytes] = 0;
    printf("%s\n", buffer);

    if( close(write_fd) ){
        perror("close");
        exit(3);
    }


    if( close(read_fd) ){
        perror("close");
        exit(3);
    }

    return 0;
}