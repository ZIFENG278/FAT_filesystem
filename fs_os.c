#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include "oscafs.h"
//#define NAME_LEN 128
extern str_t dentry[NUM_DENTRY];
extern int fat[NB_BLOCKS];

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
            fprintf(stderr, "Cannot open or create the \"%s\" file", fs);
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
    char str[NAME_LEN];
    //char data[BLOCK_SIZE];
    char spdata[TIME_BLOCK * BLOCK_SIZE];
    char *pt;
    bool same_name = false;
    pt = spdata;
    int count_alloc;
    if ((return_first_entry()) >= 0)
    {
        printf(YELLOW "Enter the name of file: " NONE);
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
                printf("the file \"%s\" have been exist, cannot creat same name file, try again(empty to qiut)\n", str);
                //puts("try again");
                same_name = false;
                continue;
            }

            else
            {
                puts(YELLOW "Please input data:" NONE);
                s_gets(spdata, TIME_BLOCK * BLOCK_SIZE); // try to write data size bigger than BLOCK_SIZE
                //int data_length;
                //printf("spdata: %s\n", spdata); //fot test
                spdata[TIME_BLOCK * BLOCK_SIZE] = '\0';
                //printf("spdata strlen is %d\n", strlen(spdata));
                int num_block;
                data_length = strlen(spdata);
                if (data_length % BLOCK_SIZE == 0)
                    num_block = data_length / BLOCK_SIZE;
                else
                    num_block = (data_length / BLOCK_SIZE) + 1;

                //printf("this file use %d block\n", num_block); //for test
                //puts("blocks: ");// for test
                if (num_block <= count_free_block())
                {
                    fd = write_first_free(str);
                    if (fd < 0)
                    {
                        printf("OSCAFS cannot contain more than %d file\n", NUM_DENTRY);
                        break;
                    }
                    fb = write_firstblock_num(fd);
                    count_alloc = 1;
                    //printf("%s is No.%d in dentry and ", str, fd);
                    //printf("begin in %d block\n", fb);
                    char databuf[num_block][BLOCK_SIZE];
                    for (int count = 0; count < num_block; count++)
                    {
                        //char databuf[BLOCK_SIZE];
                        strncpy(databuf[count], pt + (sizeof(spdata) - (TIME_BLOCK - count) * BLOCK_SIZE), BLOCK_SIZE);
                        databuf[count][BLOCK_SIZE] = '\0';
                        //printf("databuf%d: %-8s | in block: %d\n", count + 1, databuf[count], fb); //for check data if correct
                        write_block(fb, databuf[count], fp);
                        //printf("%d ", fb); // for test
                        if (count_alloc < num_block)

                        {
                            nb = add_next_block(fb);
                            fb = nb;
                            count_alloc++;
                        }

                        //printf("next block ready write in %d\n", nb); //for test
                    }
                    printf(YELLOW "Enter file name to write new one (empty to quit): " NONE);
                }

                else
                {
                    puts("OSCAFS not enough spare sapce to save this file");
                    break;
                    //printf("need %d block\n", num_block);  //for test
                    //printf("spare blocks:%d\n", count_free_block());  //for test
                }
            }
        }
    }

    else
        printf("OSCAFS cannot contain more than %d file\n", NUM_DENTRY);
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

void os_list(void)
{
    int i;
    for (i = 0; i < NUM_DENTRY; i++)
    {
        if (dentry[i].str[0] != '\0')
            printf("%s  ", dentry[i].str); //for test
    }
    printf("\n");
}

void os_list_detial(void)
{
    int i;
    int fb;
    int temp;
    for (i = 0; i < NUM_DENTRY; i++)
    {
        if (dentry[i].str[0] != '\0')
        {
            printf("No.%d: %-16s", i, dentry[i].str);
            printf("blocks: %d ", dentry[i].num);
            fb = dentry[i].num;
            while (fat[fb] != EOF_BLK)
            {
                temp = next_block(fb);
                fb = temp;
                printf("%d ", fb);
            }

            printf("\n");
        }
    }
}

void os_read(FILE *fp) // mistake in can not identif the input name
{
    int i;  //for check empty dentry
    int j;  //for select file
    int fb; // first block in file
    bool have_file = false;
    char file_name[NAME_LEN];
    char data[BLOCK_SIZE];
    int temp; //for change the value of fat table
    for (i = 0; i < NUM_DENTRY; i++)
    {
        if (dentry[i].str[0] != '\0')
        {
            have_file = true;
        }
    }

    if (have_file)
    {
        //printf("\n");
        os_list();
        printf(YELLOW "please choice a file: " NONE);
        bool true_name = false;
        while (s_gets(file_name, NAME_LEN) != NULL && file_name[0] != '\0')
        {
            for (j = 0; j < NUM_DENTRY; j++)
            {
                if (strcmp(file_name, dentry[j].str) == 0)
                {
                    true_name = true;
                    break;
                }
            }

            if (true_name)
            {
                //printf("\n");
                fb = dentry[j].num;
                // printf("%d\n", fb); //for test
                read_block(fb, data, fp);
                data[BLOCK_SIZE] = '\0';
                printf("%s", data);
                while (fat[fb] != EOF_BLK)
                {
                    char bufdata[BLOCK_SIZE];
                    temp = next_block(fb);
                    fb = temp;
                    read_block(fb, bufdata, fp);
                    bufdata[BLOCK_SIZE] = '\0';
                    printf("%s", bufdata);
                }
                printf("\n");
                //printf(YELLOW "\ndone\n" NONE);
                printf(YELLOW "Enter file name to read other one (empty to quit): " NONE);
            }

            else
            {
                printf("\"%s\" can not find in OSCAFS\n", file_name);
                printf(YELLOW "please try again (empty to quit): " NONE);
                continue;
            }

            true_name = false;
        }
    }

    else
        puts("No any file");
}

void os_fat(void)
{
    printf(YELLOW "File Allocation Table:\n" NONE);
    for (int i = 0; i < NB_BLOCKS; i++)
    {
        printf("%d %d\n", i, get_fat()[i]);
    }
}

void os_seek(void)
{
    int i;
    int j;
    int count = 0;
    char seek_input[NAME_LEN];
    char *pt;
    printf(YELLOW "please input the file name: " NONE);
    while (s_gets(seek_input, NAME_LEN) != NULL && seek_input[0] != '\0')
    {
        for (i = 0; i < NUM_DENTRY; i++)
        {
            pt = dentry[i].str;
            for (j = 0; j < strlen(dentry[i].str); j++)
            {
                if (strncmp(pt + j, seek_input, strlen(seek_input)) == 0)
                {
                    printf("'%s'  ", dentry[i].str);
                    count++;
                    break;
                }
            }
        }
        printf("\n");
        printf(GREEN "%d result\n" NONE, count);
        printf(YELLOW "input other file name to seek(empty to quit): " NONE);
        count = 0;
    }
}