#include <stdio.h>
#include <stdlib.h>

// compile command: gcc -o addresses -O3 addresses.c

#define MAX_NUMBERS 1000 // Maximum number of integers that can be read from file

unsigned int info_extract(uint32_t num){
    return (num >> 16) & 0xFFFF;
}

unsigned int page_extract(uint32_t num){
    return (num >> 8) & 0xFF;
}

unsigned int offset_extract(uint32_t num){
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

int main(int argc, char const *argv[]) {
    FILE *fp;
    int numbers[MAX_NUMBERS];
    int count = 0;
    char filename[] = "addresses.txt";

    fp = fopen(filename, "r"); // Open the file for reading
    if (fp == NULL) {
        printf("Failed to open file %s\n", filename);
        return 1;
    }

    // Read the numbers from the file and store them in the array
    while (fscanf(fp, "%d", &numbers[count]) != EOF && count < MAX_NUMBERS) {
        count++;
    }

    fclose(fp); // Close the file

    for(int a = 0; a < atoi(argv[1]); a++){
        printf("===================\nNum: %d\n===================\n", numbers[a]);
    printf("Offset: %d\n", offset_extract(numbers[a]));
    printf("Page Number: %d\n", page_extract(numbers[a]));
    printf("Info: %d\n", info_extract(numbers[a]));
    }
    printf("===================\nCOMPLETE\n===================\n");
    return 0;
}