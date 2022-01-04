#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "oscafs.h"
//Error value
#define NO_DENTRY -1

str_t dentry[NUM_DENTRY];

void clear_entry(int i)
{
    dentry[i].str[0] = '\0';
    dentry[i].num = 0;
}

void init_dentry(void)
{
    // Write "" in each dentry row
    for (int i = 0; i < NUM_DENTRY; i++)
    {
        //snprintf(dentry[i].str, STR_LEN, "");
        clear_entry(i);
    }
}

str_t *get_dentry()
{
    return dentry;
}

char *return_entry(int i)
{
    return dentry[i].str;
}

int check_entry(int i)
{
    return dentry[i].str[0] == '\0';
}

int return_first_entry()
{
    for (int i = 0; i < NUM_DENTRY; i++)
        if (check_entry(i))
            return i;

    return NO_DENTRY;
}

/* copy string str into entry i
   return length of copied string
*/
int write_entry(int i, char *str)
{
    char *dest = strncpy(dentry[i].str, str, NAME_LEN);
    dentry[i].str[NAME_LEN] = '\0';

    return strlen(dest);
}

int write_first_free(char *str)
{
    int i = return_first_entry();
    if (i >= 0)
        write_entry(i, str);

    return i;
}

int write_firstblock_num(int i)
{
    dentry[i].num = alloc_block();
    return dentry[i].num;
}