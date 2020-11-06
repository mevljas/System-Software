#include <stdio.h>

void main()
{

    char buf[100];

    // Ni potrebe preverjat, ce je razlicno od NULL, ker je pointer - ma isti pomen, ko te ze napisano
    while( fgets(buf, 100, stdin)){
        fputs( buf, stdout);
    }

}