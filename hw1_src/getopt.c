#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int opt;
    int flag_a = 0, flag_b = 0, flag_c = 0, flag_d = 0;

    while((opt = getopt(argc, argv, "abcd")) != -1) {
        switch (opt) {
            case 'a': flag_a = 1; break;
            case 'b': flag_b = 1; break;
            case 'c': flag_c = 1; break;
            case 'd': flag_d = 1; break;
            case '?': printf("Unknown flag : %c \n", optopt);  
        }
    }
    
    if (flag_a)
        printf("Flag a is enable\n");
    if (flag_b)
        printf("Flag b is enable\n");
    if (flag_c)
        printf("Flag c is enable\n");
    if (flag_d)
        printf("Flag d is enable\n");

    return 0;
}