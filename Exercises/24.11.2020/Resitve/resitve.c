/*
Preverjanje 1, 24. 11. 2020

Čas reševanja: 70 min.

Napiši program v jeziku C, ki naredi naslednje.

1. Odpre datoteko in v njej zamenja določene znake s presledki - več v nadaljevanju. Ime datoteke je podano kot argument ukazne vrstice. Če je število podanih argumentov napačno, izpišite napako in končajte program. 

Predpostavimo, da lahko vhodna datoteka vsebuje samo znake od '1' do '9', npr.:
25411384
Program naj začne pri prvem znaku in ga zamenja s presledkom. Nato naj preskoči toliko znakov, kolikor je bila števična vrednost zamenjanega znaka (2) in zopet naredi zamenjavo s presledkom. Postopek ponavlja, dokler ne pridemo do konca ali preko konca datoteke. V zgornjem primeru so zamenjave naslednje (števila v oklepaju so tista mesta, kjer je prišlo do zamenjave):
(2)54(1)1(3)84
Končno stanje datoteke je:
 54 1 84
Velikost datoteke (v bajtih) po zamenjavah mora biti enaka tisti na začetku.
Pozor: vhodna datoteka je lahko poljubne velikosti - lahko je zelo velika.

Namig: uporabite sistemski klic lseek.


2. Program datoteki spremeni dostopna dovoljenja tako, da skupini (group) in ostalim (other) odvzame pravico za pisanje (w).

3. Program ustvari nov proces (otroka), ki bo izvedel zunanji program za štetje besed (/usr/bin/wc). Primer za datoteko test.txt:
wc -w test.txt
Rezultat programa wc naj se zapiše na cev, glavni proces (roditelj) naj rezultat prebere in izpiše na standardni izhod v naslednji obliki (primer za datoteko test.txt):
"Stevilo besed v datoteki test.txt je 3."

Roditelj naj čaka, da otrok zaključi in izpiše status, ki ga je otrok vrnil:
"Proces otrok se je koncal in vrnil status 0."


V programu je prepovedano uporabiti funkcije system, fopen, fread, fwrite, fseek, fclose. Kjer se le da, uporabite sistemske klice. Obvezno preverjajte vrnjene vrednosti sistemskih klicev in ostalih funkcij ter izpišite obvestilo o morebitnih napakah na standardni izhod za napake. Poskrbite, da pri prevajanju programa ne pride do napak in opozoril (prevajajte s stikalom -Wall).

Če svoj program poganjate v WSL, naj bo vaš delovni imenik na particiji Linuxa, sicer spreminjanje dovoljenj ne bo delovalo (uporabite npr.: cd ~).

Program med delovanjem spreminja vhodno datoteko, zato vam svetujem, da si najprej naredite njeno kopijo in jo potem po potrebi kopirate v delovno verzijo, npr.:
cp test.txt test.bak
Restavracija začetega stanja:
cp test.bak test.txt

Program shranite v datoteko z imenom Priimek-Ime.c (npr. Krpan-Martin.c) in jo naložite preko nalagalnika datotek spodaj. V imenu datoteke ne uporabljajte šumnikov.

*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <wait.h>

#define err(msg) ({fprintf(stderr,msg); exit(1);})
#define err_sys(msg) ({perror(msg); exit(1);})


int main(int argc, char* argv[]){

    if(argc != 2){
        err("Napacno stevilo argumentov! Zahtevano je ime datoteke.\n");
    }

    int fd, status, fdpipe[2];
    off_t fileSize, offset, off2, jump=0;
    char buf, buffer[1024];
    pid_t pid;

    if( (fd=open(argv[1],O_RDWR,0777)) < 0 ){
        err_sys(argv[1]);
    }

    if ( (fileSize = lseek(fd,0,SEEK_END)) < 0){
        err_sys("lseek-end");
    }
    if ( (offset = lseek(fd,0,SEEK_SET)) < 0){
        err_sys("lseek-set");
    }

    while(offset < fileSize){
        if (read(fd,&buf,1) < 0){
            err_sys("read");
        }
        jump = buf - '0';
        buf = ' ';
        if ( (off2 = lseek(fd,-1,SEEK_CUR)) < 0){
            err_sys("lseek-2");
        }
        if (write(fd,&buf,1) != 1){
            err_sys("write");
        }

        if( (offset + jump) >= fileSize){
            break;
        }
        if ( (offset = lseek(fd,jump,SEEK_CUR)) < 0){
            err_sys("lseek-cur");
        }
    }

    if( close(fd) < 0 ){
        err_sys(argv[1]);
    }


    // 2. chmod
    struct stat statbuf;
    if ( stat(argv[1], &statbuf) < 0){
		err_sys(argv[1]);
	}
    if ( chmod(argv[1],statbuf.st_mode & ~ (S_IWGRP | S_IWOTH) ) < 0){
        err_sys("chmod");
    }

    // 3. fork, pipe, dup2
    if( pipe(fdpipe) < 0){
        err_sys("pipe");
    }
    if( (pid=fork()) < 0 ){
        err_sys("fork");
    }
    else if(pid == 0){
        // otrok
        if(close(fdpipe[0]) < 0){
            err_sys("pipe close child");
        }
        if(dup2(fdpipe[1],STDOUT_FILENO) < 0){
            err_sys("dup2 child");
        }
        if(close(fdpipe[1]) < 0){
            err_sys("pipe close child");
        }

        execlp("wc","wc","-w",argv[1], (char*)NULL);
        err_sys("exec");
    }
    else{
        // roditelj
        if(close(fdpipe[1]) < 0){
            err_sys("pipe close");
        }
        
        if ( wait(&status) < 0){
            err_sys("wait");
        }
        printf("Proces otrok se je koncal in vrnil status %d\n", WEXITSTATUS(status));

        if (read(fdpipe[0], buffer, 1024) < 0){
            err_sys("read stdin");
        }
        if(close(fdpipe[0]) < 0){
            err_sys("pipe close");
        }

        char* token;
        if ( !(token = strtok(buffer," "))){
           err("Error with strtok\n"); 
        }
        printf("Stevilo besed v datoteki %s: %s.\n", argv[1], token);

    }

    return 0;
}