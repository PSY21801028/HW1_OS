#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int i; 
    printf("Command Line Arguments!\n");
    printf("argc = %d\n", argc); 
    for (i = 0; i < argc; i++)
    {
        // Print arguments 
        // atoi: convert string to integer type value if the string is integer
        printf("argv[%d] = %s (%d) \n", i, argv[i], atoi(argv[i]));  
    }
    return 0;
}
