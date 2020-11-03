#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <utime.h>


void main( int argc, char *argv[])
{

    struct stat buf;
    struct utimbuf timebuf;

    stat("file1", &buf);
    open("file1", O_RDWR | O_TRUNC);

    timebuf.actime = buf.st_atime;
    timebuf.modtime = buf.st_mtime;

    utime("file1", &timebuf);


    // printf("", buf.atime, buf.mtime, buf.ctime)

}