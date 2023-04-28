#include <stdio.h>
#include <stdlib.h>

// compile: gcc -o proj3 main.c

#define PAGE_SIZE 256  // 2^8
#define FRAME_SIZE 256 // 2^8

#define NUMBER_OF_FRAMES 256 // 2^8
#define PAGE_TABLE 256       // from book
#define TLB_SIZE 16          // 16 entries in the page table

#define BUFFER 10 // stores logical addresses from addresses.txt?

// ============================

#define PAGE_NUMBER_MASK 0x0000FF00 // TWO BITS BEFORE LAST TWO
#define OFFSET_MASK 0x000000FF      // LAST TWO BITS

// ============================

int pageTable[PAGE_TABLE];

// 1 is valid, anything else is invalid.
typedef struct tlb_entry
{
    int page_number;
    int frame_number;
    int valid;

} tlb_entry_t;

signed char physical_memory[NUMBER_OF_FRAMES][FRAME_SIZE];

tlb_entry_t tlb[TLB_SIZE];

// ========METRICS=========

// ========================

// -1 result = page fault
int check_page_table(int page_num)
{
    int result = pageTable[page_num];

    if (result == -1)
    {
        printf("page fault\n");
        /* increment local stat variable for page fault
             look into backing store
                 use fseek func, pagenum * page size;
        */
    }
    else
    {
        printf("%d\n", result);
    }
    return result;
}

void update_page_table(int page_num, int frame_num)
{
    pageTable[page_num] = frame_num;
}

void flush_tlb()
{
    for (int i = 0; i < TLB_SIZE; i++)
    {
        tlb[i].valid = 0;
    }
}

void init_tlb()
{
    flush_tlb();
}

int search_tlb(int page_number)
{
    for (int i = 0; i < TLB_SIZE; i++)
    {
        if (tlb[i].valid && tlb[i].page_number == page_number)
        {
            return tlb[i].frame_number;
        }
    }
    // update tlb miss stats

    return -1; // tlb miss!
}

void update_tlb(int page_number, int frame_number)
{
    // tbd
}

void init_page_table()
{
    for (int i = 0; i < PAGE_TABLE; i++)
    {
        pageTable[i] = -1;
    }

    printf("Page Table Initialized\n");
}

/*
    You should read from the backing store and load the data into physical memory.
    To decide where in the physical memory the data should go, you will need to find an available frame.
    You can use a simple counter (like a variable called next_available_frame) that increments each time a frame is assigned. This method works when the physical memory size is the same as the virtual address space size, as you don't need to worry about page replacement.
*/
void search_backing_store(int page_number, int offset)
{
    // Open the backing store
    FILE *backing_store = fopen("BACKING_STORE.bin", "rb");
    if (backing_store == NULL)
    {
        printf("Error: Could not open BACKING_STORE.bin\n");
        exit(1);
    }

    char page[PAGE_SIZE];
    fseek(backing_store, page_number * PAGE_SIZE, SEEK_SET);
    fread(page, sizeof(char), PAGE_SIZE, backing_store);
    char value = page[offset];
    printf("pn %d, os %d, val %d", page_number, offset, value);
}

/**
 * @brief Translates a logical address to a physical address using the TLB and page table.
 *
 * @param logical_address The logical address to translate.
 * @return The corresponding physical address.
 */
void translate_address(int logical_address)
{
    // Extract the page number and offset from the logical address
    int page_number = (logical_address >> 8) & 0xFF;
    int page_offset = logical_address & 0xFF;

    int frame_number; // result

    printf("NUMBER: %d, OFFSET: %d, PAGE NUM: %d\n", logical_address, page_offset, page_number);

    frame_number = search_tlb(page_number);
    // if(frame_number == -1){} // tlb miss

    frame_number = check_page_table(page_number);
    if (frame_number != -1)
    {
        printf("it in da page table bruh!");
        printf("FRAME NUM: %d\n", frame_number);
    }
    else
    {
        search_backing_store(page_number, page_offset);
        printf("backing store!\n");
    }
    // page number first goes to TLB to find frame number, if not TLB then go to page table for frame number. If page fault, go to backing store.
}

int main(int argc, char const *argv[])
{
    printf("Program Start\n");

    init_page_table();

    check_page_table(4); // test func, remove

    update_page_table(4, 251); // test func, remove

    check_page_table(4); // test func, remove

    flush_tlb(); // flushes and resets the TLB to prepare for the main functionality.

    FILE *fp = fopen("addresses.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }

    // Loop through the file until the end is reached
    while (!feof(fp))
    {
        // Read a logical address from the file
        int logical_address;
        if (fscanf(fp, "%d", &logical_address) == 1)
        {
            translate_address(logical_address);
        }
    }

    // Close the input file
    fclose(fp);

    // begin while loop that reads in all addresses.

    /* while loop should:
        Translate logical address to physical address
        Extract Byte Value (signed char at physical address)
    */

    printf("Program Ended Successfully!\n");
    return 0;
}
