#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "oscafs.h"
//#define LEN 256 // neame of command only 20 character
char *s_gets(char *st, int n);

/*enum os_function
{
    write,
    read,
    copy
};*/
char *function_name[] = {"write", "read", "copy host", "ls", "ls -l", "FAT", "copy OSCAFS", "seek", "rm"};

int main(void)
{
    char choice[NAME_LEN];
    //enum os_function command;
    int op = 0;
    bool choice_is_found = false;

    FILE *fp = open_fs("OSCAFS");

    printf(GREEN "Enter a command please (empty to quit): " NONE);
    while (s_gets(choice, NAME_LEN) != NULL && choice[0] != '\0')
    {
        for (op = 0; op <= 8; op++) //need to change op <= when add a command
        {
            if (strcmp(choice, function_name[op]) == 0)
            {
                choice_is_found = true;
                break;
            }
        }

        if (choice_is_found)
            switch (op)
            {
            case 0:
                //printf("write success\n");
                os_write_first(fp);
                break;

            case 1:
                //printf("read success\n"); //test
                os_read(fp);
                break;

            case 2:
                //printf("copy success\n"); //test
                os_copy_host(fp);
                //printf("%d\n", file_size2()); //for test
                //printf("%d\n", file_size()); //for test

                break;

            case 3:
                os_list();
                break;

            case 4:
                os_list_detial();
                break;

            case 5:
                os_fat();
                break;

            case 6:
                os_copy_OSCAFS(fp);
                break;

            case 7:
                os_seek();
                break;

            case 8:
                os_delete(fp);
                //puts("delete success");
                break;
            }

        else
            printf("OSCAFS haven't \"%s\" command\n", choice);

        choice_is_found = false;
        printf(GREEN "Enter next command (empty to quit OSCAFS): " NONE);
    }

    close_fs(fp);
    puts("OSCAFS close");

    return 0;
}

/*char *s_gets(char *st, int n)
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
}*/