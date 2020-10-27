// Funkcije open, close; kostante, ki jih uporaibmo pri open(): O_RDONLY, O_WRONLY, O_RDWR; O_APPEND, O_CREAT, O_ECL, O_TRUNC, O_SYNC
#include <fcntl.h>
// veliko unix sistemskih klicev: read, write, lčseek, fork, chdir, chown, ...
#include <unistd.h>
// Spremeljivke stdin, stdout, stderr; funkcije: gets, getc, scanf, perror, printf, fprintf
#include <stdio.h>
#include <errno.h>
// funkcije: exit, malloc, atoi, rand, ...
#include <stdlib.h>

/* Naloga 4
- Napišite C program rep , ki izpiše zadnjih n vrstic besedilne datoteke, katere ime je podano kot vhodni argument.
- Program naj deluje podobno kot tail
- Primer klica programa: rep n4 a .txt Privzeta vrednost za n naj bo 5.
 */

#define BUFSIZE 1


void main(int argc, char *argv[])
{
    int fd, offset, desired_number_of_rows, number_of_rows;
    ssize_t bytes_read, bytes_written;
    char buf[BUFSIZE];
    char filename[1];

if (argc >= 2)
    {

        // get number of rows
        // Open source file
        if (argc == 3){
            desired_number_of_rows = atoi(argv[1]);
            if ((fd = open(argv[2], O_RDONLY)) == -1)
            {
                perror("Izvorna datoteka (argument 1)");
                exit(1);
            }
        }
            
        else{
            desired_number_of_rows = 5;
            if ((fd = open(argv[1], O_RDONLY)) == -1)
            {
                perror("Izvorna datoteka (argument 1)");
                exit(1);
            }
        }
            


        number_of_rows = 0;
        

        // Jump to the end of file
        if ( (( offset = lseek( fd, -1, SEEK_END)) < 0))
            perror("lseek");



        // Count newline
        while ( (number_of_rows < desired_number_of_rows && offset >= 0 && (bytes_read = read(fd, buf, BUFSIZE)) == 1))
        {
           
            // Decrement file cursor
            offset = lseek( fd, offset - 1, SEEK_SET);

            // Count newline
            if (buf[0] == '\n')
                number_of_rows++;
        }

         // Increment file cursor
            offset = lseek( fd, offset + 2, SEEK_SET);


        // Read data
        while ( ( offset >= 0 && (bytes_read = read(fd, buf, BUFSIZE)) == 1))
        {
            // Write data
            if ((bytes_written = write(STDOUT_FILENO, buf, bytes_read)) != bytes_read)
            {
                perror("write");
                exit(1);
            }


            // Increment file cursor
            offset = lseek( fd, offset + 1, SEEK_SET);

        
        }
        
           

        close(fd);
    }
    else
    {
        // We can't use perror, because it's not a system call error.
        printf("ERROR: Wrong number of arguments(%d).\n",argc);
    }
}