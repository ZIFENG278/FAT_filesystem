#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
//#include "osca.h"
#define LEN 10
char *s_gets(char *st, int n);

enum os_function
{
    write,
    read,
    copy
};
const char *function_name[] = {"write", "read", "copy"};

int main(void)
{
    char choice[LEN];
    enum os_function operation;
    bool choice_is_found = false;

    //FILE *fp = open_fs("OSCAFS");

    puts("Enter a operation please (empty line to quit) :");
    while (s_gets(choice, LEN) != NULL && choice[0] != '\0')
    {
        for (operation = write; operation <= copy; operation++)
        {
            if (strcmp(choice, function_name[operation]) == 0)
            {
                choice_is_found = true;
                break;
            }
        }

        if (choice_is_found)
            switch (operation)
            {
            case write:
                printf("write success\n"); //test
                //fs_write();
                break;

            case read:
                printf("read success\n"); //test
                //fs_read();
                break;

            case copy:
                printf("copy success\n"); //test
                //fs_copy();
                break;
            }

        else
            printf("OSCA FS haven't %s operation\n", choice);

        choice_is_found = false;
        puts("Enter next operation (empty line to quit) :");
    }

    //close_fs(fp);
    puts("OSCA FS close");

    return 0;
}

char *s_gets(char *st, int n)
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