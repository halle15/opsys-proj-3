#include <stdio.h>

// compile: gcc -o proj3 main.c

void print_binary(unsigned int num) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
    }
}

int main(int argc, char const *argv[])
{
    int s = 1;
    printf("size: %d", sizeof(s));
    for(int i = 0; i < 1000; i++){
        print_binary(i);
        printf("\n");
        usleep(100000);
    }
    return 0;
}
