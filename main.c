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

/*
    tlb entry struct

    struct tlb_entry
    {
        int page_number;
        int frame_number;
        int valid;
    }

    typedef struct tlb_entry tlb_entry_t;

    signed char physical_memory[NUMBER_OF_FRAMES][FRAME_SIZE]
    page table array of ints
    logical address
    physical address

*/

// compile: gcc -o proj3 main.c

// zoom info: third from last week, starts at 15:15

// objective: translate a given virtual address from addresses.txt to the appropriate physical address.

/*
    backing store represents virtual memory
    addresses.txt are the addresses that we read in from stdin
    correct.txt represents the correct output that the program should output.

    tlb is to quickly look up and find pages recently loaded with less overhead than page table
    page table is to

*/

/*  general project flow - GOAL: IMPLEMENT A VIRTUAL MEMORY MANAGER

    Take an integer from addresses.txt
    Read as input, extract the page number and offset from that address.
    Run the input through a tlb (array)
    IF TLB MISS:
        Look up frame number in TLB
    IF TLB HIT: 
        Use frame number from page table
    IF PAGE FAULT (PAGE TABLE EMPTY, PAGE NUMBER NOT FOUND):
        Go to physical memory (array), read in the page. (multiply page number to the spot in the binary backing store?)
        (read from file, load page into physical memory, update the page table, update the TLB, read in the next address, repeat.)
    CONVERT LOGICAL TO PHYSICAL ADDRESS, OUTPUT VALUE STORED AT ADDRESS, SAVE AND REPORT STATISTICS.
*/

/* general work process
    1. define our constants, page size, frame size, number of frames, etc.
    2. start the address breaking down process (get info, page num, offset.) MAKE SURE OFFSET_MASK AND PAGE_NUMBER_MASK ARE IMPLEMENTED AS CONSTANTS!
    3. start the process of file IO with address.txt
    4. store logical addresses in a char buffer[10].
    5. combine the breaking down with the fileIO of address.txt

    6. implement page table structure/array, as well as helper functions
    7. 

    X. start process for reading BACKING_STORE.bin

    R. Read from backing store (binary) and addresses.txt, store logical addresses in char buffer[10].

    Y. create a structure to store TLB entry.
    Z. create helper functions for checking, updating and flushing the tlb.

    D. implement main function, translate each logical address to corresponding physical address, extract byte value. Program should check TLB, if TLB hit, goto frame number and offset from physical memory and read, if TLB miss, go to page table, get frame number, read from physical memory. 

    F. implement command line arguments, final error checking, final quality check
*/

/* data structures/functions(most recommended by zoom at ~ 36 min)
    tlb: array of structs, 15 elements, signed char?
    backing store: backing store.bin
    physical memory: signed char p_mem[NUMBER_OF_FRAMES][FRAME_SIZE]
    page table: int array[PAGE_TABLE]
    logical address: ?
    physical address: ?
    statistics info

    function prototypes (zoom ~46:30)

    init_page_table() // set all elements in array to negative 1
    get_next_free_frame() //
    flush_tlb() // all elements in the struct to negative 1
    check_tlb(int); // 
        logic from diagram:
            look through TLB to see if page number exists in the array, 
                if you find page number, increment tlb hit for metrics, set frame number to what we find in TLB
                if not, return -1 (TLB miss), look up in page table instead.
    update_tlb(int, int); // set the element in the structure, happens when you load something from virtual mem into physical memory.
    check_page_table(int); // main helper func:

        don't loop, just check by corresponding array index (page 0 is index 0, etc.)

        if -1, page fault, increment local variable?, look into backing store.
            backing store guide
            use fseek function, page number * page size
        else
            return page table's frame number.
    
    main takes in three arguments passed in, open stores, init page table, flush TLB, while loop that reads in all addresses (fgets).
        Read through the input file and translate each logical address
            to physical address and extract byte value
            represented as a signed char at the physical address

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
