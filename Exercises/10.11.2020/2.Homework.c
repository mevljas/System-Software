#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#define MAXLINE 1024
#define ARGV_N 64
#define PIPES_N 2

void pr_exit(int status);

void signal_handler(int sig_number)
{

    switch (sig_number)
    {
    case SIGINT:
        printf("\nza izhod iz programa se enkrat pritisnite CTRL+C v roku 3 sekund.\n");
        int time_left = alarm(3);
        if (time_left > 0)
        {
            printf("Nasvidenje!\n");
            exit(0);
        }
        printf(">>> ");
        fflush(stdout); // flush buffer - force print
        break;

    case SIGALRM:
        break;

    default:
        break;
    }
}

void main()
{
    char buf[MAXLINE];
    char *args[ARGV_N];
    char *args2[ARGV_N];
    char *pipes[PIPES_N];
    pid_t pid, pid2;
    int status, i, j, n_of_pipes, fd[2];

    // Set signal handler
    signal(SIGINT, signal_handler);
    signal(SIGALRM, signal_handler);

    // Print prompt
    printf(">>> ");

    // Reads while read != NULL
    while (fgets(buf, MAXLINE, stdin))
    {

        // Change newline smybol to null
        buf[strlen(buf) - 1] = 0;

        // Check if user entered anything
        if (strlen(buf) > 0)
        {
            i = 0;

            // Check if we have multiple commands
            if (strstr(buf, "->") != NULL)
            {
                n_of_pipes = 1;
            }
            else
            {
                n_of_pipes = 0;
            }

            // printf("Number of pipes %d\n", n_of_pipes);

            if (n_of_pipes == 1)
            {
                // Split and save the first word
                args[i] = strtok(buf, " ");

                // While there are other words, split and save
                while (args[i] != NULL)
                {
                    args[++i] = strtok(NULL, " ");
                    if (strcmp(args[i], "->") == 0)
                    {

                        // Save null at the end of the string
                        args[i] = 0;

                        j = 0;

                        args2[j] = strtok(NULL, " ");

                        // While there are other words, split and save
                        while (args2[j] != NULL)
                        {
                            args2[++j] = strtok(NULL, " ");
                        }

                        // Save null at the end of the string
                        args2[j + 1] = 0;
                    }
                }

                // i = 0;
                // // Output arguments
                // while(args[i]!=NULL)
                // {
                //     printf("%s\n", args[i++]);
                // }

                // printf("--------------\n");

                // j = 0;
                // // Output arguments
                // while(args2[j]!=NULL)
                // {
                //     printf("%s\n", args2[j++]);
                // }

                // Create a child
                pid = fork();

                if (pid < 0)
                {
                    // Error
                    perror("fork");
                    exit(1);
                }
                else if (pid == 0)
                {
                    // child

                    // printf("jaz sem otrok: pid = %d, ppid %d\n", getpid(), getppid());

                    if (pipe(fd) < 0)
                    {
                        perror("pipe");
                        exit(1);
                    }

                    // Create a child
                    pid2 = fork();

                    if (pid2 < 0)
                    {
                        // Error
                        perror("fork");
                        exit(1);
                    }
                    else if (pid2 == 0)
                    {

                        // printf("jaz sem pra otrok: pid = %d, ppid %d\n", getpid(), getppid());

                        // grandchild
                        close(fd[0]); // Zapre bralni konec
                        dup2(fd[1], 1);
                        close(fd[1]); // Zapre pisalni konec

                        // Run command
                        execvp(args[0], args);

                        // If this part of the code gets accessed, there must be an error.
                        perror("exec");
                        exit(1);
                    }
                    else
                    {
                        // child

                        if ((pid2 = waitpid(pid2, &status, 0)) < 0)
                        {
                            perror("waitpid");
                        }

                        // printf("jaz sem otrok konec: pid = %d, ppid %d\n", getpid(), getppid());

                        close(fd[1]); // Zapre pisalni konec
                        dup2(fd[0], 0);
                        close(fd[0]); // Zapre bralni konec
                        execvp(args2[0], args2);
                        perror("exec");
                        exit(1);
                    }
                }
                else
                {
                    // parent

                    if ((pid = waitpid(pid, &status, 0)) < 0)
                    {
                        perror("waitpid");
                    }

                    // printf("jaz sem stars konec: pid = %d, ppid %d\n", getpid(), getppid());
                }
            }
            else
            {
                // Split and save the first word
                args[i] = strtok(buf, " ");

                // Create a child
                pid = vfork();
                if (pid < 0)
                {
                    // Error
                    perror("fork");
                    exit(1);
                }
                else if (pid == 0)
                {
                    // child

                    // While there are other words, split and save
                    while (args[i] != NULL)
                    {
                        args[++i] = strtok(NULL, " ");
                    }

                    // Save null at the end of the string
                    args[i + 1] = 0;

                    // Run command
                    execvp(args[0], args);

                    // If this part of the code gets accessed, there must be an error.
                    perror("exec");
                    exit(1);
                }
                else
                {
                    // parent

                    if ((pid = waitpid(pid, &status, 0)) < 0)
                    {
                        perror("waitpid");
                    }
                }
            }
        }

        // Print prompt
        printf(">>> ");
    }

    exit(0);
}
