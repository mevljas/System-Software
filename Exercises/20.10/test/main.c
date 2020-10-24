#include <stdio.h> // /usr/include

int main(int argc, char* argv[]){
    printf("argv: %p\n", argv);
    //printf("argv: %x\n", argv);
    printf("argv: %p\n", argv[0]);
    printf("argv: %p\n", argv[1]);
    printf("argv: %p\n", argv[2]);


    
    return 0;
}