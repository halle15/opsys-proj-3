#include <stdio.h>

#define PAGE_SIZE 256
#define FRAME_SIZE 256

#define NUMBER_OF_FRAMES 256
#define PAGE_TABLE 256
#define TLB_SIZE 16

// compile: gcc -o proj3 main.c

/* general flow
    1. define our constants, page size, frame size, number of frames, etc.
    2. start the address breaking down process (get info, page num, offset.) MAKE SURE OFFSET_MASK AND PAGE_NUMBER_MASK ARE IMPLEMENTED AS CONSTANTS!
    3. start the process of file IO with address.txt
    4. store logical addresses in a char buffer[10].
    5. combine the breaking down with the fileIO of address.txt

    6. implement page table structure/array, as well as helper functions
    7. 

    X. start process for reading BACKING_STORE.bin


    Y. create a structure to store TLB entry.
    Z. create helper functions for checking, updating and flushing the tlb.

    F. implement the main, command line arguments, final error checking, final quality check
*/

/* data structures/functions(most recommended by zoom at ~ 36 min)
    tlb: array with structs, signed char?
    backing store: backing store.bin
    physical memory: signed char p_mem[NUMBER_OF_FRAMES][FRAME_SIZE]
    page table: int array[PAGE_TABLE]
    logical address: ?
    physical address: ?
    statistics info

    function prototypes

    init_page_table()
    get_next_free_frame()
    flush_tlb()
    check_tlb(int);
    update_tlb(int, int);
    check_page_table(int);
*/

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
