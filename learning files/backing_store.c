#include <stdio.h>
#include <stdlib.h>

// compile command: gcc -o bs -O3 backing_store.c

int main(int argc, char const *argv[])
{
    FILE *fp = fopen("BACKING_STORE.bin", "rb");

    int page_number = atoi(argv[1]);

    fseek(fp, page_number * 256, SEEK_SET);

    char page[256];
    fread(page, sizeof(char), 256, fp);

    int i = 0;

    int row;

    printf("Page contents:\n");
    for (row = 0; row < 16; row++)
    {   

        printf("%d\n", page[row * 16]);
    }

    fclose(fp);

    return 0;
}
