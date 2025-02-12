#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

int main( int argc, char *argv[])
{
    DIR *dp;
    struct dirent*dirp;
    if ( argc != 2 ) {
        printf("Directory name required!");
        exit(1);
    }
    if ( (dp = opendir(argv[1])) == NULL ) {
        printf("Can't open %s", argv[1]);
        exit(1);
    }
    while ( ( dirp = readdir(dp)) != NULL)
        printf("%s\n", dirp->d_name);
        closedir(dp);
    return 0;
}