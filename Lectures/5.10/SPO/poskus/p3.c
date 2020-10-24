#include <stdio.h>

void main()
{

    // Array
    // All array fields are saved together sequentially in the memory.
    int v[3] = {10, 20, 30};
    
    int *p = v;

    for (int i = 0; i < 3; i++){
        printf("*p: %d, p: %x \n", *p, p);
        p++;
    }


	
}
