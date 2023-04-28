#include <stdio.h>

#define PAGE_SIZE 256 // 2^8 
#define FRAME_SIZE 256 // 2^8

#define NUMBER_OF_FRAMES 256 // 2^8
#define PAGE_TABLE 256 // from book
#define TLB_SIZE 16 // 16 entries in the page table

#define BUFFER 10 // 

// ============================

#define PAGE_NUMBER_MASK 0x0000FF00 // TWO BITS BEFORE LAST TWO
#define OFFSET_MASK 0x000000FF // LAST TWO BITS

// ============================

int pageTable[PAGE_TABLE]; 

typedef struct tlb_entry
{
    int page_number;
    int frame_number;
    int valid;
} tlb_entry_t;

signed char physical_memory[NUMBER_OF_FRAMES][FRAME_SIZE];

tlb_entry_t tlb[TLB_SIZE];


void flush_tlb(){
    // set all elements in struct to negative 1
}

void init_page_table(){
    for(int i = 0; i < PAGE_TABLE; i++){
        pageTable[i] = -1;
    }

    printf("Page Table Initialized\n");
}

void print_binary(unsigned int num) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
    }
}

int main(int argc, char const *argv[])
{
    printf("Program Start\n");

    init_page_table();

    // flush tlb

    // begin while loop that reads in all addresses.

    /* while loop should:
        Translate logical address to physical address
        Extract Byte Value (signed char at physical address)
    */

    printf("Program Ended Successfully!\n");
    return 0;
}
