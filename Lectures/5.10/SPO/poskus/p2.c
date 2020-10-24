#include <stdio.h>

void main()
{
    // Variable
    int a = 10;
    
    // Pointer
    // Pointer to integer	
    // P points to variable a
    // & means address
	int *p = &a;


    printf("a: %d, p: %x \n", a, p);

    a = 20;
    printf("a: %d, p: %x \n", a, p);

    *p = 30;
    printf("a: %d, p: %x \n", a, p);
	
}
