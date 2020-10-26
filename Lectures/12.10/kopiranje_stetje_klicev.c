#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


#define BUFFSIZE 8



int main() {
    int n, klicev = 0;
    char buf[BUFFSIZE];
    while ( (n = read(STDIN_FILENO, buf, BUFFSIZE) ) > 0) {
        klicev++;
        if ( write(STDOUT_FILENO, buf, n) != n ) {
            printf("Write error.\n");
            exit(1);
        }

    }

    printf("Stevilo klicev: %d", klicev);
        

    if (n < 0) {
        
        printf("Read error.\n");
        exit(1);
    }




    return 0;
}