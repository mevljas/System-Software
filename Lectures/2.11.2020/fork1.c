#include <unistd.h>
#include <stdio.h>
#include <errno.h>




void main()
{
    pid_t pid;


    // Ustvarimo otroka
    pid = fork();
    if ( pid < 0)
    {
        perror("fork");
    }
    else if ( pid == 0)
    {
        // otrok
        printf("jaz sem otrok: pid = %d, ppid %d\n", getpid(), getppid());
    }
    else
    {
        // Stars
        printf("jaz sem stars: pid = %d, ppid %d\n", getpid(), getppid());
    }


    printf("jaz sem ?: pid = %d, ppid %d\n", getpid(), getppid());
    sleep(1);
    
    
    
}