#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char *argv[]) 
{
    struct stat buf;

    if (argc != 2) {
        printf("Uporaba: %s datoteka\n", argv[0]);
        exit(0);

    }
        

    if ( stat( argv[1], &buf) < 0)
        perror("stat");

//  Check file type
    if ( S_ISREG(buf.st_mode) )
        printf("Obicajna datoteka.\n");
    
    else if ( S_ISDIR(buf.st_mode) )
        printf("Direktorij.\n");

    else if ( S_ISCHR(buf.st_mode))
        printf("Znakovna posebna datoteka.\n");

    else if ( S_ISBLK(buf.st_mode))
        printf("Bločna posebna datoteka.\n");
    
    else
        printf("Nekaj drugega.\n");


    return 0;

}