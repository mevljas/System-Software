// Funkcije open, close; kostante, ki jih uporaibmo pri open(): O_RDONLY, O_WRONLY, O_RDWR; O_APPEND, O_CREAT, O_ECL, O_TRUNC, O_SYNC
#include <fcntl.h>
// veliko unix sistemskih klicev: read, write, lčseek, fork, chdir, chown, ...
#include <unistd.h>
// Spremeljivke stdin, stdout, stderr; funkcije: gets, getc, scanf, perror, printf, fprintf
#include <stdio.h>
#include <errno.h>
// funkcije: exit, malloc, atoi, rand, ...
#include <stdlib.h>

/* Naloga 3
Napišite program v C-ju, ki vsebino datoteke izpiše v obratnem
vrstnem redu v drugo datoteko.
Imeni obeh datotek sta podani kot argumenta programa.
Pri vsakem sistemskem klicu preverite, ali se lahko izvede,
sicer izpišite sporočilo o napaki in končajte program.
Program mora delovati za „poljubno“ dolge datoteke: ni
dovoljeno prebrati celotne datoteke v delovni spomin.
Uporabite lseek.
Test programa:
    ./naloga1 a.txt b.txt
    ./naloga1 b.txt c.txt
    diff a.txt c.txt
 */

#define BUFSIZE 1


void main(int argc, char *argv[])
{
    int fd1, fd2, offset;
    ssize_t bytes_read, bytes_written;
    char buf[BUFSIZE];

if (argc == 3)
    {
        // Open source file
        
        if ((fd1 = open(argv[1], O_RDONLY)) == -1)
        {
            perror("Izvorna datoteka (argument 1)");
            exit(1);
        }

        // open target file
        if ((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC)) == -1)
        {
            perror("Ciljna datoteka (argument 2)");
            exit(1);
        }

        // Jump to the end of file
        if ( (( offset = lseek( fd1, -1, SEEK_END)) < 0))
            perror("lseek");



        // Read data
        while ( (offset >= 0 && (bytes_read = read(fd1, buf, BUFSIZE)) == 1))
        {
            // Write data
            if ((bytes_written = write(fd2, buf, bytes_read)) != bytes_read)
            {
                perror("write");
                exit(1);
            }


            // Decrement file cursor
            offset = lseek( fd1, offset - 1, SEEK_SET);
        }
        
           

        close(fd1);
        close(fd2);
    }
    else
    {
        // We can't use perror, because it's not a system call error.
        printf("ERROR: Wrong number of arguments(%d).\n",argc);
    }
}