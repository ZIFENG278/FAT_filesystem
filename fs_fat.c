#include "oscafs.h"

#define FREE_BLK 0
//#define EOF_BLK -1

// Error Values
#define NO_FREE_BLK -2
#define NOT_EOF -3

int fat[NB_BLOCKS];

void init_fat()
{
    for (int i = 0; i < NB_BLOCKS; i++)
        fat[i] = FREE_BLK;
}

int *get_fat()
{
    return fat;
}

int eof_block(int i)
{
    return fat[i] == EOF_BLK;
}

int next_block(int i)
{
    return fat[i];
}

int first_free_block()
{
    for (int i = 1; i < NB_BLOCKS; i++)
    {
        if (fat[i] == FREE_BLK)
        {
            return i;
        }
    }
    return NO_FREE_BLK;
}

int alloc_block()
{
    int i;

    if ((i = first_free_block()) > 0)
    {
        fat[i] = EOF_BLK;
        return i;
    }

    return NO_FREE_BLK;
}

int add_next_block(int b)
{
    int i;

    if (fat[b] != EOF_BLK)
        return NOT_EOF;

    if ((i = first_free_block()) > 0)
    {
        fat[b] = i;
        fat[i] = EOF_BLK;
        return i;
    }

    return NO_FREE_BLK;
}

int count_free_block(void)
{
    int i;
    int count = 0;
    for (i = 1; i < NB_BLOCKS; i++)
    {
        if (fat[i] == FREE_BLK)
        {
            count++;
        }
    }

    return count;
}

void init_single_fat(int i)
{
    fat[i] = FREE_BLK;
}
