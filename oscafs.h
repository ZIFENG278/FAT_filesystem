#include <stdio.h>

#define NB_BLOCKS 512
#define BLOCK_SIZE 8

#define NUM_DENTRY 5
#define STR_LEN 256
#define TIME_BLOCK 5 // for write maximum 5 block for one time

/*
  Directory Entry definition
 */

typedef struct
{
    int num;
    char str[STR_LEN];
} str_t;

void clear_entry(int i);

void init_dentry(void);

str_t *get_dentry();

char *return_entry(int i);

int check_entry(int i);

int return_first_entry();

/* copy string str into entry i
   return length of copied string
*/
int write_entry(int i, char *str);

int write_first_free(char *str);

int write_firstblock_num(int i);

/* 
   Block Table definitions
*/
void init_fat();

int *get_fat();

int eof_block(int i);

int next_block(int i);

int first_free_block();

int alloc_block();

int add_next_block(int b);

/* Block Set
 */
int block_offset(int b);

int read_block(int b, char *buf, FILE *fp);

int write_block(int bn, char *buf, FILE *fp);

//os function
char *s_gets(char *st, int n);

FILE *open_fs(char *su_file);

void close_fs(FILE *fp);

void os_write_first(FILE *fp);
