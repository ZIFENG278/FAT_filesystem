#include "oscafs.h"

int blocks_offset()
{
    return sizeof(str_t) * NUM_DENTRY + sizeof(int) * NB_BLOCKS;
}

int block_offset(int b)
{
    return sizeof(str_t) * NUM_DENTRY + sizeof(int) * NB_BLOCKS + b * BLOCK_SIZE;
}

int read_block(int b, char *buf, FILE *fp)
{
    fseek(fp, block_offset(b), SEEK_SET);
    return fread(buf, 1, BLOCK_SIZE, fp);
}

int write_block(int bn, char *buf, FILE *fp)
{
    fseek(fp, block_offset(bn), SEEK_SET);
    return fwrite(buf, 1, BLOCK_SIZE, fp);
}