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

// ==========STATS=============

int n_translated_addresses = 0; // how many addresses translated from addresses.txt
int page_faults = 0; // how many times the page table did not have the requested content
double pfr = 0; // page fault rate
int tlb_hits = 0; // how many times the TLB was successfully accessed.
double thr = 0; // tlb hit rate

// ============================

int next_frame = 0; // representation of the next available frame

int pageTable[PAGE_TABLE]; // representation of a page table


/// @brief struct representation of an entry in the TLB, valid bit is for replacement algorithms
typedef struct tlb_entry
{
    int page_number;
    int frame_number;
    int valid;

} tlb_entry_t;

signed char physical_memory[NUMBER_OF_FRAMES][FRAME_SIZE]; // representation of physical memory

tlb_entry_t tlb[TLB_SIZE]; // array representation of the TLB, with TLB entry structs.

// ==========DECLARATIONS==============

void init_page_table();
int check_page_table(int page_num);
void update_page_table(int page_num, int frame_num);
void flush_tlb();
void init_tlb();
int search_tlb(int page_number);
void update_tlb(int page_number, int frame_number);
int get_next_available_frame();
void put_in_memory(int frame_number, int offset, char value);
int check_physical_address(int frame_number, int offset);
void search_backing_store(int page_number, signed char *page_data);
void translate_address(int logical_address);
void print_stats();
int main(int argc, char const *argv[]);

// ==========IMPLEMENTATION=============

/// @brief Returns the number of the next available frame for use in physical memory.
/// @return The number of the next available frame.
///
/// Note that this implementation does not include a replacement strategy for when all frames are in use.
int get_next_available_frame()
{
    int frame = next_frame;
    next_frame = (next_frame + 1) % NUMBER_OF_FRAMES;
    return frame;
}

/// @brief Responsible for checking if the page table entry for the given page number exists and returns the corresponding frame number if found.
/// @param page_num The page number whose page table entry is to be checked.
/// @return The frame number stored in the page table entry for the given page number, or -1 if the entry does not exist.
int check_page_table(int page_num)
{
    if (page_num < 0 || page_num >= PAGE_TABLE)
    {
        printf("Error: Invalid page number: %d\n", page_num);
        return -1;
    }
    return pageTable[page_num];
}

/// @brief Updates the page table entry for the given page number with the given frame number.
/// @param page_num The page number whose entry is to be updated.
/// @param frame_num The frame number to be stored in the page table entry.
void update_page_table(int page_num, int frame_num)
{
    if (page_num < 0 || page_num >= PAGE_TABLE)
    {
        printf("Error: Invalid page number: %d\n", page_num);
        return;
    }
    if (frame_num < 0 || frame_num >= NUMBER_OF_FRAMES)
    {
        printf("Error: Invalid frame number: %d\n", frame_num);
        return;
    }
    pageTable[page_num] = frame_num;
}

/// @brief Initializes the TLB by flushing all entries and setting them to be invalid.
void flush_tlb()
{
    for (int i = 0; i < TLB_SIZE; i++)
    {
        tlb[i].valid = 0;
    }
}

/// @brief Responsible for searching the TLB for the given page number, and returns the corresponding frame number if it is found.
/// @param page_number The page number to search for in the TLB.
/// @return The frame number corresponding to the given page number, if it is found in the TLB. Otherwise, returns -1 to indicate a TLB miss.
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

/// @brief Function responsible for placing information into the TLB.
/// @param page_number Which page to update
/// @param frame_number At which frame corresponds to the given page number.
void update_tlb(int page_number, int frame_number)
{
    static int next_tlb_entry = 0;

    tlb[next_tlb_entry].page_number = page_number;
    tlb[next_tlb_entry].frame_number = frame_number;
    tlb[next_tlb_entry].valid = 1;

    next_tlb_entry = (next_tlb_entry + 1) % TLB_SIZE;
}

/// @brief Initializes the page table, sets all values to -1 indicating nothing stored.
void init_page_table()
{
    for (int i = 0; i < PAGE_TABLE; i++)
    {
        pageTable[i] = -1;
    }

    printf("Page Table Initialized\n");
}


/// @brief Function responsible for searching the BACKING_STORE.bin file.
/// @param page_number The page number to read from
/// @param page_data The data structure passed in to store the data to.
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

/// @brief Function responsible for retrieving a value at a given frame and offset.
/// @param frame_number The frame to retrieve from
/// @param offset The place within the frame to retrieve from
/// @return The value stored at the given location.
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

/// @brief Puts a value in the representation of physical memory given a frame_number and offset.
/// @param frame_number The frame to place the value at.
/// @param offset Where in the frame to place the value at.
/// @param value The value to be placed.
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
 */
void translate_address(int logical_address)
{
    // Extract the page number and offset from the logical address
    int page_number = (logical_address >> 8) & 0xFF;
    int page_offset = logical_address & 0xFF;

    int frame_number; // Used to find result with (frame_number * FRAME_SIZE) + offset

    int value = 0;            // Result to be printed
    int physical_address = 0; // Result

    // Search for the page number in the TLB
    frame_number = search_tlb(page_number);

    if (frame_number != -1) // TLB hit
    {
        tlb_hits++;
        printf("TLB Hit!\n");
    }
    else // TLB miss
    {
        // Check if the page number is in the page table
        frame_number = check_page_table(page_number);

        if (frame_number != -1) // Page hit
        {
        }
        else // Page fault
        {
            page_faults++;
            printf("Page Fault!\n");

            // Read the entire page from the backing store
            signed char page_data[FRAME_SIZE];
            search_backing_store(page_number, page_data);

            int next_available_frame_number = get_next_available_frame();
            if (next_available_frame_number != frame_number)
            {
                frame_number = next_available_frame_number;
            }

            // Store the entire page in physical memory
            for (int i = 0; i < FRAME_SIZE; i++)
            {
                put_in_memory(frame_number, i, page_data[i]);
            }
        }
    }

    update_tlb(page_number, frame_number);
    // Update the page table with the new frame number
    update_page_table(page_number, frame_number);

    // Calculate the physical address and fetch the value
    physical_address = (frame_number * FRAME_SIZE) + page_offset;
    value = check_physical_address(frame_number, page_offset);

    printf("Virtual Address: %d, Physical Address %d, Value: %d\n", logical_address, physical_address, value);

    //printf("Virtual Address: %d, Physical Address %d, Value: %d, Page Offset: %d, Page Number: %d, Frame Number: %d\n", logical_address, physical_address, value, page_offset, page_number, frame_number); debug version
}
/// @brief Function responsible for calculating and printing stats.
void print_stats()
{
    pfr = (float)page_faults / n_translated_addresses;
    thr = (float)tlb_hits / n_translated_addresses;
    printf("Number of Translated Addresses: %d\nPage Faults: %d\nPage Fault Rate: %f\nTLB Hits: %d\nTLB Hit Rate: %f\n", n_translated_addresses, page_faults, pfr, tlb_hits, thr);
}

/// @brief Function responsible for organizing information and calling all helper files in order to read from
/// address.txt and provide both statistics and the physical address translation + value stored at address.
/// @return 
int main(int argc, char const *argv[])
{
    printf("Program Start\n");

    init_page_table();

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
