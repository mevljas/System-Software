#include <stdio.h>

void main()
{
    // Variable
    int a = 10;
    
    // Pointer
    // Pointer to integer
    int *p;
	
    // P points to variable a
    // & means address
	p = &a;


    printf("a: %d, p: %x \n", a, p);
	
}
