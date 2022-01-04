#include <stdio.h>

#define NB_BLOCKS 512 //inode
#define BLOCK_SIZE 1024

#define NUM_DENTRY 5 //number of file can contain
//#define STR_LEN 256
#define TIME_BLOCK 5 // for write maximum 5 block for one time
#define NAME_LEN 256

#define EOF_BLK -1
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define NONE "\033[0m"

/*
  Directory Entry definition
 */

typedef struct
{
   int num;
   char str[NAME_LEN];
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

int count_free_block(void);

void init_single_fat(int i);

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

void os_list(void);

void os_list_detial(void);

void os_read(FILE *fp);

void os_fat(void);

void os_seek(void);

void os_delete(FILE *fp);

//os copy function
int file_size(char *filename);

void os_copy_host(FILE *fp);

void os_copy_OSCAFS(FILE *fp);