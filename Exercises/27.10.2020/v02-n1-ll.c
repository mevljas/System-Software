/*
Naloga 1

Napišite v C-ju poenostavljeno verzijo ukaza
ls -l, ki izpiše imena vseh datotek v trenutnem
imeniku ("."), kakor tudi tip datoteke (običajna,
imenik, vtičnica, bločna, znakovna, ...), dostopna
dovoljenja (mode) in število trdih povezav
(st_nlink).

    • Ne pozabite na S_ISUID, S_ISGID in S_ISVTX.
    • Uporaba "exec ls –l" ni dovoljena.
    • Primer izpisa:
        -rwsr-Sr-x 1 lsl
        -rw-r--r-- 1 lsl.c
        drwxr-xr-t 2 .
        drwxr-xr-x 6 ..
        -rw------T 1 stevila
*/

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char* argv[]){
    struct stat buffer;
    DIR* mapa;  // kazalec na mapo
    struct dirent* v_mapi;  //Trenutni file v direktoriju
    char* ime_mape = ".";
    // Ime imenika je lahko podano kot argument
    if(argc == 2) 
        ime_mape = argv[1];

    if((mapa = opendir(ime_mape)) == NULL){
        perror("opendir");
        exit(2);
    }

    while((v_mapi = readdir(mapa)) != NULL){
        // Lahko uporabimo tudi samo sistemski klic 'stat'.
        // lstat vrne inforamicje o povezavi, če je to povezava.
        if((lstat(v_mapi->d_name, &buffer)) != 0){
            perror("lstat");
            exit(1);
        }

        // niz za -rwxrwxrwx
        char mod[10+1];

        // Get file type
        if(S_ISREG(buffer.st_mode)) // obicajna datoteka
            mod[0] = '-'; 
        else if(S_ISDIR(buffer.st_mode)) // imenik
            mod[0] = 'd'; 
        else if(S_ISCHR(buffer.st_mode)) // znakovna posebna datoteka
            mod[0] = 'c'; 
        else if(S_ISBLK(buffer.st_mode)) // blocna posebna datoteka
            mod[0] = 'b'; 
        else if(S_ISFIFO(buffer.st_mode)) // cev
            mod[0] = 'p'; 
        else if(S_ISLNK(buffer.st_mode)) // simbolicna povezava
            mod[0] = 'l'; 
        else if(S_ISSOCK(buffer.st_mode)) // vticnica
            mod[0] = 's'; 

        // Get file permissions
        mod[1] = (S_IRUSR & buffer.st_mode)? 'r' : '-'; // user
        mod[2] = (S_IWUSR & buffer.st_mode)? 'w' : '-';
        mod[3] = (S_IXUSR & buffer.st_mode)? 'x' : '-';
        if((S_ISUID & buffer.st_mode)) mod[3] = 'S';
        if((S_IXUSR & buffer.st_mode) && (S_ISUID & buffer.st_mode)) mod[3] = 's';

        mod[4] = (S_IRGRP & buffer.st_mode)? 'r' : '-'; // group
        mod[5] = (S_IWGRP & buffer.st_mode)? 'w' : '-';
        mod[6] = (S_IXGRP & buffer.st_mode)? 'x' : '-';
        if((S_ISGID & buffer.st_mode)) mod[6] = 'S';
        if((S_IXGRP & buffer.st_mode) && (S_ISGID & buffer.st_mode)) mod[6] = 's';

        mod[7] = (S_IROTH & buffer.st_mode)? 'r' : '-'; // others
        mod[8] = (S_IWOTH & buffer.st_mode)? 'w' : '-';
        mod[9] = (S_IXOTH & buffer.st_mode)? 'x' : '-';
        if((S_ISVTX & buffer.st_mode)) mod[9] = 'T';
        if((S_IXOTH & buffer.st_mode) && (S_ISVTX & buffer.st_mode)) mod[9] = 't';
        

        // stevilo trdih povezav
        int counter = buffer.st_nlink;

        printf("%s  %d  %s\n", mod, counter, v_mapi->d_name);
    }

    return 0;
}