#include <stdio.h>

// compile command: gcc -o binary -O3 binary.c

int bit_mask()
{
    int number = 0xA9876543; // 32-bit integer: 1010 1001 1000 0111 0110 0101 0100 0011
    int mask;

    // AND operation (masking only the lower 8 bits)
    mask = 0x000000FF; // 32-bit mask: 0000 0000 0000 0000 0000 0000 1111 1111
    int and_result = number & mask;

    // OR operation (setting the upper 8 bits to 1)
    mask = 0xFF000000; // 32-bit mask: 1111 1111 0000 0000 0000 0000 0000 0000
    int or_result = number | mask;

    // XOR operation (inverting the middle 16 bits)
    mask = 0x00FFFF00; // 32-bit mask: 0000 0000 1111 1111 1111 1111 0000 0000
    int xor_result = number ^ mask;

    printf("Original number: %d =  0x%08X\n", number, number);
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
    int s = 1;
    printf("size: %d\n", sizeof(s));
    bit_mask();
    /*
    for (int i = 0; i < 1000; i++)
    {
        print_binary(i);
        printf(" - %d\n", i);
        usleep(100000);
    }
    */
    return 0;
}
