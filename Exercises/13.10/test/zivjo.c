// Header files are libraries

// ctrl + shit + b = build

#include <stdio.h>

// argc is number of arguments
// argv 
int main(int argc, char* argv[]) {
    for (size_t i = 0; i < argc; i++)
    {
        // %d = integer
        // %s = string pointer
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    printf("short: %lu\n", sizeof(short));
    printf("int: %lu\n", sizeof(int));
    printf("long: %lu\n", sizeof(long));
    printf("long long: %lu\n", sizeof(long long));
    printf("double: %lu\n", sizeof(double));
    printf("long double: %lu\n", sizeof(long double));
    


    // printf("Zivjo!\n");
    // If everything is alright, return 0.
    return 0;
}