/*
Naloga 1

Napišite program, ki deluje kot preprosta ukazna vrstica in v neskončni
zanki izvaja vnesene ukaze.

    ▪ Program se zaključi ob pritisku CTRL+D.
    ▪ Celoten ukaz preberite s standardnega vhoda v znakovni niz. Lahko
        predpostavite, da ukazni niz ni daljši od 1024 znakov. Za branje lahko
        uporabite fgets() ali pa kar read().
    ▪ Za posamezen ukaz naj proces s klicem fork() ustvari otroka, ki s
        klicem execv() ali execvp() izvrši ukaz.
    ▪ Omogočite izvajanje ukazov z argumenti.

    • Ukaz razčlenite po presledkih: presledke zamenjajte z '\0', na začetke podnizov pa
        naj kažejo kazalci, ki jih boste podali kasneje funkciji execvp().
    • Namesto ročnega razčlenjevanja lahko uporabite tudi funkcijo strtok() iz knjižnice
        string.h.
    • Lahko predpostavite, da argumentov ne bo več kot 64.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define MAX_ARG 64
#define MAX_LINE_LEN 1024

void print_n_flush(){
    printf("\033[1;36mstudent@FRI-PC\033[0m:€ "); // print s sinjo barvo
    fflush(stdout); // izpraznim medpomnilnik za izpis - 
                    // standardni izhod ima privzeto linijsko medpomnenje, 
                    // torej se izprazni, ko je poln ali ko pride do \n.
}

int main(int argc, char* argv[]){
    // Medpomnilnik za vrstico standardnega vhoda
    char line[MAX_LINE_LEN+1] = {'\0'}; // Inicializacija na 0.
    // kazalci na argumente
    char *args[MAX_ARG] = {0};
    pid_t parent_pid, child_pid;
    int status, read_bytes, counter;

    print_n_flush();

    while( read_bytes = read(STDIN_FILENO, line, MAX_LINE_LEN) ){ // branje ukazne vrstice

        // ce ni bil vnesen noben znak se vrnem na zacetek zanke
        if( line[0] == '\n'){ print_n_flush(); continue; }

        line[read_bytes-1] = '\0'; // zamenjam zadnji znak, ki je '\n', z '\0'

        counter = 0; // inicializiram stevec argumentov (prebranih besed) na 0
        
        // prebrano vrstico razclenim na posamezne besede po znaku ' '
        char* token = strtok(line, " ");
        while(token != NULL){
            args[counter++] = token;
            token = strtok(NULL, " ");
        }
        args[counter] = NULL; // zadnji argument je NULL

        // ustvarim nov proces
        if( (child_pid = fork()) == -1 ) { perror("fork() error"); exit(1); } // ce fork ni uspesen

        if(child_pid == 0){ // delo otroka
            execvp(args[0], args);
            perror(args[0]);
            return 2;
        }
        else{ // delo starsa
            if( wait(&status) == -1 ) { perror("wait() error"); exit(3); }
        }
        print_n_flush();
    }
    // preverim ce pride do napake pri sistemskem klicu read()
    if (read_bytes < 0){
        perror("read");
        exit(1);
    }
    printf("\n");
    return 0;
}