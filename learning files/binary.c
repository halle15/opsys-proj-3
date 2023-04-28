#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE 256
#define FRAME_SIZE 256

#define NUMBER_OF_FRAMES 256
#define PAGE_TABLE 256
#define TLB_SIZE 16

int main()
{
    unsigned int logical_address;
    unsigned int page_number;
    unsigned int offset;

    // Open the input file with the logical addresses
    FILE *input_file = fopen("addresses.txt", "r");
    if (input_file == NULL)
    {
        printf("Error: Could not open addresses.txt\n");
        exit(1);
    }

    // Open the backing store
    FILE *backing_store = fopen("BACKING_STORE.bin", "rb");
    if (backing_store == NULL)
    {
        printf("Error: Could not open BACKING_STORE.bin\n");
        exit(1);
    }

    // Read the logical addresses from the input file
    while (fscanf(input_file, "%u", &logical_address) != EOF)
    {
    page_number = (logical_address & 0xFFFF) >> 8;
    offset = logical_address & 0xFF;

    // Calculate the physical address
    unsigned int physical_address = (page_number * FRAME_SIZE) + offset;

    // Read the value at the physical address from the backing store
    char value;
    fseek(backing_store, physical_address, SEEK_SET);
    fread(&value, sizeof(char), 1, backing_store);

    printf("Logical address: %u, Physical address: %u, Value: %d\n", logical_address, physical_address, value);

    // Close the backing store file
    }

    // Close the files
    fclose(input_file);
    fclose(backing_store);

    return 0;
}
