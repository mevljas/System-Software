// Funkcije open, close; kostante, ki jih uporaibmo pri open(): O_RDONLY, O_WRONLY, O_RDWR; O_APPEND, O_CREAT, O_ECL, O_TRUNC, O_SYNC
#include <fcntl.h>
// veliko unix sistemskih klicev: read, write, lčseek, fork, chdir, chown, ...
#include <unistd.h>
// Spremeljivke stdin, stdout, stderr; funkcije: gets, getc, scanf, perror, printf, fprintf
#include <stdio.h>
#include <errno.h>
// funkcije: exit, malloc, atoi, rand, ...
#include <stdlib.h>

/* • Naloga 1
Napišite program v C ju, ki kopira eno datoteko v drugo. Imeni obeh datotek sta podani kot argumenta ukazne vrstice. Kopirajte datoteko po en blok (512 bajtov) naenkrat
Če ponorna (ciljna) datoteka ne obstaja, naj jo program ustvari. Če obstaja, naj jo prepiše.
Pred kopiranjem preverite
	• ali ima program 2 argumenta,
	• ali je prva datoteka berljiva (jo lahko odpremo za branje?) in
	• ali je druga datoteka zapisljiva (jo lahko opremo za pisanje?),
	• sicer izpišite ustrezno sporočilo in zaključite program.
 */

#define BUFSIZE 512

void main(int argc, char *argv[])
{
    int fd1, fd2;
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
        if ((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0777)) == -1)
        {
            perror("Ciljna datoteka (argument 2)");
            exit(1);
        }

        // Read data
        if ((bytes_read = read(fd1, buf, BUFSIZE)) < 0)
        {
            perror("read");
            exit(1);
        }
            

        // Write data
        if ((bytes_written = write(fd2, buf, bytes_read)) != bytes_read)
        {
            perror("write");
            exit(1);
        }
           

        close(fd1);
        close(fd2);
    }
    else
    {
        // We can't use perror, because it's not a system call error.
        printf("ERROR: Wrong number of arguments.\n");
    }
}