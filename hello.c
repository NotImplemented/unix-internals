#include <stdio.h>

int main()
{ 
    int var;	
    printf("Hello, world!\n");
    printf("Value = %d\n", var);

    int * ptr = 0;
    (*ptr) = var;
 

    return 0;
}

