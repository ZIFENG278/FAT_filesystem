#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "oscafs.h"
#define LEN 20 // neame of operation only 10 character
char *s_gets(char *st, int n);

/*enum os_function
{
    write,
    read,
    copy
};*/
char *function_name[] = {"write", "read", "copy", "ls", "ls -l", "FAT"};

int main(void)
{
    char choice[LEN];
    //enum os_function operation;
    int op = 0;
    bool choice_is_found = false;

    FILE *fp = open_fs("OSCAFS");

    puts("Enter a operation please (empty line to quit) :");
    while (s_gets(choice, LEN) != NULL && choice[0] != '\0')
    {
        for (op = 0; op <= 5; op++) //need to change op <= when add a operation
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
                printf("copy success\n"); //test
                //os_copy();
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
            }

        else
            printf("OSCA FS haven't %s operation\n", choice);

        choice_is_found = false;
        puts("Enter next operation (empty line to quit OSCAFS) :");
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
