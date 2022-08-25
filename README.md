# FAT File system

> OSCAFS Project
>
> Developer: Zifeng ZHANG
>
> Tutor: Sami TAKTAK
>
> Date:09/01/2022



## Introduction
The file system OSCAFS is designed based on FAT. This file system includes directory entries, FAT tables and data blocks.

OSCAFS is currently designed as a file system that can hold 512KB and can store 128 files.(The file system capacity and file number data can be changed in NB_BLOCKS and NUM_DENTRY in the header file oscafs.h).

OSCAFS uses the FAT table to index data blocks, so file writing and file reading use a chain structure to ensure that there is no waste of data blocks between disk blocks.



## File content information

> **All OSCAFS programs are written in 6 C files, 1 header file and one Makefile:**

1. osca_fs.c is the main program.

2. fs_os.c contains the code for most of the command functions of the file system,including OSCAFS open and close functions and functions related to input processing.

3. copy_os.c contains all the code related to the copy function.

4. fs_fat.c contains the code related to processing the FAT table.

5. fs_dentry.c contains the code for processing directory entries.

6. fs_block.c contains the code that handles writing data blocks.

7. oscafs.h is the header file of all functions.

8. Makefile is a compiled file.

   

## The OSCAFS file system program has 9 commands in total: 
1. **"write"** : To creat a file and input content
2. **"read"**  : To read file content
3. **"seek"** :  Search files by file name
4. **"copy host"**  :  Copy file from local to OSCAFS
5. **"copy OSCAFS" ** : Copy OSCAFS file to local
6. **"rm"** : Delet file
7. **"ls"**  : List all files
8. **"ls -l"** :  List files detail information
9. **"FAT "** :  Show FAT table

- The codes of "write", "read", "seek", "ls", "ls -l", "rm", and "FAT" are stored in fs_os.c. 
- The codes of "copy host" and "copy OSCAFS" are stored in copy_os.c.
- In the OSCAFS program, all empty lines (empty line’\n’) are used to roll back the function mode to the command line interface until the OSCAFS file system is closed.That is to say, return to the command line interface and eject the file system with the Enter key.





## Linux Usage

- clone the repository 

  ```bash
  git clone https://github.com/ZIFENG278/FAT_filesystem.git
  ```

  or download zip file and unzip

  ```bash
  wget https://github.com/ZIFENG278/FAT_filesystem/archive/refs/heads/master.zip
  ```

- compile source code

  ```bash
  make
  ```

- run execution file

  ```bash
  ./osca_fs
  ```

  

