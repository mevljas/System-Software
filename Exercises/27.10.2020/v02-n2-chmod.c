#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<unistd.h>


int main(int argc, char* argv[]){

    if(argc!=3){
        fprintf(stderr,"Wrong number of arguments.\n");
        exit(1);
    }
    //preverim ce datoteka obstaja, sicer nepotrebno saj chmod vrne -1 ce spreminjamo pravice 
    //datoteki ki ne obstaja
    if( access( argv[2], F_OK ) == -1 ) {
        fprintf(stderr,"File don't exist.\n");
        exit(1);
    } 
    char *permission = argv[1];
    struct stat buf;
    if(lstat(argv[2], &buf)<0){
        perror(argv[2]);
        exit(1);
    }

    //tričrkovni nizi
    if(strcmp(permission,"u+s") == 0){
         if(chmod(argv[2],(buf.st_mode | S_ISUID)) != 0){
            perror(argv[2]);
            exit(1);
         }
    }
    else if(strcmp(permission,"u-s") == 0){
        if(chmod(argv[2],(buf.st_mode & ~S_ISUID)) != 0){
            perror(argv[2]);
            exit(1);
         }
    
    }
    else if(strcmp(permission,"g+s") == 0){
        if(chmod(argv[2],(buf.st_mode | S_ISGID)) != 0){
            perror(argv[2]);
            exit(1);
         }
    }
    else if(strcmp(permission,"g-s") == 0){
        if(chmod(argv[2],(buf.st_mode & ~S_ISGID)) != 0){
            perror(argv[2]);
            exit(1);
         }
    }
    
    //osmisko stevilo
    else{
        char*ep; //kazalec na prvi znak, ki ni število
        /* Convert a string to a long integer.  */
        /*
            str: C-string beginning with the representation of an integral number.
            endptr: Reference to an object of type char*, whose value is set by the function to the next character in str after the numerical value.
                This parameter can also be a null pointer, in which case it is not used.
            base: Numerical base (radix) that determines the valid characters and their interpretation.
                If this is 0, the base used is determined by the format in the sequence (see above).
        */
        long int mode = strtol(permission, &ep, 8);
         if(chmod(argv[2],mode) != 0){
            perror(argv[2]);
            exit(1);
         }
    }
    
    if(lstat(argv[2], &buf)<0){
            printf(" lstat error\n");
            exit(1);
    }

    printf("New permissons: ");
    //preverim za vse datoteke (lahko je vec vrst)
    if(S_ISREG(buf.st_mode)) 
        printf("-"); // obicajna datoteka
    else if(S_ISDIR(buf.st_mode)) 
        printf("d"); // imenik
    else if(S_ISCHR(buf.st_mode)) 
        printf("c"); // znakovna posebna datoteka
    else if(S_ISBLK(buf.st_mode)) 
        printf("b"); // blocna posebna datoteka
    else if(S_ISFIFO(buf.st_mode)) 
        printf("p"); // cev
    else if(S_ISLNK(buf.st_mode)) 
        printf("l"); // simbolicna povezava
    else if(S_ISSOCK(buf.st_mode)) 
        printf("s"); // vticnica
    
    // User read permission
    (buf.st_mode & S_IRUSR)?printf("r"):printf("-");
    // User write permission
    (buf.st_mode & S_IWUSR)?printf("w"):printf("-");
    
    //mali s pomeni s+x nastavljeno
    //veliki S pomeni samo s ne pa tudi x, s nastavljen
    if((buf.st_mode & S_ISUID)){
        (buf.st_mode & S_IXUSR)?printf("s"):printf("S");
    }
    else{
        (buf.st_mode & S_IXUSR)?printf("x"):printf("-");
    }
    
    (buf.st_mode & S_IRGRP)?printf("r"):printf("-");
    (buf.st_mode & S_IWGRP)?printf("w"):printf("-");

    //mali s pomeni s+x nastavljeno
    //veliki S pomeni samo s ne pa tudi x, s nastavljen
    if((buf.st_mode & S_ISGID)){
        (buf.st_mode & S_IXGRP)?printf("s"):printf("S");
    }
    else{
        (buf.st_mode & S_IXGRP)?printf("x"):printf("-");
    }

    //mali t pomeni s+x nastavljeno
    //veliki T pomeni samo s ne pa tudi x, s nastavljen
    (buf.st_mode & S_IROTH)?printf("r"):printf("-");
    (buf.st_mode & S_IWOTH)?printf("w"):printf("-");
    if((buf.st_mode & __S_ISVTX)){ 
        (buf.st_mode & S_IXOTH)?printf("t"):printf("T");
    }
    else{
        (buf.st_mode & S_IXOTH)?printf("x"):printf("-");
    }

    printf("\n");
}