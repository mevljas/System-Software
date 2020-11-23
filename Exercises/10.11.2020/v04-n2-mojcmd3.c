/*
Naloga 2

    Svojo ukazno vrstico sedaj razširite tako, da bo znala izvajati ukaze, ki so
    med sabo „povezani“ s cevjo.
    ▪ Namesto znaka |, bomo za cev uporabili niz ->

    ▪ Primer:
    ps -ef -> grep root

    Izvede naj se program ps z argumentom –ef in vse, kar izpiše na
    standardni izhod, se preusmeri na pisalni konec cevi. Bralni
    konec cevi pa se preveže na standardni vhod, iz katerega bere
    program grep.

    ▪ Omejite se na eno cev, torej: <ukaz1> -> <ukaz2>
    ▪ Cev obstaja med roditeljem in otrokom; roditelj naj izvede desno stran
    (<ukaz2>), otrok pa levo stran (<ukaz1>). Namig: proces „roditelj“ je
    otrok procesa, ki bere uporabnikove nize iz std. vhoda (glej vajo 3).
    ▪ Uporabite dup2, da preusmerite std. vhod in izhod. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define MAX_ARG 64
#define MAX_LINE_LEN 1024

#define err_sys(mess) { fprintf(stderr,"Error: %s.", mess); exit(1); }

// globalni counter pritiskov CTRL+C
int ctrl_c_counter = 0;

// funkcija izpise prompt v sinji barvi in izprazne buffer za izpis
void print_n_flush(){
    printf("\033[1;36mstudent@FRI-PC\033[0m:€ ");
    fflush(stdout);
}

// funkcija se poklice, ko ujamemo signal SIGINT (CTRL+C)
void sig_int(int signo){
    if(ctrl_c_counter == 0){ // ce prvic pritisnem CTRL+C
        printf(" Za izhod iz programa se enkrat pritisnite CTRL+C v roku 3 sekund.\n");
    }
    else if(ctrl_c_counter == 1){ // ce drugic pritisnem CTRL+C v roku 3 sekund
        printf(" Nasvidenje.\n");
        exit(4);
    }
    ctrl_c_counter++;
    alarm(3); // po preteku 3 sekund se poklice spodnja funkcija sig_alrm
}

// funkcija se poklice, ko ujamemo signal SIGALRM (ko potecejo 3 sekunde)
void sig_alrm(int signo){
    ctrl_c_counter = 0;
    print_n_flush();
}

// main()
int main(int argc, char* argv[]){
    char line[MAX_LINE_LEN+1] = {'\0'};
    pid_t otrok_pid, vnuk_pid;
    int status, read_bytes;

    // ujamem SIGINT -> poklicem sig_int()
    if( signal(SIGINT, sig_int) == SIG_ERR ) err_sys("signal() error");
    
    // ujamem SIGALRM -> poklicem sig_alrm()
    if( signal(SIGALRM, sig_alrm) == SIG_ERR ) err_sys("signal() error");

    print_n_flush();

    while( read_bytes = read(STDIN_FILENO, line, MAX_LINE_LEN) ){ // branje ukazne vrstice

        // ce ni bil vnesen noben znak zacnem znova
        if( line[0] == '\n' ){ print_n_flush(); continue; }

        // zamenjam '\n' z '\0'
        line[read_bytes-1] = '\0';

        // kazalci na argumente + stevec prebranih besed
        char *args[MAX_ARG] = {0};
        int counter = 0;
        int pipe_flag = 0; // zastavica ce je v ukazu prisoten znak '->'
        int counter_pipe; // stevec za pozicijo ukaza po znaku '->'
        
        // prebrano vrstico razclenim na posamezne besede po znaku ' '
        char* token = strtok(line, " ");
        while(token != NULL){
            args[counter++] = token;
            if(token[0] == '-' && token[1] == '>'){
                pipe_flag = 1; // zastavica cevi je true
                args[counter-1] = NULL; // kazalec na '->' postane NULL
                counter_pipe = counter;
            }
            token = strtok(NULL, " ");
        }
        args[counter] = NULL; // zadnji argument je NULL

        // ustvarim nov proces - otroka procesa stars
        if( (otrok_pid = fork()) == -1 ) err_sys("fork() error"); // ce fork() ni uspesen

        if(otrok_pid == 0){ // delo otroka
            if(pipe_flag == 1){ // je cev v ukazu
                int fd[2];

                if(pipe(fd) < 0) err_sys("pipe() error"); // ce pipe() ni uspesen

                // ustvarim nov proces - vnuka procesa stars, otroka procesa otrok
                if( (vnuk_pid = fork()) == -1 ) err_sys("fork() error"); // ce fork() ni uspesen
                
                if(vnuk_pid == 0){ // delo vnuka - izvede levi del ukaza
                    close(fd[0]); // zaprem bralni konec fd[]
                    dup2(fd[1], STDOUT_FILENO); // "preusmerim" pisalni konec fd[] na STDOUT
                    close(fd[1]); // zaprem pisalni konec fd[]

                    execvp(args[0], args);
                }
                else{ // delo otroka - izvede desni del ukaza
                    close(fd[1]); // zaprem pisalni konec fd[]
                    dup2(fd[0], STDIN_FILENO); // "preusmerim" bralni konec fd[] na STDIN
                    close(fd[0]); // zaprem bralni konec fd[]

                    if( waitpid(vnuk_pid, NULL, 0) < 0 ) err_sys("waitpid() error"); // proces otrok pocaka na proces vnuk
                    execvp(args[counter_pipe], args + (counter_pipe) ); // izvede se prvi ukaz po znaku '->' z vsemi argumenti
                }
            }
            else execvp(args[0], args); // ni cevi v ukazu

            err_sys(args[0]);
            return 2;
        }
        else{ // delo starsa
            if( wait(&status) == -1 ) err_sys("wait() error");
        }
        print_n_flush();        
    }

    if(read_bytes < 0) err_sys("read"); // ce pride do napake pri sistemskem klicu read()

    printf("\n");
    return 0;
}