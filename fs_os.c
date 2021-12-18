#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "oscafs.h"
#define NAME_LEN 128

char *s_gets(char *st, int n) //instead gets
{
    char *ret_val;
    char *find;

    ret_val = fgets(st, n, stdin);
    if (ret_val)
    {
        find = strchr(st, '\n'); // fine the "\n"
        if (find)                //if the address is not a NULL
            *find = '\0';        //put a NULL

        else
            while (getchar() != '\n')
                continue; //clean the input cache
    }
    return ret_val;
}

FILE *open_fs(char *fs) // open OSCAFS
{
    FILE *fp;

    if ((fp = fopen(fs, "r+")) == NULL)
    {
        if ((fp = fopen(fs, "w+")) == NULL)
        {
            fprintf(stderr, "Cannot open or create the %s file", fs);
            return NULL;
        }
        else
        {
            init_dentry();
            init_fat();
            //init_test_fs(fp); // only for test purpose
        }
    }
    else
    {
        rewind(fp);
        fread(get_dentry(), sizeof(str_t), NUM_DENTRY, fp);
        fread(get_fat(), sizeof(int), NB_BLOCKS, fp);
    }
    return fp;
}

void close_fs(FILE *fp)
{
    rewind(fp);

    fwrite(get_dentry(), sizeof(str_t), NUM_DENTRY, fp);
    fwrite(get_fat(), sizeof(int), NB_BLOCKS, fp);

    fclose(fp);
}

void os_write_first(FILE *fp)
{
    int fd;          // dentry row number
    int fb;          //first block
    int nb;          // next block number
    int data_length; // Count the number of characters
    char str[STR_LEN];
    //char data[BLOCK_SIZE];
    char spdata[5 * BLOCK_SIZE];
    char *pt;
    bool same_name = false;
    pt = spdata;
    if ((return_first_entry()) >= 0)
    {
        puts("Enter the name of file:");
        while (s_gets(str, NAME_LEN) != NULL && str[0] != '\0') // check is it have same name file
        {
            for (int i = 0; i < NUM_DENTRY; i++)
            {
                if (strcmp(str, return_entry(i)) == 0)
                {
                    same_name = true;
                    break;
                }
            }

            if (same_name)
            {
                printf("the file %s have been exist, cannot creat same name file\n", str);
                puts("try again");
                same_name = false;
                continue;
            }

            else
            {
                fd = write_first_free(str);
                if (fd < 0)
                {
                    printf("OSCAFS cannot contain more %d file\n", NUM_DENTRY);
                    break;
                }
                fb = write_firstblock_num(fd);
                printf("%s is No.%d in dentry and ", str, fd);
                printf("begin in %d block\n", fb);
                puts("Please input data:");
                s_gets(spdata, TIME_BLOCK * BLOCK_SIZE); // try to write data size bigger than BLOCK_SIZE
                //int data_length;
                printf("spdata: %s\n", spdata); //fot test
                printf("data strlen is %d\n", strlen(spdata));

                int num_block;
                data_length = strlen(spdata);
                if (data_length % BLOCK_SIZE == 0)
                    num_block = data_length / BLOCK_SIZE;
                else
                    num_block = (data_length / BLOCK_SIZE) + 1;

                printf("this file use %d block\n", num_block);
                //puts("blocks: ");
                char databuf[num_block][BLOCK_SIZE];
                for (int count = 0; count < num_block; count++)
                {
                    //char databuf[BLOCK_SIZE];
                    strncpy(databuf[count], pt + (sizeof(spdata) - (TIME_BLOCK - count) * BLOCK_SIZE), BLOCK_SIZE);
                    databuf[count][BLOCK_SIZE] = '\0';
                    printf("databuf%d: %s\n", count + 1, databuf[count]); //for check data if correct
                    write_block(fb, databuf[count], fp);
                    //printf("%d ", fb); // for test
                    nb = add_next_block(fb);
                    //printf("next block ready write in %d\n", nb); //for test
                    fb = nb;
                }
                puts("Enter file name to write new one (empty to quit)");

                /*else
                {
                    printf("this file use one block: %d\n", fb);
                    char data[BLOCK_SIZE];
                    strncpy(data, spdata, BLOCK_SIZE);
                    printf("data: %s\n", data); //for test
                    write_block(fb, data, fp);
                    puts("Enter file name to write new one (empty to quit)");
                }*/
                /*s_gets(data, BLOCK_SIZE);
                write_block(fb, data, fp); // write only block 
                puts("Enter file name to write new one (empty to quit write mode)");*/
            }
        }
    }

    else
        printf("OSCAFS cannot contain more %d file\n", NUM_DENTRY);
}

/*void os_write_end(char *filename)
{
    for (int i = 0; i < NUM_DENTRY; i++)
    {
        char temp[BLOCK_SIZE];
        if (strcmp(filename, dentry[i].str) == 0)
            printf("file %s have exit, it would be write at the end\n", filename);
        s_gets(temp, STR_LEN, );
        write_block()
    }
}
*/