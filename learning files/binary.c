#include <stdio.h>

// compile command: gcc -o binary -O3 binary.c

int info_extract(int num){
    return (num >> 16) & 0xFFFF;
}

int page_extract(int num){
    return (num >> 8) & 0xFF;
}

int offset_extract(int num){
    return num & 0xFF;
}

// learning function, bunch of printers
int bit_mask()
{
    int number = 0xFF00;
    int mask;

    // AND operation (masking only the lower 8 bits)
    mask = 0x000000FF; // 32-bit mask: 0000 0000 0000 0000 0000 0000 1111 1111
    int and_result = (number >> 8) & mask;

    // OR operation (setting the upper 8 bits to 1)
    mask = 0xFF000000; // 32-bit mask: 1111 1111 0000 0000 0000 0000 0000 0000
    int or_result = number | mask;

    // XOR operation (inverting the middle 16 bits)
    mask = 0x00FFFF00; // 32-bit mask: 0000 0000 1111 1111 1111 1111 0000 0000
    int xor_result = number ^ mask;

    printf("Original number: %d = 0x%08X\n", number, number);
    printf("AND result: %d = 0x%08X\n", and_result, and_result);
    printf("OR result: %d = 0x%08X\n", or_result, or_result);
    printf("XOR result: %d = 0x%08X\n", xor_result, xor_result);
}

void print_binary(unsigned int num)
{
    for (int i = 31; i >= 0; i--)
    {
        printf("%d", (num >> i) & 1);
    }
}

/* First, write a simple program that extracts the page number and offset based on: | 31-16 INFORMATION | 15-8 PAGE NUMBER | 7-0 OFFSET |
 */

int main(int argc, char const *argv[])
{
    int s = 0xF0F0FFAF;
    // printf("size: %d\n", sizeof(s));
    // bit_mask();
    /*
    for (int i = 0; i < 1000; i++)
    {
        print_binary(i);
        printf(" - %d\n", i);
        usleep(100000);
    }
    */
    printf("Num: %d\n===================\n", s);
    printf("Offset: %d\n", offset_extract(s));
    printf("Page Number: %d\n", page_extract(s));
    printf("Info: %d\n", info_extract(s));
    return 0;
}
