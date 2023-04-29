#include <stdio.h>
#include <stdlib.h>

// compile: gcc -o proj3 main.c

#define PAGE_SIZE 256  // 2^8
#define FRAME_SIZE 256 // 2^8

#define NUMBER_OF_FRAMES 256 // 2^8
#define PAGE_TABLE 256       // from book
#define TLB_SIZE 16          // 16 entries in the page table

#define BUFFER 10 // stores logical addresses from addresses.txt?

#define PAGE_NUMBER_MASK 0x0000FF00 // TWO BITS BEFORE LAST TWO
#define OFFSET_MASK 0x000000FF      // LAST TWO BITS

int n_translated_addresses = 0;
int page_faults = 0;
double pfr = 0;
int tlb_hits = 0;
double thr = 0;

// ============================

int next_frame = 0;

int pageTable[PAGE_TABLE];

// 1 is valid, anything else is invalid.
typedef struct tlb_entry
{
    int page_number;
    int frame_number;
    int valid;

} tlb_entry_t;

signed char physical_memory[NUMBER_OF_FRAMES][FRAME_SIZE]; // representation of physical memory

tlb_entry_t tlb[TLB_SIZE];

int get_next_available_frame()
{
    int frame = next_frame;
    next_frame = (next_frame + 1) % NUMBER_OF_FRAMES;
    return frame;
}

// -1 result = page fault
int check_page_table(int page_num)
{
    return pageTable[page_num];
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
    static int next_tlb_entry = 0;

    tlb[next_tlb_entry].page_number = page_number;
    tlb[next_tlb_entry].frame_number = frame_number;
    tlb[next_tlb_entry].valid = 1;

    next_tlb_entry = (next_tlb_entry + 1) % TLB_SIZE;
}

void init_page_table()
{
    for (int i = 0; i < PAGE_TABLE; i++)
    {
        pageTable[i] = -1;
    }

    printf("Page Table Initialized\n");
    next_frame = 0; // Initialize next_frame to 0 after the page table is initialized
}

/*
    You should read from the backing store and load the data into physical memory.
    To decide where in the physical memory the data should go, you will need to find an available frame.
    You can use a simple counter (like a variable called next_available_frame) that increments each time a frame is assigned. This method works when the physical memory size is the same as the virtual address space size, as you don't need to worry about page replacement.
*/
void search_backing_store(int page_number, signed char *page_data)
{
    // Open the backing store
    FILE *backing_store = fopen("BACKING_STORE.bin", "rb");
    if (backing_store == NULL)
    {
        printf("Error: Could not open BACKING_STORE.bin\n");
        exit(1);
    }

    // Read the entire page from the backing store into the page_data buffer
    fseek(backing_store, page_number * PAGE_SIZE, SEEK_SET);
    fread(page_data, sizeof(signed char), PAGE_SIZE, backing_store);

    // Close the backing store
    fclose(backing_store);
}

int check_physical_address(int frame_number, int offset)
{
    // Check if the frame_number and offset are within bounds
    if (frame_number >= 0 && frame_number < NUMBER_OF_FRAMES && offset >= 0 && offset < FRAME_SIZE)
    {
        // Get the signed char value stored in the physical memory at the given frame number and offset
        signed char stored_value = physical_memory[frame_number][offset];

        // Return the value as an int
        return (int)stored_value;
    }
    else
    {
        printf("Error: Invalid frame_number or offset.\n");
        return -1;
    }
}

void put_in_memory(int frame_number, int offset, char value)
{
    // Check that the frame number and offset are within bounds
    if (frame_number < 0 || frame_number >= NUMBER_OF_FRAMES)
    {
        printf("Error: Frame number %d is out of bounds\n", frame_number);
        exit(1);
    }
    if (offset < 0 || offset >= FRAME_SIZE)
    {
        printf("Error: Offset %d is out of bounds\n", offset);
        exit(1);
    }

    // Store the value in the appropriate location in physical memory
    physical_memory[frame_number][offset] = (signed char)value;
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

    int frame_number; // used to find result with (frame_number * FRAME_SIZE) + offset

    int value = 0;            // result to be printed
    int physical_address = 0; // result

    frame_number = search_tlb(page_number);
    if (frame_number != -1)
    {
        tlb_hits++;
        printf("TLB Hit!\n");
        physical_address = (frame_number * FRAME_SIZE) + page_offset;
        value = check_physical_address(frame_number, page_offset);

    } // tlb hit
    else
    {  
        frame_number = check_page_table(page_number);
        if (frame_number != -1)
        {
            //printf("from page \n");
            physical_address = (frame_number * FRAME_SIZE) + page_offset;
            value = check_physical_address(frame_number, page_offset);
            // handle page hit
        }
        else
        {
            page_faults++;
            printf("Page Fault!\n");
            // Read the entire page from the backing store
            signed char page_data[FRAME_SIZE];
            search_backing_store(page_number, page_data);

            frame_number = get_next_available_frame();

            // Store the entire page in physical memory
            for (int i = 0; i < FRAME_SIZE; i++)
            {
                put_in_memory(frame_number, i, page_data[i]);
            }

            // Get the value from the correct offset within the page
            value = page_data[page_offset];

            physical_address = (frame_number * FRAME_SIZE) + page_offset;

            update_page_table(page_number, frame_number);

            update_tlb(page_number, frame_number);
            // look in backing store, send to physical memory, send to page table, send to tlb.
        }
    }

    // printf("Virtual Address: %d, Physical Address %d, Value: %d\n", logical_address, physical_address, value);

    printf("Virtual Address: %d, Physical Address %d, Value: %d, Page Offset: %d, Page Number: %d, Frame Number: %d\n", logical_address, physical_address, value, page_offset, page_number, frame_number);
    
    // page number first goes to TLB to find frame number, if not TLB then go to page table for frame number. If page fault, go to backing store.
}

void print_stats(){
    pfr = (float) page_faults / n_translated_addresses;
    thr = (float) tlb_hits / n_translated_addresses;
    printf("Number of Translated Addresses: %d\nPage Faults: %d\nPage Fault Rate: %f\nTLB Hits: %d\nTLB Hit Rate: %f\n", n_translated_addresses, page_faults, pfr, tlb_hits, thr);
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
            n_translated_addresses++;
        }
    }

    // Close the input file
    fclose(fp);

    print_stats();

    printf("Program Ended Successfully!\n");
    return 0;
}
