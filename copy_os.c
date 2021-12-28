#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include "oscafs.h"

int file_size(char *filename)
{
    struct stat statbuf;
    stat(filename, &statbuf);
    int size = statbuf.st_size;

    return size;
}

/*void os_copy_OSCAFS(FILE *fp);
{
    char file_name[NAME_LEN];

    puts("please input a file name");
    os_list();
}*/

void os_copy_host(FILE *fp)
{
    FILE *file_host;
    int fd;          // dentry row number
    int fb;          //first block
    int nb;          // next block number
    int data_length; // Count the number of characters
    //char str[NAME_LEN];
    char file_name_copy[NAME_LEN];
    //char data[BLOCK_SIZE];
    //char spdata[5 * BLOCK_SIZE];
    char *pt;
    bool same_name = false;
    //pt = spdata;
    int count_alloc;
    if ((return_first_entry()) >= 0)
    {
        puts("please input a host file name");

        while (s_gets(file_name_copy, NAME_LEN) != NULL && file_name_copy[0] != '\0') // check is it have same name file
        {
            if ((file_host = fopen(file_name_copy, "r+")) == NULL)
            {
                fprintf(stderr, "Cannot open the %s file\n", file_name_copy);
                puts("please try other file");
                continue;
            }

            for (int i = 0; i < NUM_DENTRY; i++)
            {
                if (strcmp(file_name_copy, return_entry(i)) == 0)
                {
                    same_name = true;
                    break;
                }
            }

            if (same_name)
            {
                printf("the file %s have been exist, cannot copy same name file\n", file_name_copy);
                puts("try again");
                same_name = false;
                continue;
            }

            else
            {
                int num_block;
                int size = file_size(file_name_copy) - 1;
                //printf("%d\n", size); //for test
                if (size % BLOCK_SIZE == 0)
                    num_block = size / BLOCK_SIZE;
                else
                    num_block = (size / BLOCK_SIZE) + 1;

                //printf("num_block:%d\n", num_block); //for test

                char spdata[num_block * BLOCK_SIZE];
                pt = spdata;
                fread(spdata, size, 1, file_host);
                spdata[num_block * BLOCK_SIZE] = '\0';
                printf("spdata:%s\n", spdata); // for test
                printf("spdata strlen is %d\n", strlen(spdata));
                fd = write_first_free(file_name_copy);
                if (fd < 0)
                {
                    printf("OSCAFS cannot contain more %d file\n", NUM_DENTRY);
                    break;
                }
                fb = write_firstblock_num(fd);

                printf("%s is No.%d in dentry and ", file_name_copy, fd);
                printf("begin in %d block\n", fb);
                count_alloc = 1;
                //puts("Please input data:");
                //s_gets(spdata, TIME_BLOCK * BLOCK_SIZE); // try to write data size bigger than BLOCK_SIZE
                //int data_length;
                //printf("spdata: %s\n", spdata); //fot test
                //spdata[count_alloc * BLOCK_SIZE] = '\0';
                //printf("spdata strlen is %d\n", strlen(spdata));

                //int num_block;
                //data_length = strlen(spdata);
                //if (data_length % BLOCK_SIZE == 0)
                // num_block = data_length / BLOCK_SIZE;
                //else
                // num_block = (data_length / BLOCK_SIZE) + 1;

                printf("this file use %d block\n", num_block);
                char databuf[num_block][BLOCK_SIZE];
                for (int count = 0; count < num_block; count++)
                {
                    //char databuf[BLOCK_SIZE];
                    strncpy(databuf[count], pt + (sizeof(spdata) - (num_block - count) * BLOCK_SIZE), BLOCK_SIZE);
                    databuf[count][BLOCK_SIZE] = '\0';
                    printf("databuf%d: %-8s | in block: %d\n", count + 1, databuf[count], fb); //for check data if correct
                    write_block(fb, databuf[count], fp);
                    //printf("%d ", fb); // for test
                    if (count_alloc < num_block)
                    {
                        nb = add_next_block(fb);
                        fb = nb;
                        count_alloc++;
                    }
                }
                puts("Enter file name to copy new one to OSCAFS(empty to quit)");
            }
        }
    }

    else
        printf("OSCAFS cannot contain more %d file\n", NUM_DENTRY);
}

/*FILE *open_fs(char *fs) // open OSCAFS
{
    FILE *fp;

    if ((fp = fopen(fs, "r+")) == NULL)
    {
        fprintf(stderr, "Cannot open the %s file", fs);
        return NULL;
    }
    else
    {
        rewind(fp);
        fread(get_dentry(), sizeof(str_t), NUM_DENTRY, fp);
        fread(get_fat(), sizeof(int), NB_BLOCKS, fp);
    }
    return fp;
}*/