#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

//  Ce bo datoteka prevelika, bo prebral samo 10000 znakov.
#define BUFSIZE 10000


#define err(mess) { fprintf( stderr, "napaka: %s\n", mess); exit(1); }


void delete_line( int fd, int vrstica);


void main( int argc, char *argv[])
{
    int fd;
    int n;
    char buf[BUFSIZE];

    if ( argc == 3) // append line
    {
        // odpre argumet 1. Ce ne obstaja, ga bo naredu. Bo appendu. Dodam rw za vse.
        fd = open( argv[1], O_CREAT | O_WRONLY | O_APPEND, 0666);
        if ( fd < 0)
        {
            err("open")
        }

        // Preberem ceu file v buffer - ni narejeno za velike datoteke
        n = read(STDIN_FILENO, buf, BUFSIZE);

        if ( n < 0)
        {
            err("read")
        }

        if ( write( fd, buf, n) != n)
        {
            err("read")
        }
        
        
    }
    else if ( argc == 4) // delete line
    {
        fd = open( argv[1], O_RDWR, 0666);
        if ( fd < 0)
        {
            err("open")
        }


        // Funkciji podam vrstico za izbris
        delete_line( fd, atoi(argv[3]));
        
    }
    else
    {
        err("Uporaba:\n\tedi <datoteka> a\n\tedi <datoteka> d <stevilka:vrstice.\n");
    }
    close(fd);
    


}


void delete_line( int fd, int vrstica) {
    char buf[BUFSIZE];
    int n;
    int i, j;
    int lines;
    int zacetek = 0, konec, dolzina;

    n = read( fd, buf, BUFSIZE);
    if ( n < 0) 
    {
        err("read")
    }

    i = 0; // stevec znakov
    lines = 0;
    do
    {
        if ( buf[i] == '\n')
        {
            lines++;
            if ( lines == vrstica - 1)
            {
                zacetek = i + 1;
            }
            
        }
        i++;
        // Loopamo, dokler ne najdemo vrstice.
    } while ( lines < vrstica);
    konec = i - 1;
    // Zapomnimo si azcectek in koenc nase vrstice in zracunamo dolzino.
    dolzina = konec - zacetek + 1;
    printf("zacetek: %d, konec: %d, dolzina %d, n: %d\n", zacetek, konec, dolzina, n);

    // gremo od konca +1 do konca datoeke.
    for (j = konec + 1; j <= n - 1; j++)
    {
        buf[j - dolzina] = buf[j]; //za vecje datoteke bolje memcpy
    }

    // grem na zacetek fajla
    lseek( fd, 0, SEEK_SET);

    // zapisem celoten buffer
    if (write( fd, buf, n - dolzina) != n - dolzina)
    {
        err("write")
    }

    // Odrezem od tam naprej.
    ftruncate( fd, n - dolzina);
    
    



    

}