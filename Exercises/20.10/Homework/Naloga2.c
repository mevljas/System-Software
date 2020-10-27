// Funkcije open, close; kostante, ki jih uporaibmo pri open(): O_RDONLY, O_WRONLY, O_RDWR; O_APPEND, O_CREAT, O_ECL, O_TRUNC, O_SYNC
#include <fcntl.h>
// veliko unix sistemskih klicev: read, write, lčseek, fork, chdir, chown, ...
#include <unistd.h>
// Spremeljivke stdin, stdout, stderr; funkcije: gets, getc, scanf, perror, printf, fprintf
#include <stdio.h>
#include <errno.h>
// funkcije: exit, malloc, atoi, rand, ...
#include <stdlib.h>

/*
Naloga 2
-Napišite svojo verzijo terminalskega ukaza cat s pomočjo read , write , open in close .
-Program naj na standardni izhod (stdout ) izpiše vsebino datotek podanih preko argumentov ukazne vrstice.
-Če je program klican brez argumentov, kopira stdin v stdout
-Lahko je podanih tudi več vhodnih argumentov (datotek). 

 */

#define BUFFSIZE 512

void main(int argc, char *argv[])
{
    int fd;
    ssize_t bytes_read, bytes_written;
    char buf[BUFFSIZE];

    if (argc > 1)
    {
        // Read files.
        for (int i = 1; i < argc; i++)
        {
            // Open source file
            if ((fd = open(argv[i], O_RDONLY)) == -1)
            {
                perror("Izvorna datoteka (argument 1)");
                exit(1);
            }

            while((bytes_read = read(fd, buf, BUFFSIZE)) != 0)
            {
                // Read data
                if(bytes_read < 0)
                {
                    perror("read");
                    exit(1);
                }
                // Write data
                // printf(buf);
                if ((bytes_written = write(STDOUT_FILENO, buf, bytes_read)) != bytes_read)
                {
                    perror("write");
                    exit(1);
                }
            }

        

            close(fd);
        }
        
        
    }
    else
    {
        // Read user input.
        while ( (bytes_read = read(STDIN_FILENO, buf, BUFFSIZE) ) > 0) {
            if ( write(STDOUT_FILENO, buf, bytes_read) != bytes_read ) {
                perror("write");
                exit(1);
            }

        }
    }

}